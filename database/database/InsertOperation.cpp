#include "stdafx.h"
#include "InsertOperation.h"


InsertOperation::InsertOperation(string tableName, vector<ColumnTitle> columns, vector<Data> datas,Data primaryData)
	:tableName(tableName), title(columns), data(datas), primaryData(primaryData)
{
	//todo Ñ­»·Ö§³Ö
}

InsertOperation::InsertOperation(string tableName, vector<Data> datas,Data primaryData)
	: tableName(tableName), data(datas), primaryData(primaryData)
{

}

Table InsertOperation::exec(){
	BPlusTree temBptree(tableName, NULL);

	row rowValue;
	for (int i = 0; i < data.size(); i++)//change data into a row
	{
		rowValue.push_back(data.at(i).data);
	}

	temBptree.insert_into_tree(primaryData.data, rowValue);
	return Table();
}