#pragma once

#define Key string
#define Value row
#define PagePointer int
#define PageNum int
#define root 1

enum NodeType
{
	NUL = 0, ROOT, INTERIOR, LEAF
};

const int InteriorNodeCapacity = 25;
const int LeafNodeCapacity = 5;

typedef pair<PageNum, int> Locate;
typedef pair<PageNum, NodeType> Target;
typedef pair<Key, PageNum> InsertConsequence;
typedef int(*KeyFunc) (string, string);
typedef int(*RowFunc) (string, string);


#define InterCap InteriorNodeCapacity
#define LeafCap LeafNodeCapacity
#define InCons InsertConsequence

int char2int(char s[], int l, int r, int base = 10){
	int ret = 0;
	switch (base){
	case 10:
		if (l >= r) return 0;
		for (int i = l; i<r; i++) if (s[i] != 0){
			ret = ret*base + (s[i] - '0');
		}
		break;

	default: break;
	}
	return ret;
}

string char2string(char s[], int l, int r){
	string ret = "";
	for (int i = l; i<r; i++) if (s[i] != 0)
		ret.push_back(s[i]);
	return ret;
}




class Node
{
public:
	NodeType type;
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
*/
class LeafNode : public Node
{
public:
	LeafNode();
	LeafNode(char s[]);
	~LeafNode();
	vector<Key> keys;
	vector<Value> values;
	PagePointer next_page;
	void Write2Disk();
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
*/
class InteriorNode : public Node
{
public:
	InteriorNode();
	InteriorNode(char s[], NodeType _type = INTERIOR);
	~InteriorNode();
	vector<Key> keys;
	vector<PagePointer> pointers;
	
	void Write2Disk();
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
	PageNum nextApply;
public:
	BPlusTree(string, Table*);
	~BPlusTree();
	void initTree(vector<ColumnTitle> &title, ColumnTitle primaryKey);
	Target search(PageNum node, Key& key, KeyFunc cmp);
	Target insert_into_tree(PageNum node, Key& newkey, Value& val, KeyFunc cmp);
	InCons insert_into_node(Target target, Key& newkey, Value& val, KeyFunc cmp);

	vector<row> getAll();
	vector<row> getLessThan(Key& key, KeyFunc cmp); // included
	vector<row> getBiggerThan(Key& key, KeyFunc cmp); //included
	vector<row> getRange(Key& left,Key &right , KeyFunc cmp);
};

