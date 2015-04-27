#pragma once
#include "Data.h"
#include "TableColumn.h"
typedef bool(*Operator)(const Data&, const Data&);

bool isEQ(const Data& left, const Data& right);
bool isNE(const Data& left, const Data& right);
bool isGT(const Data& left, const Data& right);
bool isLT(const Data& left, const Data& right);
bool isGE(const Data& left, const Data& right);
bool isLE(const Data& left, const Data& right);

struct Condition{
	Operator op;
	TableColumn left, right;
	bool isLeftConst, isRightConst;
	Data leftData, rightData;
};
