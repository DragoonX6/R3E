#ifndef TITAN_FILE_SYSTEM_H
#define TITAN_FILE_SYSTEM_H

#include "String.hpp"
#include "FileSystem.hpp"
#include "TitanFile.hpp"
#include "FlatFile.hpp"
#include "Log.hpp"
#include <stdio.h>

#include "Index.hpp"
#include <vector>
#include <cassert>

extern class VFSFileSystem *VfsSys;

class VFSFileSystem : public FileSystem {
public:
	VFSFileSystem();
	VFSFileSystem(const char* baseDir);

	virtual ~VFSFileSystem();

	//LMA: not used since we used an extrated client now ;)
	bool LoadIndex(const char* path);
	virtual File* OpenFile(const char* path, const char* /*mode*/, bool /*dataFile*/ = true, bool /*showError*/ = true);
	virtual bool FileExists(const char* path);
	virtual long FileSize(const char* path);
	virtual void GetFullPath(String& path);
	void SetBaseDirectory(const char* dir);
	const char* GetBaseDirectory();
	void PreLoad();
	void GetBatchFiles(String directory, String ext, std::vector<VFSFILE> *l);
	void ClearBatchBuffer();
	static bool IsCurrentSys;

private:
	String mBaseDirectory;
	Index mIndex;
	std::vector<VFSFILE> Filebuff;
};

#endif