// database.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include<string>
#include<iostream>
#include<vector>
#include<sstream>
#include<vector>
//#include"CreateOperation.h"
#include"QueryOperation.h"

using namespace std;

//字符串分割函数
vector<string> split(string str, string pattern)
{
	std::string::size_type pos;
	std::vector<std::string> result;
	str += pattern;//扩展字符串以方便操作
	int size = str.size();

	for (int i = 0; i<size; i++)
	{
		pos = str.find(pattern, i);
		if (pos<size)
		{
			std::string s = str.substr(i, pos - i);
			if (pos - i != 0)
				result.push_back(s);
			i = pos + pattern.size() - 1;
		}
	}
	return result;
}

vector<string> split(string str, string pattern, string pattern2)//去掉括号函数creat时使用
{
	std::string::size_type pos;
	std::string::size_type minpos;
	std::vector<std::string> result;
	str += " ";//扩展字符串以方便操作
	int size = str.size();

	for (int i = 0; i<size; i++)
	{
		minpos = str.find(pattern, i);
		pos = str.find(pattern2, i);
		pos = (minpos < pos ? minpos : pos);
		if (pos<size)
		{
			std::string s = str.substr(i, pos - i);
			if (pos - i != 0)
				result.push_back(s);
			i = pos + pattern.size() - 1;
		}
	}
	return result;
}


void parser(string t)//解析函数
{
	string type = t.substr(0, 6);
	string left = t.substr(6, t.size() - 6);
	if (type == "SELECT" || type == "select")
	{

	}
	else{
		if (type == "CREATE" || type == "create")
		{

		}
	}
}


int main()
{
	parser("select ");
	return 0;
}




bool issubQuery(string str)
{
	if (str.at(0) == '('&&str.at(str.length() - 1) == ')')
		return true;
	else
		return false;
}
bool iscolandtab(string str)
{
	if (str.find(".") != str.npos)
		return true;
	else
		return false;
}
RecordSet findtables(string tablename);

Operation *parser(string t)
{
	if (issubQuery(t)){//如有括号包裹，去掉括号
		t = t.substr(1, t.length() - 2);
	}
	string type;//操作类型
	Operation *op;
	int count = 0;
	vector<string> allwords = split(t, " ");//按照空格先分词
	type = allwords[count];
	count++;
	if (type == "SELECT" || type == "select")//判断类型
	{
		vector<string> columns = split(allwords.at(count), ",");
		count++;

		if (allwords.at(count) != "from"&&allwords.at(count) != "FROM"){
			cerr << "Illegal Input";
			abort();
		}
		count++;
		vector<string> tabledef = split(allwords.at(count), ",");
		vector<RecordSet> tables;
		for (int i = 0; i<tabledef.size(); i++)//把表准备好
		{
			if (issubQuery(tabledef.at(i)))
			{
				Operation *subquery = parser(tabledef.at(i));
				tables.push_back(subquery.exec());
				continue;
			}
			tables.push_back(findtables(tabledef.at(i)));
		}
		count++;
		vector<RecordSet>::iterator it = tables.begin(), end = tables.end();
		vector<ColumnAndTable> colandtab;
		for (int i = 0; i<columns.size(); i++)//列和表匹配
		{
			if (iscolandtab(columns[i]))//指明了表和列
			{
				vector<string> thisline = split(columns[i], ".");
				ColumnAndTable temp;
				temp.first = line[0];
				temp.second = line[1];
				colandtab.push_back(temp);
				continue;
			}
			for (it = tables.begin(); it != end; it++){//未指明表，全部匹配
				ColumnAndTable temp;
				temp.first = columns[i];
				temp.second = "";//(*it).//此处有问题，如果是递归生成的表，怎么表示？
				colandtab.push_back(temp);
			}
		}
		if (allwords.at(count) != "where"&&allwords.at(count) != "WHERE"){
			cerr << "Illegal Input";
			abort();
		}
		count++;
		vector<string> equations = split(allwords.at(count), ",");
		op = new QueryOperation(tables, colandtab, equations);
	}
	else{
		//分词时注意类型表名后面必须接 空格；主键不能接空格，必须直接接括号
		if (type == "CREATE" || type == "create")//creat table by TZH
		{
			string tableName = "";//
			vector<string> allwords = split(t, " (", ") ");
			/*for (int i = 0; i < allwords.size(); i++)
			cout << allwords.at(i) << endl;*/
			vector<string> firstPart = split(allwords.at(0), " ");
			tableName = firstPart.at(2);//third word is the table name
			cout << tableName << endl;
			vector<string> columnAndType = split(allwords.at(1), ",");
			vector<column> cAndTs;
			for (int i = 0; i < columnAndType.size() - 1; i++)
			{
				new columnsAndType;
				columnsAndType.
					cout << columnAndType.at(i) << endl;
			}
			string primaryKey = split(columnAndType.at(columnAndType.size() - 1), "(", ")").at(1);
			cout << "primary key:" << primaryKey;
		}
	}
	return op;
}
int main()
{
	parser("select ab,c,d from e,f where ab>1");
	return 0;
}




