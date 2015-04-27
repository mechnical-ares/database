#pragma once
#include "stdafx.h"
#include "Operation.h"

class InsertOperation :public Operation
{
public:
	InsertOperation(string, vector<ColumnTitle>, vector<Data>);
	InsertOperation(string, vector<Data>);
	virtual Table exec();
	string tableName;
	vector<Data> data;
	vector<ColumnTitle> title;
};

