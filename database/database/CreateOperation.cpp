#include "stdafx.h"
#include "CreateOperation.h"
/*
string tableName;
vector<ColumnTitle> columns;
enum DataType{
CONST,INT,DOUBLE,BOOL,STRING,
};
struct ColumnTitle{
string column_name;
DataType datatype;
};*/
CreateOperation::CreateOperation(string tableName, vector<ColumnTitle> columns, ColumnTitle primaryKey = {}) :tableName(tableName), columns(columns)
{
	//
}

string Inner2Text(const char s[]){
	string ret = "";
	for (size_t i = 0; i < strlen(s); i++){
		switch (s[i]){
		case '\n': ret.push_back('\\'); ret.push_back('n'); break;
		case '\t': ret.push_back('\\'); ret.push_back('t'); break;
		case '\\': ret.push_back('\\'); ret.push_back('\\'); break;
		case '\0': ret.push_back('\\'); ret.push_back('0'); break;
		case ',': ret.push_back('\\'); ret.push_back(','); break;
		default: ret.push_back(s[i]);
		}
	}
	return ret;
}
string Inner2Text(string &s){
	return Inner2Text(s.c_str());
}

string cleanStr(string s){
	string ret = "";
	for (size_t i = 0; i < s.length(); i++){
		switch (s[i]){
		case '\n': break;
		case '\t': break;
		case '\\': break;
		case '\0': break;
		case ':': break;
		case '*': break;
		case '?': break;
		case '<': break;
		case '>': break;
		case '|': break;
		default: ret.push_back(s[i]);
		}
	}
	return ret;
}

Table CreateOperation::exec(){
	tableName = cleanStr(tableName);
	string path = string(".\\Data\\") + tableName;
	ifstream test(path.c_str());
	if (test){
		string info = "The table is already in your computer!\n";
		cerr << info;
		throw info;
	}
	test.close();
	try
	{
		TCHAR szDirName[] = L".\\Data";
		CreateDirectory(szDirName, NULL);		
		ofstream fout(path.c_str(), fstream::out);
		fout <<Inner2Text(tableName) << endl;
		for (size_t i = 0; i<columns.size(); i++)
			fout << Inner2Text(columns[i].column_name) << (i + 1 == (int)columns.size())?"\n":",";
		for (size_t i = 0; i<columns.size(); i++)
			fout << columns[i].datatype << (i + 1 == (int)columns.size())?"\n":",";
	}
	catch (exception e)
	{
		cerr << "Create table failed!\n";
	}
	return NULL;
}




