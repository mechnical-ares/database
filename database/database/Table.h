#pragma once
#include <string>
#include <vector>
using namespace std;
enum DataType{
	DB_CONST,
	DB_INT, 
	DB_DOUBLE, 
	DB_BOOL, 
	DB_STRING,
};
struct ColumnTitle{
	string column_name; 
	DataType datatype;
	ColumnTitle(string cn="", DataType d=DB_STRING){
		column_name = cn, datatype = d;
	}
	bool operator==(const ColumnTitle& r)const{
		return column_name == r.column_name && datatype == r.datatype;
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
string cleanStr(string s);