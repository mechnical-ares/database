#pragma once
#include "stdafx.h"
class Disk{
public:
	Disk(const string& name);
	int readBlock(const int id,  char * data);
	int writeBlock(const int id, char* data);
	static const int BLCOK_SIZE = 512;
};