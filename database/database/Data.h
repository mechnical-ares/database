#pragma once
#include <string>
using namespace std;
enum DataType{
	DB_CONST,
	DB_INT,
	DB_DOUBLE,
	DB_BOOL,
	DB_STRING,
};
struct Data{
	DataType type;
	string data;
	Data(){}
	Data(DataType type, string data) :type(type), data(data){}
};