#pragma once
#include "Operation.h"
#include "Table.h"
class CreateOperation :
	public Operation
{
public:
	CreateOperation(string, vector<columnsAndType>);
private:
	string tableName;
	vector<columnsAndType> columns;
};

