#include "stdafx.h"
#include "QueryOperation.h"
using namespace std;
namespace query{
	typedef vector<record> Records;
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

	Records join(const Records& p, const Records& q){
		Records result;
		for (const auto& r1 : p){
			for (const auto& r2 : q){
				record tmp = r1;
				tmp.insert(tmp.end(),r2.begin(), r2.end());
				result.push_back(tmp); 
			}
		}
		return result;
	}

	int getPos(const vector<string>& tables, const Records& titles, ColumnAndTable columnAndTable){
		int result = 0;
		for (int i = 0; i < tables.size(); i++){
			if (tables[i] != columnAndTable.second){
				result += titles[i].size();
			}else{
				for (int j = 0; j < titles[i].size(); j++){
					if (titles[i][j] == columnAndTable.first){
						break;
					}
					++result;
				}
			}
		}
		return result;
	}

	Records filter(const Records& records, const vector<string>& tables ,const Records& titles, const vector<Equation>& equations){
		Records result;
		for (auto& record : records){
			bool ok = true;
			for (const auto& equation : equations){
				ColumnAndTable left = equation.first;
				ColumnAndTable right = equation.second;
				int posl = getPos(tables, titles, left); 
				int posr = getPos(tables, titles, right);
				if (record[posl] != record[posr]){
					ok = false;
					break;
				}
			}
			if (ok){
				result.push_back(record);
			}
		}
		return result;
	}

	Records filter(const Records& records, const vector<string>& tables, const Records& titles, const vector<ColumnAndTable>& columns){
		Records result;
		for (const auto& rec: records){
			record tmp;
			for (const auto& column : columns){
				int pos = getPos(tables, titles, column);
				tmp.push_back(rec[pos]);
			}
			result.push_back(tmp);
		}
		return result;
	}

}
using namespace query;
QueryOperation::QueryOperation(vector<string> tables, vector<ColumnAndTable> columns, vector<Equation> equations)
	:tables(tables), columns(columns), equations(equations){
}

RecordSet QueryOperation::exec(){
	
	vector<Records> all_records;
	Records titles;
	for (auto&& table : tables){
		ifstream file(table);
		string line;
		Records records;
		getline(file, line);
		titles.push_back(split(line, ','));
		while (getline(file,line))
		{
			records.push_back(split(line, ','));
		}
	}
	Records result = all_records[0];
	for (int i = 1; i < all_records.size(); i++){
		result = join(result, all_records[i]);
	}
	result = filter(result, tables, titles, equations);
	result = filter(result, tables, titles, columns);
	
	RecordSet resultSet{columns,result};
	return resultSet;

}