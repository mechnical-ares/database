#include "stdafx.h"

bool isEQ(const Data& left, const Data& right){
	if (left.type != right.type)
		return false;
	return left.data == right.data;
}
bool isNE(const Data& left, const Data& right){
	return !isEQ(left, right);
}
bool isGT(const Data& left, const Data& right){
	if (left.type != right.type)
		return false;
	if (left.type == DB_INT){
		int l = atoi(left.data.c_str());
		int r = atoi(right.data.c_str());
		return l > r;
	}
	if (left.type == DB_DOUBLE){
		double l = stod(left.data.c_str());
		double r = stod(right.data.c_str());
		return l > r;
	}
	if (left.type == DB_BOOL){
		int l = left.data == "true" ? 1 : 0;
		int r = left.data == "true" ? 1 : 0;
		return l > r;
	}
	return left.data > right.data;
}
bool isLT(const Data& left, const Data& right){
	if (left.type != right.type)
		return false;
	if (left.type == DB_INT){
		int l = atoi(left.data.c_str());
		int r = atoi(right.data.c_str());
		return l < r;
	}
	if (left.type == DB_DOUBLE){
		double l = stod(left.data.c_str());
		double r = stod(right.data.c_str());
		return l < r;
	}
	if (left.type == DB_BOOL){
		int l = left.data == "true" ? 1 : 0;
		int r = left.data == "true" ? 1 : 0;
		return l < r;
	}
	return left.data < right.data;
}
bool isGE(const Data& left, const Data& right){
	return !isLT(left, right);
}
bool isLE(const Data& left, const Data& right){
	return !isGT(left, right);
}