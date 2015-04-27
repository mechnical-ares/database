#pragma once
#include <string>
#include "Data.h"
using namespace std;

struct ColumnTitle{
	string column_name;
	DataType datatype;
	ColumnTitle(string cn = "", DataType d = DB_STRING){
		column_name = cn, datatype = d;
	}
	bool operator==(const ColumnTitle& r)const{
		return column_name == r.column_name && datatype == r.datatype;
	}
};