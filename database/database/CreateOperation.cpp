#include "stdafx.h"

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
CreateOperation::CreateOperation(string tableName, vector<ColumnTitle> columns, ColumnTitle primaryKey = {})
	:tableName(tableName), columns(columns), primaryKey(primaryKey)
{
	
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
static char Char2Inner(char c1, char c2, int &i){
	if (c1 == '\\'){
		i++;
		switch (c2){
		case 'n': return '\n';
		case 't': return '\t';
		case ',': return ',';
		case '\\': return '\\';
		case '0': return '\0';
		default: return c2;
		}
	}
	else{
		return c1;
	}
}
static string Text2Inner(const char s[], int len = 0){
	if (!len) len = strlen(s);
	string ret = "";
	for (int i = 0; i < len; i++){
		ret.push_back(Char2Inner(s[i], s[i + 1], i));
	}
	return ret;
}
static string Text2Inner(string &s){
	return Text2Inner(s.c_str());
}

static string cleanStr(string s){
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
	vector<Condition> emptyCondition;
	Table temTable(tableName, emptyCondition);
	temTable.primaryKey = primaryKey;
	temTable.title = columns;

	BPlusTree myBPtree(tableName,&temTable);
	myBPtree.initTree(columns, primaryKey);

	/*string path = string(".\\Data\\") + tableName;
	ifstream test(path.c_str());
	if (test){
		string info = "The table is already in your computer!\n";
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
			fout << Inner2Text(columns[i].column_name) << ((i + 1 == (int)columns.size())?"\n":",");
		for (size_t i = 0; i<columns.size(); i++)
			fout << columns[i].datatype << ((i + 1 == (int)columns.size()) ? "\n" : ",");
		fout.close();
	}
	catch (exception e)
	{
		string info = "Create table failed!\n";
		throw info;
	}*/

	return temTable;
}




