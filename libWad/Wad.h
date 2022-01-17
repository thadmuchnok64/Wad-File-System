#pragma once
#include <string>
#include <vector>
//#include <iostream>

class Wad {

public:
	//Header
	std::string magic;
	int numOfDescriptors;
	int descriptorOffset;
	//Data
	std::vector<char> data;
	char* dataBuffer;
	//descriptors
	std::vector<int> elementOffsets;
	std::vector<int> elementLengths;
	std::vector<std::string> names;

	std::string getMagic();

	bool isContent(const std::string &path);

	bool isDirectory(const std::string &path);

	int getSize(const std::string &path);

	int getContents(const std::string &path, char* buffer, int length, int offset=0);

	int getDirectory(const std::string &path, std::vector<std::string> *directory);

	int doesPathExist(const std::string &path);

	static Wad* loadWad(const std::string &path);


	Wad();

	~Wad();

};


