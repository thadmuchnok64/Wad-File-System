//#define FUSE_USE_VERSION 26
#include <fuse.h>
#include <string>
#include <errno.h>
#include <stdio.h>
#include <sys/types.h>
#include <time.h>
#include <stdlib.h>
#include "../libWad/Wad.h"
#include "../libWad/Wad.cpp"
#include <unistd.h>
#include <sys/syscall.h>
#include <cstring>
#include <iostream>

Wad* loadedWad;


static int do_getattr(const char* path, struct stat* st){

memset(st,0,sizeof(struct stat));

//st->st_uid = getuid();
//st->st_gid = getgid();
st->st_atime = time(NULL);
st->st_mtime = time(NULL);
std::cout<<path<<std::endl;
if(path=="/"|| loadedWad->isDirectory(path) ==1 )
{
st->st_mode = S_IFDIR | 0755;
st->st_nlink - 2;

} else if (loadedWad->isContent(path)==1)
{
st->st_mode = S_IFREG | 0644;
st->st_nlink = 1;
st->st_size = loadedWad->getSize(path);

}else
{
return -ENOENT;

}

return 0;

}


static int do_readdir(const char* path, void* buffer, fuse_fill_dir_t filler, off_t offset, struct fuse_file_info* fi)
{

filler(buffer, ".", NULL, 0 );
filler(buffer, "..",NULL, 0 );
//filler(buffer, "fortnite",NULL,0);
//char a[loadedWad->names.at(2).size()+1];
//strcpy(a,loadedWad->names.at(2).c_str());
//filler(buffer,a,NULL,0);
std::vector<std::string> dir;
loadedWad->getDirectory(path,&dir);
for(int i = 0; i < dir.size() ; i++){
char c[dir.at(i).size()+1];
strcpy(c,dir.at(i).c_str());
filler(buffer, c,NULL,0);

}

return 0;

}

//static const char* filePath = "/file";
//static const char* fileName = "file";
//static const char* fileContent = "Content content content... where are you?";

static int read_callback(const char* path, char* buf, size_t size, off_t offset, struct fuse_file_info* fi){

char buffer[loadedWad->getSize(path)];
int fileIndex = loadedWad->doesPathExist(path); // Returns the index of the path in the array of files and directories

	if(fileIndex==-1){
	return -1;
	}
/*

		char* test = "please work oh god why";
		memcpy(buf,test,size);
		return size;
		*/

		size_t len = loadedWad->getContents(path,buffer,loadedWad->getSize(path),loadedWad->elementOffsets.at(fileIndex));
//		memcpy(buf,buffer+offset,size);
		memcpy(buf,buffer,size);
		return strlen(buffer);//-offset;

//		return -ENOENT;

}

static struct fuse_operations fuse_example_operations = {

.getattr = do_getattr,
.read = read_callback,
.readdir = do_readdir,

};


int main(int argc, char* argv[])
{
	if(argc < 3){
	std::cout<<"Missing arguments!"<<std::endl;
	return -1;
	}

//	if(argc>=1)
	//loadedWad = loadWad(argv[1]);
	loadedWad = Wad::loadWad(argv[1]);//"/home/reptilian/wad/DOOM1.WAD");
//	for(int i = 0; i < loadedWad->names.size() ; i++){
//	std::cout<<loadedWad->names.at(i)<<std::endl;
//	}
//	do_getattr(loadedWad.names.at(i)

//	}

	char* newArg[2];
	newArg[0] = argv[1];
	newArg[1] = argv[2];
	argc = 2;
	return fuse_main(argc,newArg, &fuse_example_operations, NULL);
//	std::cout<<"spaghetti"<<std::endl;
	//free(loadedWad);
	delete loadedWad;
}


/*
static const char *filepath = "/file";
static const char* filename = "file";
static const char* filecontent = "Spaghetti time";

static int getattr_callback(const char* path, struct stat* stbuf){

	if(strcmp(path, "/") == 0) {
	stbuf->st_mode = S_IFDIR | 0755;
	stbuf->st_nlink = 2;
	return 0;
	}

	if(strcmp(path, filepath) == 0){

	stbuf->st_mode = S_IFREG | 0777;
	stbuf->st_nlink = 1;
	stbuf->st_size = strlen(filecontent);
	return 0;
	}

	return -ENOENT;

}

static int read_callback(const char* path, char* buf, size_t size, off_t offset, struct fuse_file_info *fi){

if(strcmp(path,filepath)==0){

size_t len = strlen(filecontent);
if(offset >= len){

return 0;
}
if(offset + size > len){

memcpy(buf, filecontent + offset, len-offset);
return len - offset;
}

memcpy(buf, filecontent + offset, size);
return size;
}

return -ENOENT;
}

static int readdir_callback(const char* path, void* buf, fuse_file_dir_t filler, off_t offset, struct fuse_file_info* fi){

(void) offset;
(void) fi;

filler(buf, ".", NULL, 0);
filler(buf, "..", NULL, 0);
filler(buf,filename, NULL, 0);
return 0;
}

static struct fuse_operations fuse_example_operations = {

.getattr = getattr_callback,
.open = open_callback,
.read = read_callback,
.readdir = readdir_callback,
};

int main(int argc, char* argv[]){

return fuse_main(argc, argv, &fuse_example_operations, NULL);
}

}
*/
