#include <cstdint>
#include <string>
#include <string.h>
#include <fstream>
#include <iostream>
//#include <iomanip>
#include <vector>
#include <ctype.h>
#include <algorithm>
#include "Wad.h"
/*
class Wad {
public:
	//header
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
*/

	Wad::Wad(){
		//constructor
	}
	Wad::~Wad(){
		//destructor
	}

	Wad* Wad::loadWad(const std::string &path){


		//Wad* w = &Wad();
		//Wad* w = (Wad*)malloc(sizeof(Wad));
//		new(w) Wad;
		Wad* w = new Wad();
		std::ifstream infile(path);
		infile.seekg(0,std::ios::end);
		size_t length = infile.tellg();
		infile.seekg(0,std::ios::beg);
	//	char* buffer = (char*)malloc(sizeof(char)*(sizeof(length)+1));

		char buffer[4];
		char buffer2[8];
		char byt;
		infile.read(buffer,4);
		//char mag[] = {buffer[0],buffer[1],buffer[2],buffer[3]};
		std::string s(buffer,sizeof(buffer));
		w->magic = s;

		infile.read(buffer,4);
		int i;
		memcpy(&i,buffer,sizeof(i));
		//char mag1[] = {buffer[4],buffer[5],buffer[6],buffer[7]};
		w->numOfDescriptors = i; // buffToInt(buffer);
	//	debugging stuff
	//	std::cout << w->magic << std::endl;

		infile.read(buffer,4);
		memcpy(&i,buffer,sizeof(i));
		w->descriptorOffset = i;


		//navigates past directory offset

		for(int x = 0 ; x<i-12; x++)
		{

			infile.read(&byt,1);
			w->data.push_back(byt);
		}
		//std::cout<<w->data.at(200)<<std::endl;
		w->dataBuffer = w->data.data();
		//reads directories
		std::vector<std::string> pathName;

		//pathName.push_back("/");
		int markerChecker = 12;


		for(int x = 0 ; x<w->numOfDescriptors ; x++){

			std::string lastPath="";
			if(pathName.size()>0){
			for(int z = 0 ; z < pathName.size();z++){

				lastPath = lastPath + pathName.at(z);
}			}
//std::cout<<lastPath<<std::endl;
			infile.read(buffer,4);
			memcpy(&i,buffer,sizeof(i));
			w->elementOffsets.push_back(i);
	//	std::cout<<"Offset: "<<i;
			infile.read(buffer,4);
			memcpy(&i,buffer,sizeof(i));
	//	std::cout<<"	Length: "<<i;
			w->elementLengths.push_back(i);
			infile.read(buffer2,8);
			std::string st(buffer2,sizeof(buffer2));
	//	std::cout<<"	Name: "<<st<<std::endl;
//			std::cout<<st<<std::endl;
//			std::cout<<st.find("M")<<"   size: "<< st.length() <<std::endl;
			if (st.find("E")==0&&isdigit(st.at(1))!=0&&st.find("M")==2&&isdigit(st.at(3))!=0){
//			std::cout<<"SOMETHING SHOULD HAPPEN HERE!"<<std::endl;	
			markerChecker = 1;
			st = "/"+st;
			st.erase(std::find(st.begin(),st.end(),'\0'),st.end());
				pathName.push_back(st);
			w->names.push_back(lastPath+st);
//			std::cout<<(lastPath+st)<<"  "<<i<<std::endl;

			}
			else if (st.find("_START") != std::string::npos){
		//	std::cout<<(st)<<std::endl;

				st = st.substr(0,st.find("_START"));
				st = "/"+st;
				st.erase(std::find(st.begin(),st.end(),'\0'),st.end());
				pathName.push_back(st);


			w->names.push_back(lastPath+st);
//			std::cout<<(lastPath+st)<<"  "<<i<<std::endl;


			//	pathName.push_back(st);
			} else if(st.find("_END")!=std::string::npos){

				st = st.substr(0,st.find("_END"));

			if(pathName.size()>0){
//				st.erase(std::find(st.begin(),st.end(),'\0'),st.end());

				pathName.erase(pathName.end());
				}
			//	w->names.push_back(pathName
//			std::cout<<(lastPath)<<"  "<<i<<std::endl;

			w->names.push_back(lastPath);
			} else{

			st.erase(std::find(st.begin(),st.end(),'\0'),st.end());
//			std::cout<<(lastPath+"/"+st)<<"  "<<i<<std::endl;
			w->names.push_back(lastPath+"/"+st);
			}
			if(markerChecker == 11 && pathName.size()>0){
//				std::cout<<"DEBUG: "<<pathName.at(pathName.size()-1)<<std::endl;

				pathName.erase(pathName.end());
			}

		markerChecker++;
}

		//std::string st(buffer2,sizeof(buffer2));
		//std::cout<<st<<std::endl;

		return w;
	}

	std::string Wad::getMagic(){

	return magic;

	}



	bool Wad::isDirectory(const std::string &path){

		if(path=="/"){
			return true;
		}
		if(doesPathExist(path)<0){
			return false;
		}
		return !isContent(path);

	}

	bool Wad::isContent(const std::string &path){

		if(doesPathExist(path)<0){
			return false;
		}
		if(elementLengths.at(doesPathExist(path))>0){
		return true;
	//	std::cout<<doesPathExist(path)<<std::endl;
		} else{
		return false;
	//	std::cout<<"Path doesn't exist" <<std::endl;
		}

	}

	int Wad::getSize(const std::string &path){

	if(isContent(path)){
		return elementLengths.at(doesPathExist(path));
	}
	return -1;
	}


	char* test;


	int Wad::getContents(const std::string &path ,char* buffer,int length, int offset){

	if(isContent(path)){
	int copied = 0;
	std::vector<char> c;
	int charToCopy = length;
	//std::cout<<dataBuffer[200];
	if(getSize(path)<length){
		charToCopy = getSize(path);
	}

//	if(charToCopy>sizeof(buffer)){

//	char* newArray = new char[charToCopy];
//	char* temp = buffer;
//	buffer = newArray;
//	delete[] temp;
	int newOff= offset;
//	}
	if(offset>=12){
	newOff = offset - 12;
	}
	for(int i = newOff; i< newOff+charToCopy ; i++){
//	if(copied<strlen(buffer)){
	buffer[copied] = dataBuffer[i];
	copied++;
	//c.push_back(dataBuffer[i]);
//	} else { break;}
//	std::cout<<c.at(i)<<std::endl;
	}
//	std::cout<<std::endl;
//	char* spaghetti;
	//c.push_back(dataBuffer[200]);
	//char* ar;
	//ar = (char*)malloc(sizeof(*ar));
//	std::copy(c.begin(),c.end(),spaghetti);
//	strncpy(buffer,spaghetti,charToCopy);

//	buffer = spaghetti;

	//buffer = ar;
	//problem line
	//memcpy(buffer,ar,sizeof(ar));
	//buffer = ar;
	return copied;
	}
	return -1;

	}


	int Wad::getDirectory(const std::string &path, std::vector<std::string> *directory){
		int it = 0;
		std::string pathFixer = path;
		if(pathFixer != "/" && pathFixer.back()=='/'){
		pathFixer.pop_back();
		}
//		std::cout<<"DEBUG: "<<pathFixer<<std::endl;
		if(isDirectory(pathFixer)){
			if(path!="/"){

			int index = doesPathExist(pathFixer);
			index++;
			std::string sds;
		//	if(path=="/")
		//	sds = "";
		//	else
			sds = pathFixer;
			//if(sds.back()=='/')
			//sds.pop_back();
//			std::cout<<"DEBUG: "<<sds<<std::endl;
			const std::string newPath = sds + '/';

			while(index<names.size()&&names.at(index).find(pathFixer)!=-1){
				std::string s;
				s = names.at(index).substr(names.at(index).find(newPath)+newPath.length());
				bool tester = true;
				if(s.find("/")!=-1){
				s = s.substr(0,s.find("/"));
				}
				int itt = 0;
				while(tester==true&&itt<directory->size())
				{

					if(directory->at(itt)==s){
					tester=false;
					}
					itt++;
//					std::cout<<itt<<std::endl;
				}


				if(tester&&s!=""){
				directory->push_back(s);
				//directory->push_back(names.at(index).substr(names.at(index).find(newPath)+newPath.length()));
				it++;
			}
			index++;
			}
			return it;
		} else {
		int itt = 0;
		for(int i = 0 ; i < names.size();i++){
		if(names.at(i).size()>0){
		std::string s;
		s = names.at(i);
		s = s.substr(1);
		bool tester = true;
		if(s.find("/")!=-1){
		tester = false;
		}
		for( int a = 0 ; a < directory->size() ; a++){
		if(s==directory->at(a)){
		tester = false;
		break;
		}
		}
		if(tester){
		directory->push_back(s);
		itt++;
		}
		}
		}
		 return itt;
		}
	}
		return -1;

	}


	// checks if the path exists. Returns the index if it does, otherwise returns -1.
	int Wad::doesPathExist(const std::string &path){
		//std::cout<<path<<std::endl;
		for(int i = 0 ; i < names.size() ; i++){
//			std::cout<<names.at(i)<<std::endl;
			if(names.at(i).length()==path.length()){
			bool equal = true;
			int y = 0;
			for(int x = 0 ; x<names.at(i).length() ; x++){
//			std::cout <<names.at(i)<<" "<<path<<" ";
		//	if(path.at(y)=='/'){
		//		while(x%9!=0){
		//			x++;
		//		}
		//	}

			if(x<path.length()&&x<names.at(i).length()&&names.at(i).at(x)!=path.at(x)){

			//	} else {
				equal = false;
				break;
			//	}
		//	} else if(x<names.at(i).size()&&path.at(y)=='/'){
		//		y--;
			}else if(names.at(i).size()==0){
				equal = false;
			}

			}
		//std::cout << std::endl;
		if(equal)
		return i;
		}
}
	return -1;
	}


//};
