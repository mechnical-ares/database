#include "stdafx.h"
#include "InsertOperation.h"


InsertOperation::InsertOperation(string tableName, vector<ColumnTitle> columns, vector<Data> datas)
	:tableName(tableName), title(columns), data(datas)
{

}

InsertOperation::InsertOperation(string tableName, vector<Data> datas)
	: tableName(tableName), data(datas)
{

}