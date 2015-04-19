#include "stdafx.h"
#include "Table.h"



Table::Table(string tableName) :tableName(tableName){
	
}

bool isInt(string s){
	while (s.front() == ' ')
		s.erase(s.begin());
	while (s.back() == ' ')
		s.pop_back();

	if (s == "")
		return false;
	int i = 0;
	if (s[0] == '-'){
		i++;
	}
	else if (!isdigit(s[0])){
		return false;
	}
	for (; i < s.length(); i++){
		if (!isdigit(s[0]))
			return false;
	}
	return true;
}
bool isDouble(string s){
	while (s.front() == ' ')
		s.erase(s.begin());
	while (s.back() == ' ')
		s.pop_back();

	if (s == "")
		return false;
	int i = 0;
	if (s[0] == '-'){
		i++;
	}
	else if (!isdigit(s[0])){
		return false;
	}
	bool hasDot = false;
	for (; i < s.length(); i++){
		if (s[i] == '.' ){
			hasDot = true;
		}
		else{
			return false;
		}
		if (!isdigit(s[0]))
			return false;
	}
	return true;
}
Data::Data(const string& s){
	data = s;
	if (isInt(s)){
		type = INT;
	}
	else if (isDouble(s)){
		type = DOUBLE;
	}
	else if (s == "true" || s == "false"){
		type = BOOL;
	}
	else{
		type = STRING;
	}
}