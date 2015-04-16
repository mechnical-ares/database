#include "stdafx.h"
#include "QueryOperation.h"


QueryOperation::QueryOperation(vector<string> tables, vector<ColumnAndTable> columns, vector<Equation> equations)
	:tables(tables), columns(columns), equations(equations){
}


