#pragma once
#include <string>
using namespace std;
struct TableColumn{
	string tableName;
	string colunmName;
	bool operator==(const TableColumn& r)const{
		return tableName == r.tableName && colunmName == r.colunmName;
	}
};
