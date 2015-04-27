#pragma once

#define Key string
#define Value row
#define Page int
#define root 1

enum NodeType
{
	NUL = 0, ROOT, INTERIOR, LEAF
};

const int InteriorNodeCapacity = 20;
const int LeafNodeCapacity = 3;

typedef pair<Page, int> Locate;
typedef pair<Page, NodeType> Target;

typedef int(*KeyFunc) (string, string);
typedef int(*RowFunc) (string, string);
struct InsertConsequence
{
	Key key; Page page; Page father;
	bool left;
	InsertConsequence(Key k="", Page p=0, Page f=0,bool l=false){
		key = k, page = p, father = f, left = l;
	}
};

#define InterCap InteriorNodeCapacity
#define LeafCap LeafNodeCapacity
#define InCons InsertConsequence

int char2int(char s[], int l, int r, int base = 10){
	int ret = 0;
	switch (base){
	case 10:
		if (l > r) return 0;
		for (int i = l; i<=r; i++) if (s[i] != 0){
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

string char2string(char s[], int l, int r){
	string ret = "";
	for (int i = l; i<=r; i++) if (s[i] != 0)
		ret.push_back(s[i]);
	return ret;
}




class Node
{
public:
	NodeType type;
	Page father;
	Node(NodeType _type = LEAF);
	~Node();

	void Write2Disk();
};


/*
p.keys.size() == p.values.size() for all nodes p.
∀p∈LeafNode,∀k∈p.keys,∀k′∈p.next_page.keys,k<k′

char s[512]:
511: type
510: size  //num of nodes, force to int
509: num of attributes, (size of vector)
0~499: keys and values, (20+10*8)*5
500~508: next_page pointer


char s[512]:
511: type
510: size  //num of nodes, force to int
509: num of attributes, (size of vector)
504~508: next_page pointer, base 128
499~503: prev_page pointer, base 128
494~498: father pointer, base 128
0~479: keys and values, (20+20*7)*3
*/
class LeafNode : public Node
{
public:
	LeafNode();
	LeafNode(char s[]);
	~LeafNode();
	vector<Key> keys;
	vector<Value> values;
	Page next_page;
	Page prev_page;
	void Write2Disk(int page);
};



/*
p.keys.size() +1 == p.pointers.size()

for all key k and the adjacent pointers, after(k) and after(k),
k>max(keys(before(k)))
k≤min(keys(after(k)))

char s[512]:
511: type
510: size  //force to int
0~509: keys + pointers , 10 char per
0~9: first pointer
500~509: the final pointer (if the node is full)

char s[512]:
511: type
510: size  //force to int
505~509: father point
0~504: keys + pointers, 5 + (20 + 5)*20

*/
class InteriorNode : public Node
{
public:
	InteriorNode();
	InteriorNode(char s[], NodeType _type = INTERIOR);
	~InteriorNode();
	vector<Key> keys;
	vector<Page> pointers;
	
	void Write2Disk(Page page);
};




/*
Definition: Neighbouring pointers

Give a key ki, we define before(ki) to be the page pointer before ki,
and after(ki) the page pointer after ki.

Namely,

p.before(ki) = p.pointers[i]
p.after(ki) = p.pointers[i+1]
*/


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




class BPlusTree
{
private:
	Disk disk;
	Page nextApply;
public:
	BPlusTree(string, Table*);
	~BPlusTree();
	void initTree(vector<ColumnTitle> &title, ColumnTitle primaryKey);
	Target search(Page node, Key& key, KeyFunc cmp);
	void insert_into_tree(Page node, Key& newkey, Value& val, KeyFunc cmp);
	InCons insert_into_leaf(Page page, Key& newkey, Value& val, KeyFunc cmp);
	InCons insert_into_interior(Page page, Key& newkey, Page child, KeyFunc cmp, bool left=false);

	vector<row> getAll();
	vector<row> getLessThan(Key& key, KeyFunc cmp); // included
	vector<row> getBiggerThan(Key& key, KeyFunc cmp); //included
	vector<row> getRange(Key& left,Key &right , KeyFunc cmp);
};

