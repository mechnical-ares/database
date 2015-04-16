#include <string>
#include <vector>
#include <map>
using std::string;
using std::vector;
using std::pair;
typedef pair<string, string> ColumnAndTable; //列和所在的表
typedef pair<ColumnAndTable, ColumnAndTable> Equation;// 两个列的相等关系
typedef vector<string> record;
struct RecordSet{
	record titles; //记录的表头
	vector<record> data; //数据
};
class Operation{
public:
	virtual RecordSet exec() = 0;
};