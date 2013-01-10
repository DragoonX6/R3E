#ifndef ROSE_INDEX_H
#define ROSE_INDEX_H

#include <vector>
#include "FlatFile.hpp"
#include "Log.hpp"
#include "Hashtable.h"

typedef struct
{
	unsigned long filecount;
	unsigned long deletecount;
	unsigned long startoffset;
}VFSINFO; 
typedef struct
{
	char *vfs;
	unsigned short strlen;
	char *path;
	unsigned long offset;
	unsigned long size;
	unsigned long blockSize;
	bool deleted;
	unsigned char compressiontype;
	unsigned char encryptiontype;
	unsigned long version;
	unsigned long checksum;
}VFSFILE;

class Index
{
public:
	Index();
	//Index(const char* path);
	~Index();
	bool Load(const char *path, std::vector<VFSFILE*> &Entry, CHashTable<VFSFILE*> &Table);
	unsigned long GetBaseVersion() const;
	unsigned long GetCurrentVersion() const;
	unsigned long VfsCount() const;
	unsigned long GetVfsOffet(unsigned long index) const;
	String GetVfsName(unsigned long index) const;
	VFSINFO *GetVfsInfo(const char *filename) const;
	void GetAllVfsFiles(std::vector<VFSFILE> *l);
	void GetBatchFiles(String directory, String ext, std::vector<VFSFILE> *l);
	void Close() const;
private:
	unsigned long curversion;
	unsigned long basever;
	unsigned long vfscount;
	unsigned long *offset;

	String *vfsname;

	VFSINFO **vinfo;
	VFSFILE *vfsfile;
	FlatFile *fIndex;
	const char* file;
};

#endif