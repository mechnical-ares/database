#include "stdafx.h"
#include "Table.h"



Table::Table(string tableName, vector<columnsAndType> columns, int rowNum, int colNum) :tableName(tableName), columns(columns), rowNum(rowNum), colNum(colNum)
{
}
