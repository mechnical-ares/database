#pragma once

#ifndef OPERATION
#define OPERATION

#include "Table.h"
#include <string>
#include <vector>
using namespace std;
struct TableColumn{
	string tableName;
	string colunmName;
	bool operator==(const TableColumn& r)const{
		return tableName == r.tableName && colunmName == r.colunmName;
	}
};

//enum Operator{
//	EQ, //==
//	NE, //!=
//	GT, //>
//	LT, //<
//	GE, //>=
//	LE, //<=
//};


typedef bool (*Operator)(const Data&, const Data& );

bool isEQ(const Data& left, const Data& right){
	if (left.type != right.type)
		return false;
	return left.data == right.data;
}
bool isNE(const Data& left, const Data& right){
	if (left.type != right.type)
		return false;
	return left.data != right.data;
}
bool isGT(const Data& left, const Data& right){
	if (left.type != right.type)
		return false;
	return left.data > right.data;
}
bool isLT(const Data& left, const Data& right){
	if (left.type != right.type)
		return false;
	return left.data < right.data;
}
bool isGE(const Data& left, const Data& right){
	if (left.type != right.type)
		return false;
	return left.data >= right.data;
}
bool isLE(const Data& left, const Data& right){
	if (left.type != right.type)
		return false;
	return left.data <= right.data;
}
struct Condition{
	Operator op;
	TableColumn left, right;
};

class Operation{
public:
	virtual Table exec() = 0;
};


#endif