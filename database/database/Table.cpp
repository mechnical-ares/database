#include "stdafx.h"
#include "Table.h"



Table::Table(string tableName, vector<columnsAndType> columns, int rowNum, int colNum, columnsAndType primaryKey) :tableName(tableName), columns(columns), rowNum(rowNum), colNum(colNum), primaryKey(primaryKey)
{
}
