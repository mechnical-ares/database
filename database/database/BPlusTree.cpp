#include "stdafx.h"
#include "BPlusTree.h"
#include "Disk.h"
Node::Node(NodeType _type) :type(_type){}


InteriorNode::InteriorNode() : Node(INTERIOR){}
InteriorNode::InteriorNode(char s[], NodeType _type) : Node(_type){
	keys.clear(); pointers.clear();
	int i = 0, length = (int)s[510];
	for (; i<length * 20; i += 20){
		keys.push_back(char2string(s, i, i + 10));
		pointers.push_back(char2int(s, i + 10, i + 20));
	}
	pointers.push_back(char2int(s, i, i + 10));
}

LeafNode::LeafNode() :Node(LEAF){ type = LEAF; }
LeafNode::LeafNode(char s[512]) : Node(LEAF){
	keys.clear(); values.clear();
	int i = 0, j = 0, num = (int)s[510], length = (int)s[509];
	for (; i<num; i++){
		keys.push_back(char2string(s, i * 100, i * 100 + 20));
		row r; r.clear();
		for (j = 0; j<length; j++){
			r.push_back(char2string(s, i * 100 + 20 + j * 10, i * 100 + 30 + j * 10));
		}
		values.push_back(r);
	}
	next_page = char2int(s, 500, 509);
}


Target BPlusTree::insert_into_tree(PageNum page, Key& newkey, Value& val, KeyFunc cmp){
	
	Target leaf = search(root, newkey, cmp);
	insert_into_node(leaf, newkey, val, cmp);

}


InCons BPlusTree::insert_into_node(Target target, Key& newkey, Value& val, KeyFunc cmp)
{
	char s[512];
	disk.readBlock(target.first, s);
	Node node = createNode(s);
	int i,j,size = (int)s[510];


	switch (node.type){
	case ROOT:
	{

	}
	break;

	case INTERIOR:
	{

	}
	break;

	case LEAF:
	{
		LeafNode* p = (LeafNode*)(&node);
		vector<Key>::iterator itek=p->keys.begin();
		vector<Value>::iterator itev=p->values.begin();
		if (size < LeafCap){
			if (cmp(newkey, p->keys[0]) < 0)
				p->keys.insert(p->keys.begin(), newkey), p->values.insert(p->values.begin(), val);
			else if (cmp(newkey, p->keys[size-1])>=0)
				p->keys.insert(p->keys.end(), newkey), p->values.insert(p->values.end(), val);
			else{
				itek++; itev++;
				for (; itek < p->keys.end(); itek++, itev++)
					if (cmp(newkey, *(itek-1)) >= 0 && cmp(newkey, *itek) < 0)
						p->keys.insert(itek, newkey), p->values.insert(itev, val);
			}
			return make_pair(0, 0);
		}
		else{
			
		}
	}
	break;

	default:
		break;
	}
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




Target BPlusTree::search(PageNum page, Key& key, KeyFunc cmp){
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
