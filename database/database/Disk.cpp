#include "stdafx.h"
const int Disk::BLOCK_SIZE = 512;

Disk::Disk(const string& name) :name(name),db_file(NULL),meta_file(NULL){
	meta_file = fopen("meta", "r+b");
	if (meta_file == NULL){
		meta_file = fopen("meta", "w+b");
	}
	if (meta_file == NULL)
		return;
	const int BUFFER_SIZE = 4096;
	char tmp[BUFFER_SIZE];
	string current_name;
	globalMaxId = -1;
	while (fgets(tmp, BUFFER_SIZE, meta_file) != NULL){
		++globalMaxId;
		stringstream stream(tmp);
		stream >> current_name;
		if (current_name == name){
			blockID.push_back(globalMaxId);
		}
	}
	db_file = fopen("db", "r+b");
	if (db_file == NULL)
		db_file = fopen("db", "w+b");
}
int Disk::readBlock(const int id, char * data){
	if (db_file == NULL)
		return DB_FILE_ERROR;
	if (meta_file == NULL)
		return META_FILE_ERROR;
	while (id >= blockID.size()){
		if (allocateBlock()){
			return ALLOC_ERROR;
		}
	}
	int real_id = blockID[id];
	fseek(db_file, BLOCK_SIZE * real_id, SEEK_SET);
	fread(data, sizeof(char), BLOCK_SIZE, db_file);
	return 0;
}

int Disk::writeBlock(const int id, char * data){
	if (db_file == NULL)
		return DB_FILE_ERROR;
	if (meta_file == NULL)
		return META_FILE_ERROR;
	while (id >= blockID.size()){
		if (allocateBlock()){
			return ALLOC_ERROR;
		}
	}
	int real_id = blockID[id];
	fseek(db_file, BLOCK_SIZE * real_id, SEEK_SET);
	fwrite(data, sizeof(char), BLOCK_SIZE , db_file);
	return 0;

}
int Disk::allocateBlock(){
	if (fseek(db_file, 0, SEEK_END))
		return -1;
	char buffer[BLOCK_SIZE];
	memset(buffer, 0, sizeof buffer);
	fwrite(buffer, sizeof (char), BLOCK_SIZE, db_file);
	blockID.push_back(++globalMaxId);
	fseek(meta_file, 0, SEEK_END);
	fprintf(meta_file, "%s\n", name.c_str());
	return 0;
}
Disk::~Disk(){
	if (db_file != NULL)
		fclose(db_file);
	if (meta_file != NULL)
		fclose(meta_file);
}

