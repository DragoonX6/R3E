#include "TitanFileSystem.hpp"

VFSFileSystem::VFSFileSystem(){}
VFSFileSystem::VFSFileSystem(const char* baseDir){
	SetBaseDirectory(baseDir);
	IsCurrentSys = true;
}

VFSFileSystem::~VFSFileSystem()
{
	mIndex.Close();
}

bool VFSFileSystem::LoadIndex(const char* path){
	String realpath = path;
	GetFullPath(realpath);
	return mIndex.Load(realpath);
}

File* VFSFileSystem::OpenFile(const char* path, const char* /*mode*/, bool /*dataFile = true*/, bool /*showError = true*/)
{
	// We do a different loading from here, we are loading from defragmented vfs so the root files are loaded from root.vfs
	VFSFILE* entry = new VFSFILE;
	{
		String tmp = path;
		GetFullPath(tmp);
		FlatFile* file = new FlatFile(tmp, "rb");
		if(!file->IsOpen())
		{
			file->Open(path, "rb");
			if(!file->IsOpen())
			{
				delete file;

				// by default looking for flat file, then VFS.
				entry = mIndex.GetVfsFile(path);
				if(!entry)
					return 0;
			}
			else
			{
				return file;
			}
		}
		else
		{
			return file;
		}
		/* replace this by the code above if you want to look in vfs first
		String tmp = path;
		GetFullPath(tmp);
		entry = mIndex.GetVfsFile(path);
		if(!entry)
		{
			FlatFile *file = new FlatFile(tmp, "rb");
			if(!file->IsOpen())
			{
				file->Open(path, "rb");
				if(!file->IsOpen())
				{
					SAFE_DELETE(file);
					return 0;
				}
				return file;
			}
			return file;
		}*/
	}

	VFSFile* file = new VFSFile();
	unsigned char* buffer = new unsigned char[entry->size];
	String vfspath = mBaseDirectory + entry->vfs;
	if(!file->Open(vfspath, "rb"))
		return 0;
	file->SeekVFS(entry->offset);
	file->ReadVFSData(buffer, entry->size);
	file->SetData(buffer, entry->size);
	file->CloseVFS();
	SAFE_DELETE(entry);
	SAFE_DELETE_ARRAY(buffer);
	return file;
}

bool VFSFileSystem::FileExists(const char* path){
	File* fh = OpenFile(path, "rb");
	if(!fh) return false;
	SAFE_DELETE(fh);
	return true;
}

long VFSFileSystem::FileSize(const char* path){
	File* fh = OpenFile(path, "rb");
	if(!fh) return 0;
	long size = fh->Size();
	SAFE_DELETE(fh);
	return size;
}

void VFSFileSystem::GetFullPath(String& path){
	path = mBaseDirectory + path;
}

void VFSFileSystem::SetBaseDirectory(const char* dir){
	mBaseDirectory = dir;
	mBaseDirectory.ReplaceAll("\"", "");
	mBaseDirectory.ReplaceAll("/", "\\");

	if(mBaseDirectory.Length() > 0){
		char end = mBaseDirectory.At(mBaseDirectory.Length() - 1);
		if(end != '\\') mBaseDirectory += "\\";
	}

	LOG("FlatFileSystem Base Directory: '%s'", mBaseDirectory);
}

const char* VFSFileSystem::GetBaseDirectory(){
	return mBaseDirectory;
}

void VFSFileSystem::IndexGetHIM(String Path, List<String> *l)
{
	mIndex.GetAllHIM(Path, l);
}

bool VFSFileSystem::IsCurrentSys = false;