#pragma once
#include "Operation.h"
class CreateOperation :
	public Operation
{
public:
	CreateOperation(string ,vector<string>);
private:
	string tableName;
	vector<string> columns;
};

