#include "Operation.h"


using namespace std;
enum DataType{
	CONEST,
	INT, 
	DOUBLE, 
	BOOL, 
	STRING,
};
struct ColumnTitle{
	string column_name; 
	DataType datatype;
	ColumnTitle(string cn="", DataType d=STRING){
		column_name = cn, datatype = d;
	}
};

struct Data{
	DataType type;
	string data;
	Data(DataType type, string data) :type(type), data(data){}
};

typedef vector<string> row;
class Table{
public:
	Table(){}
	Table(string tableName);
	string tableName;
	vector<ColumnTitle> title;
	ColumnTitle primaryKey;
	vector<row> data;
};
