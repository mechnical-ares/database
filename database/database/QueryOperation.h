#pragma once



#include "Operation.h"


class QueryOperation :public Operation
{
public:
	QueryOperation(vector<TableColumn> columns, vector<string> tableNames, vector<Condition> conditions);
	virtual Table exec();
	vector<TableColumn> columns;
	vector<string> tableNames;
	vector<Condition> conditions;
};



Table join(const Table& p, const Table& q);
int getPos(const vector<Table>& tables, const vector<string>& tableNames, const TableColumn& column);
Table filter(const Table& table, const vector<Table>& tables, const vector<string>& tableNames, const vector<Condition>& conditions);
Table filter(const Table& table, const vector<Table>& tables, const vector<string>& tableNames, const vector<TableColumn>& columns);