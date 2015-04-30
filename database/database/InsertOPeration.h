#pragma once
#include "Operation.h"

class InsertOperation :public Operation
{
public:
	InsertOperation(string, vector<ColumnTitle>, vector<Data>, Data);
	InsertOperation(string, vector<Data>, Data);
	virtual Table exec();
	string tableName;
	vector<Data> data;
	vector<ColumnTitle> title;
	Data primaryData;
};

