#include "stdafx.h"
char Char2Inner(char c1, char c2, int &i){
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
string Text2Inner(const char s[], int len = 0){
	if (!len) len = strlen(s);
	string ret = "";
	for (int i = 0; i < len; i++){
		ret.push_back(Char2Inner(s[i], s[i + 1], i));
	}
	return ret;
}
string Text2Inner(string &s){
	return Text2Inner(s.c_str());
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
TableManagement::TableManagement()
{
	initialFromDisk();
}


TableManagement::~TableManagement()
{
	writeToDisk();
}
int TableManagement::hasTable(string name){
	for (size_t i = 0; i < TableInfomation.size(); i++)
	{
		if (TableInfomation.at(i).Tablename == name)
			return true;
	}
	return false;
}
int TableManagement::hasColumn(string TableName, string ColumnName){
	for (size_t i = 0; i < TableInfomation.size(); i++)
	{
		if (TableInfomation.at(i).Tablename == TableName)
		{
			for (size_t j = 0; j < TableInfomation.at(i).Columns.size(); j++)
			{
				if (TableInfomation.at(i).Columns.at(j).column_name == ColumnName)
					return true;
			}
		}
	}
	return false;
}
int TableManagement::addTable(TableInfo t){
	if (hasTable(t.Tablename))
	{
		cerr << "Table already has been existed\n";
		return false;
	}
	else{
		TableInfomation.push_back(t);
		TableNum++;
		return true;
	}
}
vector<TableInfo> TableManagement::getTablebyColumn(string ColumnName){
	vector<TableInfo> Tables;
	for (size_t i = 0; i < TableInfomation.size(); i++)
	{

		for (size_t j = 0; j < TableInfomation.at(i).Columns.size(); j++)
		{
			if (TableInfomation.at(i).Columns.at(j).column_name == ColumnName)
				Tables.push_back(TableInfomation.at(i));
		}
	}
	return Tables;
}
vector<string> TableManagement::getColumnbyTable(string TableName){
	vector<string> Column;
	for (size_t i = 0; i < TableInfomation.size(); i++)
	{
		if (TableInfomation.at(i).Tablename == TableName)
		{
			for (size_t j = 0; j < TableInfomation.at(i).Columns.size(); j++)
				Column.push_back(TableInfomation.at(i).Columns.at(j).column_name);
			return Column;
		}
	}
	throw "Table"+TableName+" Not Found";
}
DataType TableManagement::getDataType(string TableName, string ColumnName){
	DataType t;
	for (size_t i = 0; i < TableInfomation.size(); i++)
	{
		if (TableInfomation.at(i).Tablename == TableName)
		{
			for (size_t j = 0; j < TableInfomation.at(i).Columns.size(); j++)
				if (TableInfomation.at(i).Columns.at(j).column_name == ColumnName){
					t = TableInfomation.at(i).Columns.at(j).datatype;
					return t;
				}
		}
	}
	throw "Table"+TableName+" And "+ColumnName +" Not Found";
}
ColumnTitle TableManagement::getPrimaryKey(string TableName){
	ColumnTitle t;
	for (size_t i = 0; i < TableInfomation.size(); i++)
	{
		if (TableInfomation.at(i).Tablename == TableName)
		{
			t = TableInfomation.at(i).primaryKey;
			return t;
		}
	}
	throw "Table "+ TableName +" Not Found";
}
int TableManagement::writeToDisk(){
	string path = string(".\\Table\\") + cleanStr("TableInfo");
	ofstream fout(path.c_str());
	if (!fout){
		throw "No TableInfo Files Found!\n";
		return false;
	}
	else
	{
		for (size_t i = 0; i < TableInfomation.size(); i++){
			fout << TableInfomation.at(i).Tablename<<endl;
			for (size_t j = 0; j < TableInfomation.at(i).Columns.size() - 1; j++){
				fout << TableInfomation.at(i).Columns.at(j).column_name <<",";
			}
			if (TableInfomation.at(i).Columns.size() >= 1)
			{
				fout << TableInfomation.at(i).Columns.at(TableInfomation.at(i).Columns.size()-1).column_name << endl;
			}
			for (size_t j = 0; j < TableInfomation.at(i).Columns.size() - 1; j++){
				fout << TableInfomation.at(i).Columns.at(j).datatype << ",";
			}
			if (TableInfomation.at(i).Columns.size() >= 1)
			{
				fout << TableInfomation.at(i).Columns.at(TableInfomation.at(i).Columns.size()-1).datatype << endl;
			}
			fout << TableInfomation.at(i).primaryKey.column_name << endl;
			fout << TableInfomation.at(i).primaryKey.datatype << endl;
		}
		return true;
	}
}
int TableManagement::initialFromDisk(){
	string path = string(".\\Table\\") + cleanStr("TableInfo");
	ifstream fin(path.c_str());
	if (!fin){
		string info = "No TableInfo Files Found!\n";
		throw info;
		return false;
	}
	else{
		int count = 0;
		this->TableInfomation.clear();
		
		while (!fin.eof()){
			char s[1000];
			TableInfo info;
			fin.getline(s, 1000); // Name
			info.Tablename = Text2Inner(&s[0]);
			fin.getline(s, 1000); //Attributes
			
			for (size_t i = 0, len = 0; i <= strlen(s); i++){
				if (s[i] == ',' || s[i] == '\n' || s[i] == '\0')
					info.Columns.push_back(ColumnTitle(Text2Inner(&s[i - len], len))), len = 0;
				else len++;
			}

			vector<DataType> types;
			int tmp;
			types.clear();
			fin.getline(s, 1000); //DataType
			for (size_t i = 0, j = 0, len = 0; i < info.Columns.size(); i++, j += 2){
				tmp = s[j] - '0';
				types.push_back((DataType)tmp);
				info.Columns.at(i).datatype = types[i];
			}
			fin.getline(s, 1000); //Parimary Key
			info.primaryKey.column_name = Text2Inner(&s[0]);
			fin.getline(s, 1000); //Parimary Key DataType
			tmp = s[0] - '0';
			info.primaryKey.datatype = (DataType)tmp;
			TableInfomation.push_back(info);
			count++;
		}
		this->TableNum = count;
		return true;
	}
}
