#pragma once
#include "Operation.h"

class QueryOperation :public Operation
{
public:
	virtual Table exec();
	vector<TableColumn> columns;
	vector<string> tableNames;
	vector<Condition> conditions;

};

