#pragma once
#include "Operation.h"
#include "Table.h"
class CreateOperation :
	public Operation//..
{
public:
	string tableName;
	vector<ColumnTitle> columns;
};

