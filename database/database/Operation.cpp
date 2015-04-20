#include "stdafx.h"
bool isEQ(const Data& left, const Data& right){
	if (left.type != right.type)
		return false;
	return left.data == right.data;
}
bool isNE(const Data& left, const Data& right){
	if (left.type != right.type)
		return false;
	return left.data != right.data;
}
bool isGT(const Data& left, const Data& right){
	if (left.type != right.type)
		return false;
	return left.data > right.data;
}
bool isLT(const Data& left, const Data& right){
	if (left.type != right.type)
		return false;
	return left.data < right.data;
}
bool isGE(const Data& left, const Data& right){
	if (left.type != right.type)
		return false;
	return left.data >= right.data;
}
bool isLE(const Data& left, const Data& right){
	if (left.type != right.type)
		return false;
	return left.data <= right.data;
}