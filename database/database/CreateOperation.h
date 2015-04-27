#pragma once

#ifndef CREATEOPERATION
#define CREATEOPERATION

#include <iostream>
#include <fstream>
#include "Operation.h"
#include "Table.h"

class CreateOperation : public Operation
{
public:
	string tableName;
	vector<ColumnTitle> columns;
	CreateOperation(string tableName, vector<ColumnTitle> columns, ColumnTitle primaryKey);
	virtual Table exec();
};

#endif