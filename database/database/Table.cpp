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
		}
	}
	else{
		return c1;
	}
}
string Text2Inner(const char s[],int len = 0){
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
	for (int i = 0; i < s.length(); i++){
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



Table::Table(string tableName) :tableName(cleanStr(tableName)){
	string path = string(".\\Data\\") + cleanStr(tableName);
	ifstream fin(path.c_str());
	if (!fin){
		string info = "No table found!\n";
		throw info;
	}
	else{
		char s[1000];
		fin.getline(s, 1000); // Name

		fin.getline(s, 1000); //Attributes
		this->title.clear();
		for (int i = 0, len = 0; i <= strlen(s); i++){
			if (s[i] == ',' || s[i] == '\n' || s[i] == '\0')
				this->title.push_back(ColumnTitle(Text2Inner(&s[i - len], len))), len = 0;
			else len++;
		}

		vector<DataType> types;
		char c;
		int tmp;
		fin.getline(s, 1000); //DataType
		for (int i = 0, len = 0; i < this->title.size(); i++){
			fin >> tmp >> c;
			types.push_back((DataType)tmp);
			this->title[i].column_name = types[i];
		}

		this->data.clear();
		while (fin.getline(s, 1000)){ //Data
			row r;	r.clear();
			for (int i = 0, len = 0; i <= strlen(s); i++){
				if (s[i] == ',' || s[i] == '\n' || s[i] == '\0')
					r.push_back(Text2Inner(&s[i - len], len)),len=0;
				else len++;
			}
			this->data.push_back(r);
		}
	}
}


/*
class Table{
public:
Table(string tableName);
string tableName;
vector<ColumnTitle> title;
ColumnTitle primaryKey;
vector<row> data;
};
struct ColumnTitle{
string column_name;
DataType datatype;
};
typedef vector<string> row;
struct Data{
DataType type;
string data;
};
*/