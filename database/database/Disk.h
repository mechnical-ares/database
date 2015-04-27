#pragma once
#include "stdafx.h"
#include <cstdio>
enum Error{
	DB_FILE_ERROR = 1,
	META_FILE_ERROR,
	ALLOC_ERROR,
};
class Disk{
public:
	Disk(const string& name);
	int readBlock(const size_t id,  char * data);
	int writeBlock(const size_t id, char* data);
	int clearBlock(const size_t id, const char data = 0);
	static const int BLOCK_SIZE;
	~Disk();

private:
	string name;
	FILE *db_file, *meta_file;
	vector<int> blockID;
	int globalMaxId;

	int allocateBlock();
};
