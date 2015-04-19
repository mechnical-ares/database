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
	Data(const string& s);
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
