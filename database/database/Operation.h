#pragma once
#include "Table.h"
using namespace std;

class Operation{
public:
	virtual Table exec() = 0;
};
