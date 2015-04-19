#include "stdafx.h"
#include "QueryOperation.h"
using namespace std;
namespace query{
	typedef vector<row> Records;
	vector<string> split(const string& s, const char delimiter){
		size_t start = 0,end =0;
		vector<string> ans;
		string token;
		while ((end = s.find(start,delimiter) != s.npos)){
				token = s.substr(start,end);
				ans.push_back(token);
				start = end + 1;
		}
		return ans;
	}

	Table join(const Table& p, const Table& q){
		Table result;
		for (const auto& r1 : p.data){
			for (const auto& r2 : q.data){
				row tmp = r1;
				tmp.insert(tmp.end(),r2.begin(), r2.end());
				result.data.push_back(tmp); 
			}
		}
		result.title = p.title;
		result.title.insert(result.title.end(), q.title.begin(), q.title.end());
		return result;
	}

	int getPos(const vector<Table>& tables, const vector<string>& tableNames, const TableColumn& column){
		int result = 0;
		for (int i = 0; i < tables.size(); i++){
			if (tableNames[i] != column.colunmName){
				result += tables[i].title.size();
			}else{
				for (int j = 0; j < tables[i].title.size(); j++){
					if (tables[i].title[j].column_name == column.colunmName){
						break;
					}
					++result;
				}
			}
		}
		return result;
	}

	Table filter(const Table& table, const vector<Table>& tables, const vector<string>& tableNames, const vector<Condition>& conditions){
		Table result = table;
		result.data.clear();
		for (auto& record : table.data){
			bool ok = true;
			for (const auto& condition : conditions){
				int posl = getPos(tables, tableNames, condition.left);
				int posr = getPos(tables, tableNames, condition.right);
				Data datal(record[posl]);
				Data datar(record[posr]);
				if (!condition.op(datal, datar)){
					ok = false;
					break;
				}
			}
			if (ok){
				result.data.push_back(record);
			}
		}
		return result;
	}

	Table filter(const Table& table, const vector<Table>& tables, const vector<string>& tableNames, const vector<TableColumn>& columns){
		Table result;
		vector<int> columsID;
		int sum = 0;
		for (int i = 0; i < tableNames.size(); i++){
			for (int j = 0; j < tables[i].title.size(); j++){
				TableColumn current{ tableNames[i], tables[i].title[j].column_name };
				if (find(columns.begin(), columns.end(), current) != columns.end()){
					columsID.push_back(sum);
					result.title.push_back(tables[i].title[j]);
				}
				++sum;
			}
		}

		for (const auto& rec: table.data){
			row tmp;
			for (int id : columsID){
				tmp.push_back(rec[id]);
			}
			result.data.push_back(tmp);
		}
		return result;
	}

}
using namespace query;
QueryOperation::QueryOperation(vector<TableColumn> columns, vector<string> tableNames, vector<Condition> conditions)
	:columns(columns), tableNames(tableNames), conditions(conditions)
{
}

Table QueryOperation::exec(){
	
	vector<Table> tables;
	for (const auto& name : tableNames){
		tables.push_back(Table(name));
	}
	Table result = tables[0];
	for (int i = 1; i < tables.size(); i++){
		result = join(result, tables[i]);
	}
	result = filter(result, tables, tableNames, conditions);
	result = filter(result, tables, tableNames, columns);
	
	result.tableName = "result";
	return result;
}