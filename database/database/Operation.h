#include "stdafx.h"

using namespace std;
struct TableColumn{
	string tableName;
	string colunmName;
};

//enum Operator{
//	EQ, //==
//	NE, //!=
//	GT, //>
//	LT, //<
//	GE, //>=
//	LE, //<=
//};


typedef bool(*Operator)(const Data&, const Data& );
bool isEQ(const Data& left, const Data& right){
	if (left.type != right.type)
		return false;
	return left.data == right.data;
}
struct Condition{
	Operator op;
	TableColumn left, right;
};

class Operation{
public:
	virtual Table exec() = 0;
};