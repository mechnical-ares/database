#pragma once
struct TableInfo{
	vector<ColumnTitle> Columns;
	string Tablename;
	ColumnTitle primaryKey;
	TableInfo(){}
	TableInfo(vector<ColumnTitle> Columns, string Tablename) :Columns(Columns), Tablename(Tablename){}
};
class TableManagement
{
public:
	TableManagement();
	~TableManagement();

	vector<TableInfo> TableInfomation;
	int TableNum = 0;//记录当前表的总数，用于非重复地初始化临时表的名字
	int hasTable(string name);
	int hasColumn(string TableName, string ColumnName);
	int hasColumn(string ColumnName);
	vector<TableInfo> getTablebyColumn(string Columnname);
	vector<string> getColumnbyTable(string TableName);
	ColumnTitle getPrimaryKey(string TableName);
	int addTable(TableInfo);
	DataType getDataType(string TableName, string ColumnName);
	int writeToDisk();//操作完成/运行结束时写入磁盘保存
	int initialFromDisk();//对象初始化时从磁盘读取表格管理信息
};