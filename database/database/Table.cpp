#include "stdafx.h"
#include "Table.h"



Table::Table(string tableName) :tableName(tableName){
	
}

Data::Data(const string& s){
	data = s;
	if (isInt(s)){
		type = INT;
	}
	else if (isDouble(s)){
		type = DOUBLE;
	}
	else if (s == "true" || s == "false"){
		type = BOOL;
	}
	else{
		type = STRING;
	}
}