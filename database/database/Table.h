#include <string>
#include <vector>
//#include "Operation.h"
using namespace std;

struct columnsAndType{
	string column; //表头
	string type; //数据类型
};


class Table{
public:
	Table(string, vector<columnsAndType>, int, int);
private:
	string tableName;
	vector<columnsAndType> columns;
	int rowNum;
	int colNum;
	
};
