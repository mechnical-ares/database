#include <string>
#include <vector>
#include <memory>

using namespace std;
enum DataType{
	CONST, 
	INT, 
	DOUBLE, 
	BOOL, 
	STRING,
};
struct ColumnTitle{
	string column_name; 
	DataType datatype;
};

struct Data{
	DataType type;
	string data;
};

typedef vector<string> row;
class Table{
public:
	Table(string tableName);
	string tableName;
	vector<ColumnTitle> title;
	ColumnTitle primaryKey;
	vector<row> data;
};
