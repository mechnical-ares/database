#include "stdafx.h"
#include "BPlusTree.h"
#include "Disk.h"


KeyFunc buildKeyfunc(DataType type){
	auto fn = [&](string l, string r)->int {
		if (l == r)
			return 0;
		Data ld{ type, l };
		Data rd{ type, r };
		if (isLT(ld, rd))
			return -1;
		else
			return 1;
	};
	return fn;
}

int char2int(char s[], int l, int r, int base){
	int ret = 0;
	switch (base){
	case 10:
		if (l > r) return 0;
		for (int i = l; i <= r; i++) if (s[i] != 0){
			ret = ret*base + (s[i] - '0');
		}
		break;
	case 128:
	{
		while (s[l] == 0 && l <= r) l++;
		for (; l <= r; l++){
			ret = ret * 128 + (int)s[l];
		}
	}

	default: break;
	}
	return ret;
}

void int2char(char s[], int l, int r, int value, int base){
	if (base == 0 || base > 128){
		throw "Base error!";
		return;
	}
	int i;
	for (i = l; i <= r; i++)
		s[i] = 0;
	i = r;
	while (value){
		s[i] = value % base;
		value /= base;
		i--;
		if (i < l) break;
	}
}

void string2chars(char s[], int l, int r, string &ss){
	for (int i = r, j = ss.length() - 1; i >= l && j >= 0; i--, j--){
		s[i] = ss[j];
	}
}

string char2string(char s[], int l, int r){
	string ret = "";
	for (int i = l; i <= r; i++) if (s[i] != 0)
		ret.push_back(s[i]);
	return ret;
}

Node::Node(NodeType _type) :type(_type){}
Node::~Node(){}
InteriorNode::~InteriorNode(){}
LeafNode::~LeafNode(){}
BPlusTree::~BPlusTree(){}
InteriorNode::InteriorNode() : Node(INTERIOR){}
InteriorNode::InteriorNode(char s[512], NodeType _type) : Node(_type){
	keys.clear(); pointers.clear();
	int i = 0, length = (int)s[510];
	father = char2int(s, 505, 509, 128);

	for (; i<length * 25; i += 25){
		pointers.push_back(char2int(s, i, i + 4));
		keys.push_back(char2string(s, i + 5, i + 24));
	}
	pointers.push_back(char2int(s, i, i + 4));
}

LeafNode::LeafNode() :Node(LEAF){ type = LEAF; numOfAttrs = 0; }
LeafNode::LeafNode(char s[512], int numOfAttrs) : Node(LEAF), numOfAttrs(numOfAttrs){
	keys.clear(); values.clear();
	int i = 0, j = 0, num = (int)s[510], length = (int)s[509];
	next_page = char2int(s, 504, 508,128);
	prev_page = char2int(s, 499, 503, 128);
	father = char2int(s, 494, 498, 128);
	
	for (; i<num; i++){
		keys.push_back(char2string(s, i * 160, i * 160 + 19));
		row r; r.clear();
		for (j = 0; j<length; j++){
			r.push_back(char2string(s, i * 160 + 20 + j * 20, i * 160 + 39 + j * 20));
		}
		values.push_back(r);
	}
}


void BPlusTree::insert_into_tree(Key& newkey, Value& val){
	Target node = search(root, newkey);
	if (node.second == ROOT){ //build tree, page 1-2,3 ; next_apply <- 4
		char s[512];
		memset(s, 0, sizeof s);
		s[511] = LEAF;
		int2char(s, 499, 503, NUL, 128);//prev_page
		int2char(s, 504, 508, 3, 128);//next_page
		int2char(s, 494, 498, root, 128);//father_page
		LeafNode leaf2(s,this->numOfAttrs);
		leaf2.keys.clear();
		leaf2.values.clear();
		leaf2.Write2Disk(2, this->disk);
		//page 2 complete

		int2char(s, 499, 503, 2, 128);//prev_page
		int2char(s, 504, 508, NUL, 128);//next_page
		int2char(s, 494, 498, root, 128);//father_page
		LeafNode leaf3(s, this->numOfAttrs);
		leaf3.keys.clear();
		leaf3.values.clear();
		leaf3.keys.push_back(newkey);
		leaf3.values.push_back(val);
		leaf3.Write2Disk(3, this->disk);
		//page 3 complete

		//page 1(root)
		memset(s, 0, sizeof s);
		disk.readBlock(root, s);
		//Node* node = createNode(s);
		//InteriorNode *p = dynamic_cast<InteriorNode*>(node);
		InteriorNode& node = InteriorNode(s);
		InteriorNode *p = (InteriorNode*)&node;
		p->keys.clear();
		p->keys.push_back(newkey);
		p->pointers.clear();
		p->pointers.push_back(2); p->pointers.push_back(3);
		p->Write2Disk(root, this->disk);

		this->nextApply = 4;
		WriteHead2Disk();
		return;
	}
	InCons con = insert_into_leaf(node.first, newkey, val);
	while (con.page != 0){
		con = insert_into_interior(con.father, con.key, con.page, con.left);
	}
	WriteHead2Disk();
}

InCons BPlusTree::insert_into_interior(Page page, Key& newkey, Page child, bool left){
	char s[512];
	disk.readBlock(page, s);
	InteriorNode& node = InteriorNode(s);
	InteriorNode *p = (InteriorNode*)&node;
	int size = (int)s[510], type = (int)s[511], father = char2int(s,505,509,128);
	
	vector<Key>::iterator itek = p->keys.begin();
	vector<Page>::iterator itev = p->pointers.begin();

	if (cmp(newkey, p->keys[0]) < 0)
		p->keys.insert(p->keys.begin(), newkey), p->pointers.insert(p->pointers.begin() + 1, child);
	else if (cmp(newkey, p->keys[size - 1]) >= 0)
		p->keys.insert(p->keys.end(), newkey), p->pointers.insert(p->pointers.end(), child);
	else{
		itek++; itev++;
		for (; itek < p->keys.end(); itek++, itev++)
			if (cmp(newkey, *(itek - 1)) >= 0 && cmp(newkey, *itek) < 0)
				p->keys.insert(itek, newkey), p->pointers.insert(itev + 1, child);
	}

	size++;
	if (size < InterCap){
		p->Write2Disk(page, this->disk);
		return InCons();
	}
	else{
		switch (type){
		case ROOT:
		{
			Page newpage1 = nextApply++, newpage2 = nextApply++;
			InteriorNode newinter1, newinter2;
			newinter1.keys.clear(); newinter1.pointers.clear();
			newinter2.keys.clear(); newinter2.pointers.clear();
			int half = size / 2,i=0;
			for (i = 0; i < half; i++){
				newinter1.keys.push_back(p->keys[i]);
				newinter1.pointers.push_back(p->pointers[i]);
			}
			newinter1.pointers.push_back(p->pointers[i]);

			for (i = half + 1; i < size; i++){
				newinter2.keys.push_back(p->keys[i]);
				newinter2.pointers.push_back(p->pointers[i]);
			}
			newinter2.pointers.push_back(p->pointers[i]);

			p->keys.resize(half); p->pointers.resize(half + 1);

			newinter1.father = root; newinter1.type = INTERIOR;
			newinter2.father = root; newinter2.type = INTERIOR;

			Key left = p->keys[half];

			p->keys.clear(); p->pointers.clear();
			p->keys.push_back(left); p->pointers.push_back(newpage1); p->pointers.push_back(newpage2);

			newinter1.Write2Disk(newpage1, disk);
			newinter2.Write2Disk(newpage2, disk);
			p->Write2Disk(root, disk);

			return InCons();
		}
		break;

		case INTERIOR:
		{
			Page newpage = nextApply++;
			InteriorNode newinter;
			newinter.keys.clear(); newinter.pointers.clear();
			int half = size / 2,i;
			for (i = half+1; i < size; i++){
				newinter.keys.push_back(p->keys[i]);
				newinter.pointers.push_back(p->pointers[i]);
			}
			newinter.pointers.push_back(p->pointers[i]);

			Key retKey = p->keys[half];
			p->keys.resize(half); p->pointers.resize(half+1);

			newinter.father = p->father; newinter.type = p->type;

			newinter.Write2Disk(newpage, disk);
			p->Write2Disk(page, disk);

			return InCons(retKey, newpage, father);
		}
		break;

		default:
			return InCons();
			break;
		}
	}
}

InCons BPlusTree::insert_into_leaf(Page page, Key& newkey, Value& val)
{
	char s[512];
	disk.readBlock(page, s);
	LeafNode& node = LeafNode(s,this->numOfAttrs);
	int i,size = (int)s[510];
	LeafNode* p = (LeafNode*)(&node);
	vector<Key>::iterator itek = p->keys.begin();
	vector<Value>::iterator itev = p->values.begin();
	if (size == 0){// node is empty
		p->keys.push_back(newkey), p->values.push_back(val);
	}
	else{
		if (cmp(newkey, p->keys[0]) < 0)
			p->keys.insert(p->keys.begin(), newkey), p->values.insert(p->values.begin(), val);
		else if (cmp(newkey, p->keys[size - 1]) > 0)
			p->keys.insert(p->keys.end(), newkey), p->values.insert(p->values.end(), val);
		else{
			if (cmp(newkey, p->keys[size - 1]) == 0){
				cerr << "Two keys conflict!" << endl;
				throw("Two keys conflict!");
			}
			itek++; itev++;
			for (; itek < p->keys.end(); itek++, itev++){
				if (cmp(newkey, *(itek - 1)) == 0){
					cerr << "Two keys conflict!" << endl;
					throw("Two keys conflict!");
				}
				if (cmp(newkey, *(itek - 1)) >= 0 && cmp(newkey, *itek) < 0)
					p->keys.insert(itek, newkey), p->values.insert(itev, val);
			}
		}
	}	
	size++;
	if (size <= LeafCap){
		p->Write2Disk(page,this->disk);
		return InCons("", 0, 0, 0);
	}

	Page newpage = nextApply++;
	LeafNode newleaf;
	newleaf.keys.clear(); newleaf.values.clear(); newleaf.numOfAttrs = p->numOfAttrs;
	int half = size / 2;
	for (i = size / 2; i < size; i++){
		newleaf.keys.push_back(p->keys[i]);
		newleaf.values.push_back(p->values[i]);
	}
	p->keys.resize(size / 2); p->values.resize(size / 2);

	newleaf.father = p->father; newleaf.type = p->type;
	newleaf.next_page = p->next_page; newleaf.prev_page = page;
	p->next_page = newpage;

	newleaf.Write2Disk(newpage, disk);
	p->Write2Disk(page, disk);

	return InCons(newleaf.keys[0], newpage, newleaf.father);

}

/*
page 0, char[512]:
511:type - NUL
510:num of attributes, (the length of title), no bigger than 10
505~509:the largest applied number, from 0(virtual root, *this), 1(real root), 2(first leaf)
400~419:primaryKey
420:type of primaryKey 
0~199:keys + types (19+1)*10
*/
BPlusTree::BPlusTree(string name, Table* t) :disk(name){
	char s[512];
	disk.readBlock(0, s);
	cmp = buildKeyfunc((DataType)s[419]);
	this->numOfAttrs = s[510];
	nextApply = char2int(s, 505, 509);
	if (t != NULL){
		t->tableName = name;
		t->title.clear();
		t->data.clear();
		int i = 0, length = (int)s[510];
		for (; i<length * 20; i += 20){
			t->title.push_back(ColumnTitle(char2string(s, i, i + 18), (DataType)char2int(s, i + 19, i + 19)));
		}
	}
	if (nextApply == 0){ // the tree is empty, no data here, init
		//pass;  can do nothing before the first insert
	}
}




Target BPlusTree::search(Page page, Key& key){
	char s[512];
	disk.readBlock(page, s);
	NodeType type = (NodeType)s[511];
	switch (type){
	case ROOT:
	case INTERIOR:
	{
		if (type == ROOT && char2int(s, 505, 509, 128) == 0) //tree is empty, return ROOT
			return make_pair(root, ROOT); 
		InteriorNode& node = InteriorNode(s);
		InteriorNode* p = (InteriorNode*)&node;
		if (cmp(key, p->keys[0])<0)
			return search(p->pointers[0], key);
		else if (cmp(key, p->keys[p->keys.size() - 1])>0)
			return search(p->pointers[p->keys.size()], key);
		else
		{
			for (size_t i = 0; i<p->keys.size() - 1; i++) if (cmp(key, p->keys[i]) >= 0 && cmp(key, p->keys[i + 1])<0)
				return search(p->pointers[i + 1], key);
		}
	}
	break;

	case LEAF:
	{
		return make_pair(page, type);
	}
	break;

	default:
		break;
	}
	return make_pair(0, NUL);
}

/*
Node* createNode(char s[512]){
	NodeType type = NodeType((int)s[511]);
	switch (type){
	case ROOT: //root
		return new InteriorNode(s, ROOT);
		break;

	case INTERIOR: //interior
		return new InteriorNode(s);
		break;

	case LEAF:
		return new LeafNode(s,s[509]);
		break;

	default:
		return new Node();
		break;
	}
}
*/

//typedef vector<string> row;
vector<row> BPlusTree::getAll(){
	vector<row> ret; ret.clear();
	char s[512];
	disk.readBlock(root,s);
	NodeType type = (NodeType)s[511];
	if (s[510] == 0) return ret; //tree is empty
	//Node node = createNode(s);
	while (type != LEAF){
		InteriorNode& node = InteriorNode(s);
		InteriorNode* p = (InteriorNode*)&node;
		disk.readBlock(p->pointers[0],s);
		type = (NodeType)s[511];
	}
	LeafNode& node = LeafNode(s, this->numOfAttrs);
	LeafNode* p = (LeafNode*)&node;
	vector<Value>::iterator ite;
	while (true){
		for (ite = p->values.begin(); ite != p->values.end(); ite++)
			ret.push_back(*ite);
		if (p->next_page == NUL) break;
		disk.readBlock(p->next_page, s);
		node = LeafNode(s, this->numOfAttrs);
		p = (LeafNode*)&node;
	}
	return ret;
}

vector<row> BPlusTree::getLessThan(Key& key){
	Target t = search(root, key);
	vector<row> ret; ret.clear();
	char s[512];
	disk.readBlock(root, s);
	if (s[510] == 0) return ret; //tree is empty
	NodeType type = (NodeType)s[511];
	while (type != LEAF){
		InteriorNode& node = InteriorNode(s);
		InteriorNode* p = (InteriorNode*)&node;
		disk.readBlock(p->pointers[0], s);
		type = (NodeType)s[511];
	}
	LeafNode& node = LeafNode(s, this->numOfAttrs);
	LeafNode* p = (LeafNode*)&node;
	vector<Value>::iterator ite = p->values.begin();
	vector<Key>::iterator itek = p->keys.begin();
	while (true){
		for (ite = p->values.begin(), itek = p->keys.begin(); ite != p->values.end(); ite++, itek++){
			if (cmp(key, *itek) >= 0) goto fin;
			ret.push_back(*ite);
		}
		if (p->next_page == NUL) break;
		disk.readBlock(p->next_page, s);
		node = LeafNode(s, this->numOfAttrs);
		p = (LeafNode*)&node;
	}
fin:
	return ret;

}

vector<row> BPlusTree::getBiggerThan(Key& key){
	vector<row> ret; ret.clear();
	Target t = search(root, key);
	if (t.first == root) return ret; // three is empty, return 
	char s[512];
	disk.readBlock(t.first, s);
	NodeType type = (NodeType)s[511];
	LeafNode& node = LeafNode(s, this->numOfAttrs);
	LeafNode* p = (LeafNode*)&node;
	vector<Value>::iterator ite = p->values.begin();
	vector<Key>::iterator itek = p->keys.begin();
	while (cmp(key, *itek) < 0) ite++, itek++;

	while (true){
		for (; ite != p->values.end(); ite++)
			ret.push_back(*ite);
		if (p->next_page == NUL) break;
		disk.readBlock(p->next_page, s);
		node = LeafNode(s, this->numOfAttrs);
		p = (LeafNode*)&node;
	}
	return ret;
}

vector<row> BPlusTree::getRange(Key& left, Key &right){
	vector<row> ret; ret.clear();
	Target t = search(root, left);
	if (t.first == root) return ret; // three is empty, return
	char s[512];
	disk.readBlock(t.first, s);
	NodeType type = (NodeType)s[511];
	LeafNode& node = LeafNode(s, this->numOfAttrs);
	LeafNode* p = (LeafNode*)&node;
	vector<Value>::iterator ite = p->values.begin();
	vector<Key>::iterator itek = p->keys.begin();
	while (cmp(left, *itek) < 0) ite++, itek++;

	while (true){
		for (; ite != p->values.end(); ite++, itek++){
			if (cmp(right, *itek) <= 0) goto fin;
			ret.push_back(*ite);
		}
		if (p->next_page == NUL) break;
		disk.readBlock(p->next_page, s);
		node = LeafNode(s, this->numOfAttrs);
		p = (LeafNode*)&node;
		ite = p->values.begin(), itek = p->keys.begin();
	}
fin:
	return ret;
}


void BPlusTree::WriteHead2Disk(){
	char s[512];
	memset(s, 0, sizeof s);
	this->disk.readBlock(0, s);
	int2char(s, 505, 509, this->nextApply, 128);
	this->disk.writeBlock(0, s);
}

void BPlusTree::initTree(vector<ColumnTitle> &title, ColumnTitle primaryKey){
	//init page 0
	char s[512];
	memset(s, 0, sizeof s);
	s[511] = (int)NUL;
	s[510] = (char)title.size();
	int2char(s, 505, 509, 0, 128);
	string2chars(s, 400, 418, primaryKey.column_name);
	s[419] = primaryKey.datatype;
	for (size_t i = 0; i < title.size(); i++){
		string2chars(s, i * 20, i * 20 + 18, title[i].column_name);
		s[i * 20 + 19] = (char)title[i].datatype;
	}
	this->disk.writeBlock(0, s);
	//finish init page 0

	//init page 1(root)
	memset(s, 0, sizeof s);
	s[511] = (int)ROOT;
	InteriorNode in(s,ROOT);
	in.Write2Disk(root,this->disk);
	//finish init page 1(root)

}


void LeafNode::Write2Disk(Page page, Disk &disk){
	char s[512];
	memset(s, 0, sizeof s);
	s[511] = (char)this->type;
	s[510] = (char)this->keys.size();
	s[509] = (char)this->numOfAttrs;
	int2char(s, 504, 508, this->next_page);
	int2char(s, 499, 503, this->prev_page);
	int2char(s, 494, 498, this->father);
	for (size_t i = 0; i < this->keys.size(); i++){
		string2chars(s, i * 160, i * 160 + 19, this->keys[i]);
		for (size_t j = 0; j < this->numOfAttrs; j++){
			string2chars(s, i * 160 + 20 + j * 20, i * 160 + 39 + j * 20,this->values[i][j]);
		}
	}
	disk.writeBlock(page, s);
}

void InteriorNode::Write2Disk(Page page, Disk &disk){
	char s[512];
	memset(s, 0, sizeof s);
	s[511] = (char)this->type;
	s[510] = (char)this->keys.size();
	int2char(s, 505, 509, this->father);
	int i = 0,j=0, length = this->keys.size();
	for (; i < length * 25; i += 25,j++){
		int2char(s, i, i + 4, this->pointers[j]);
		string2chars(s, i + 5, i + 24, this->keys[j]);
	}
	int2char(s, i, i + 4, this->pointers[j]);
	disk.writeBlock(page, s);
}
