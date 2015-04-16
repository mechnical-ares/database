#pragma once
#include "Operation.h"

class QueryOperation :public Operation
{
public:
	QueryOperation(vector<string> tables, vector<ColumnAndTable> columns, vector<Equation> equations);
	virtual RecordSet exec() = 0;
private:
	vector<string> tables; //要查询的所有表名
	vector<ColumnAndTable> columns; //查询的列名和所在表名
	vector<Equation> equations;
};

