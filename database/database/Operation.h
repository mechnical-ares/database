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

bool isEQ(const Data& left, const Data& right);
bool isNE(const Data& left, const Data& right);
bool isGT(const Data& left, const Data& right);
bool isLT(const Data& left, const Data& right);
bool isGE(const Data& left, const Data& right);
bool isLE(const Data& left, const Data& right);
struct Condition{
	Operator op;
	TableColumn left, right;
};

class Operation{
public:
	virtual Table exec() = 0;
};


#endif
