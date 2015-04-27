#pragma once
#include "stdafx.h"
#include "Operation.h"

class InsertOperation :public Operation
{
public:
	InsertOperation();
	virtual Table exec();
	string tableName;
	vector<Data> data;
};

