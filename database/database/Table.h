#pragma once
#include "Operation.h"

using namespace std;
enum DataType{
	ACONST,
	AINT, 
	ADOUBLE, 
	ABOOL, 
	ASTRING,
};
struct ColumnTitle{
	string column_name; 
	DataType datatype;
	ColumnTitle(string cn="", DataType d=ASTRING){
		column_name = cn, datatype = d;
	}
};

struct Data{
	DataType type;
	string data;
	Data(DataType type, string data) :type(type), data(data){}
};

typedef vector<string> row;
class Table{
public:
	Table(){}
	Table(string tableName);
	string tableName;
	vector<ColumnTitle> title;
	ColumnTitle primaryKey;
	vector<row> data;
};
