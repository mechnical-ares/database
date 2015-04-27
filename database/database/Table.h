#pragma once
#include <string>
#include <vector>
#include "Condition.h"
#include "ColumnTitle.h"
using namespace std;

typedef vector<string> row;
class Table{
public:
	Table(){}
	Table(string tableName);
	Table(string tableName, const vector<Condition>& conditions);
	void showTable();
	string tableName;
	vector<ColumnTitle> title;
	ColumnTitle primaryKey;
	vector<row> data;
};
string cleanStr(string s);
