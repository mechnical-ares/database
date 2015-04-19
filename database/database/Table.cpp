#include "stdafx.h"
#include "Table.h"



Table::Table(string tableName, vector<ColumnTitle> title, ColumnTitle primaryKey) :tableName(tableName), title(title), primaryKey(primaryKey)
{
}
