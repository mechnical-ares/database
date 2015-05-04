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
typedef function<int(const string&, const string&)> KeyFunc;
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

int char2int(char s[], int l, int r, int base = 10);
void int2char(char s[], int l, int r, int value, int base = 128);
void string2chars(char s[], int l, int r, string &ss);
string char2string(char s[], int l, int r);
KeyFunc buildKeyfunc(DataType type);



class Node
{
public:
	NodeType type;
	Page father;
	Node(NodeType _type = LEAF);
	~Node();

	void Write2Disk(Disk &disk);
};


/*
p.keys.size() == p.values.size() for all nodes p.
∀p∈LeafNode,∀k∈p.keys,∀k′∈p.next_page.keys,k<k′


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
	LeafNode(char s[], int numOfAttrs);
	~LeafNode();
	vector<Key> keys;
	vector<Value> values;
	Page next_page;
	Page prev_page;
	int numOfAttrs;
	void Write2Disk(int page, Disk &disk);
};



/*
p.keys.size() +1 == p.pointers.size()

for all key k and the adjacent pointers, after(k) and after(k),
k>max(keys(before(k)))
k≤min(keys(after(k)))

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
	
	void Write2Disk(Page page, Disk &disk);
};




/*
Definition: Neighbouring pointers

Give a key ki, we define before(ki) to be the page pointer before ki,
and after(ki) the page pointer after ki.

Namely,

p.before(ki) = p.pointers[i]
p.after(ki) = p.pointers[i+1]
*/


//Node* createNode(char s[512]);




class BPlusTree
{
private:
	Disk disk;
	Page nextApply;
	KeyFunc cmp;
	int numOfAttrs;
public:
	BPlusTree(string name, Table* t = NULL);
	~BPlusTree();
	void initTree(vector<ColumnTitle> &title, ColumnTitle primaryKey);
	Target search(Page node, Key& key);

	void insert_into_tree(Key& newkey, Value& val);
	InCons insert_into_leaf(Page page, Key& newkey, Value& val);
	InCons insert_into_interior(Page page, Key& newkey, Page child, bool left=false);

	vector<row> getAll();
	vector<row> getLessThan(Key& key); // included
	vector<row> getBiggerThan(Key& key); //included
	vector<row> getRange(Key& left,Key &right );

	void WriteHead2Disk();
};

