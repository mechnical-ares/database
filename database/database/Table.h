#include <string>
#include <vector>
//#include "Operation.h"
using namespace std;



class Table{
public:
	Table(string, vector<string>,int, int);
private:
	string tableName;
	vector<string> columns;
	int rowNum;
	int colNum;
	
};
