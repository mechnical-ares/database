#pragma once
#include "Operation.h"
#include "Table.h"

class QueryOperation :public Operation
{
public:
	QueryOperation(vector<TableColumn> columns, vector<string> tableNames, vector<Condition> conditions);
	virtual Table exec();
	vector<TableColumn> columns;
	vector<string> tableNames;
	vector<Condition> conditions;
};

