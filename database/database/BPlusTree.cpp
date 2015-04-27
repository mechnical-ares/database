#include "stdafx.h"
#include "BPlusTree.h"
#include "Disk.h"

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
InteriorNode::InteriorNode(char s[], NodeType _type) : Node(_type){
	keys.clear(); pointers.clear();
	int i = 0, length = (int)s[510];
	father = char2int(s, 505, 509, 128);

	for (; i<length * 25; i += 25){
		keys.push_back(char2string(s, i, i + 4));
		pointers.push_back(char2int(s, i + 5, i + 24));
	}
	pointers.push_back(char2int(s, i, i + 4));
}

LeafNode::LeafNode() :Node(LEAF){ type = LEAF; }
LeafNode::LeafNode(char s[512]) : Node(LEAF){
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


void BPlusTree::insert_into_tree(Page page, Key& newkey, Value& val, KeyFunc cmp){	
	Target leaf = search(root, newkey, cmp);
	InCons con = insert_into_leaf(leaf.first, newkey, val, cmp);
	while (con.page != 0){
		con = insert_into_interior(con.father, con.key, con.page, cmp, con.left);
	}
}

InCons BPlusTree::insert_into_interior(Page page, Key& newkey, Page child, KeyFunc cmp, bool left){
	char s[512];
	disk.readBlock(page, s);
	Node node = createNode(s);
	InteriorNode *p = (InteriorNode*)&node;
	int i, j, size = (int)s[510], type = (int)s[511], father = char2int(s,505,509,128);
	
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

	if (size < InterCap)
		return InCons();
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

InCons BPlusTree::insert_into_leaf(Page page, Key& newkey, Value& val, KeyFunc cmp)
{
	char s[512];
	disk.readBlock(page, s);
	Node node = createNode(s);
	int i,j,size = (int)s[510];
	LeafNode* p = (LeafNode*)(&node);
	vector<Key>::iterator itek = p->keys.begin();
	vector<Value>::iterator itev = p->values.begin();
	size++;
	if (cmp(newkey, p->keys[0]) < 0)
		p->keys.insert(p->keys.begin(), newkey), p->values.insert(p->values.begin(), val);
	else if (cmp(newkey, p->keys[size - 1]) >= 0)
		p->keys.insert(p->keys.end(), newkey), p->values.insert(p->values.end(), val);
	else{
		itek++; itev++;
		for (; itek < p->keys.end(); itek++, itev++)
			if (cmp(newkey, *(itek - 1)) >= 0 && cmp(newkey, *itek) < 0)
				p->keys.insert(itek, newkey), p->values.insert(itev, val);
	}
	
	if (size<=LeafCap)
		return InCons("", 0, 0, 0);

	Page newpage = nextApply++;
	LeafNode newleaf;
	newleaf.keys.clear(); newleaf.values.clear();
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
500~509:the largest applied number, from 0(virtual root, *this), 1(real root), 2(first leaf)
400~409:primaryKey
0~199:keys + types (19+1)*10
*/
BPlusTree::BPlusTree(string name, Table* t = NULL) :disk(name){
	char s[512];
	disk.readBlock(0, s);
	if (t != NULL){
		t->tableName = name;
		t->title.clear();
		t->data.clear();
		int i = 0, length = (int)s[510];
		for (; i<length * 20; i += 20){
			t->title.push_back(ColumnTitle(char2string(s, i, i + 19), (DataType)char2int(s, i + 19, i + 20)));
		}
	}
	nextApply = char2int(s, 500, 510);
	if (nextApply == 0){ // the tree is empty, no data here, init
		//pass;  can do nothing before the first insert
	}
}




Target BPlusTree::search(Page page, Key& key, KeyFunc cmp){
	char s[512];
	disk.readBlock(page, s);
	Node node = createNode(s);
	switch (node.type){
	case ROOT:
	case INTERIOR:
	{
		InteriorNode* p = (InteriorNode*)&node;
		if (cmp(key, p->keys[0])<0)
			return search(p->pointers[0], key, cmp);
		else if (cmp(key, p->keys[p->keys.size() - 1])>0)
			return search(p->pointers[p->keys.size()], key, cmp);
		else
		{
			for (int i = 0; i<p->keys.size() - 1; i++) if (cmp(key, p->keys[i]) >= 0 && cmp(key, p->keys[i + 1])<0)
				return search(p->pointers[i + 1], key, cmp);
		}
	}
	break;

	case LEAF:
	{
		return make_pair(page, node.type);
	}
	break;

	default:
		break;
	}
	return make_pair(0, NUL);
}

Node createNode(char s[512]){
	NodeType type = NodeType((int)s[511]);
	switch (type){
	case ROOT: //root
		return InteriorNode(s, ROOT);
		break;

	case INTERIOR: //interior
		return InteriorNode(s);
		break;

	case LEAF:
		return LeafNode(s);
		break;

	default:
		return Node();
		break;
	}
}

//typedef vector<string> row;
vector<row> BPlusTree::getAll(){
	vector<row> ret; ret.clear();
	char s[512];
	disk.readBlock(root,s);
	Node node = createNode(s);
	while (node.type != LEAF){
		InteriorNode* p = (InteriorNode*)&node;
		disk.readBlock(p->pointers[0],s);
		node = createNode(s);
	}
	LeafNode* p = (LeafNode*)&node;
	vector<Value>::iterator ite;
	while (true){
		for (ite = p->values.begin(); ite != p->values.end(); ite++)
			ret.push_back(*ite);
		if (p->next_page == NUL) break;
		disk.readBlock(p->next_page, s);
		Node node = createNode(s);
		p = (LeafNode*)&node;
	}
	return ret;
}

vector<row> BPlusTree::getLessThan(Key& key, KeyFunc cmp){
	Target t = search(root, key, cmp);
	vector<row> ret; ret.clear();
	char s[512];
	disk.readBlock(root, s);
	Node node = createNode(s);
	while (node.type != LEAF){
		InteriorNode* p = (InteriorNode*)&node;
		disk.readBlock(p->pointers[0], s);
		node = createNode(s);
	}
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
		Node node = createNode(s);
		p = (LeafNode*)&node;
	}
fin:
	return ret;

}

vector<row> BPlusTree::getBiggerThan(Key& key, KeyFunc cmp){
	Target t = search(root, key, cmp);
	vector<row> ret; ret.clear();
	char s[512];
	disk.readBlock(t.first, s);
	Node node = createNode(s);
	LeafNode* p = (LeafNode*)&node;
	vector<Value>::iterator ite = p->values.begin();
	vector<Key>::iterator itek = p->keys.begin();
	while (cmp(key, *itek) < 0) ite++, itek++;

	while (true){
		for (; ite != p->values.end(); ite++)
			ret.push_back(*ite);
		if (p->next_page == NUL) break;
		disk.readBlock(p->next_page, s);
		Node node = createNode(s);
		p = (LeafNode*)&node;
	}
	return ret;
}

vector<row> BPlusTree::getRange(Key& left, Key &right, KeyFunc cmp){
	Target t = search(root, left, cmp);
	vector<row> ret; ret.clear();
	char s[512];
	disk.readBlock(t.first, s);
	Node node = createNode(s);
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
		Node node = createNode(s);
		p = (LeafNode*)&node;
		ite = p->values.begin(), itek = p->keys.begin();
	}
fin:
	return ret;
}


void BPlusTree::initTree(vector<ColumnTitle> &title, ColumnTitle primaryKey){
	char s[512];
	memset(s, 0, sizeof s);
	s[511] = (int)NUL;
	s[510] = (char)title.size();
	int2char(s, 505, 509, 0, 128);
	string2chars(s, 400, 419, primaryKey.column_name);
	for (int i = 0; i < title.size(); i++){
		string2chars(s, i * 20, i * 20 + 18, title[i].column_name);
		s[i * 20 + 19] = (char)title[i].datatype;
	}
}


void LeafNode::Write2Disk(Page page, Disk &disk){
	char s[512];
	memset(s, 0, sizeof s);
	s[511] = (char)this->type;
	s[510] = (char)this->keys.size();
	s[509] = (char)this->values[0].size();
	int2char(s, 504, 508, this->next_page);
	int2char(s, 499, 503, this->prev_page);
	for (int i = 0; i < this->keys.size(); i++){
		string2chars(s, i * 160, i * 160 + 19, this->keys[i]);
		for (int j = 0; j < this->values[0].size(); j++){
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
		string2chars(s, i, i + 4,this->keys[j]);
		int2char(s, i + 5, i + 24, this->pointers[j]);
	}
	int2char(s, i + 5, i + 24, this->pointers[j]);
	disk.writeBlock(page, s);
}
