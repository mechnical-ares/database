#pragma once
#include "Table.h"
/*
enum DataType{
CONST,INT,DOUBLE,BOOL,STRING,
};
struct ColumnTitle{
	string column_name;
	DataType datatype;
};*/
class CreateOperation : public Operation//..
{
public:
	string tableName;
	vector<ColumnTitle> columns;
	CreateOperation(string tableName, vector<ColumnTitle> columns, ColumnTitle primaryKey);
	virtual Table exec();
};

