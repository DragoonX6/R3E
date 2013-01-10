#include "TitanFileSystem.hpp"

VFSFileSystem::VFSFileSystem()
{
}
VFSFileSystem::VFSFileSystem(const char* baseDir){
	SetBaseDirectory(baseDir);
	//IsCurrentSys = true;
}

VFSFileSystem::~VFSFileSystem()
{
	mIndex.Close();
}

bool VFSFileSystem::LoadIndex(const char* path){
	String realpath = path;
	GetFullPath(realpath);
	return mIndex.Load(realpath, FileEntry, FileTbl);
}

File* VFSFileSystem::OpenFile(const char* path, const char* /*mode*/, bool /*dataFile = true*/, bool /*showError = true*/)
{
	// We do a different loading from here, we are loading from defragmented vfs so the root files are loaded from root.vfs
	VFSFILE* entry = new VFSFILE;
	{
		if(FileEntry.size() != 0) // Try to find the file in the list buffer, this is faster then looking in the vfs this mainly used for loading maps quicker
		{
			std::string name(path);
			std::replace(name.begin(), name.end(), '/', '\\');
			if(name.find("\\\\") != (-1))
			{
				for(;name.find("\\\\") != (-1);)
				{
					int off = name.find("\\\\");
					name.erase(off+1, 1);
				}
			}
			char* normalizedpath = (char*)name.c_str();
			if(FileTbl.getHash(toHash(normalizedpath), &entry))
			{
				goto loadfromvfs;
			}
			else
			{
				String tmp = normalizedpath;
				FlatFile* file = new FlatFile(tmp, "rb");
				if(!file->IsOpen())
				{
					GetFullPath(tmp);
					file->Open(tmp, "rb");
					if(!file->IsOpen())
					{
						SAFE_DELETE(file);
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
			}
			return 0;
		}
		if(FileEntry.size() == 0)
		{
			String tmp = path;
			FlatFile* file = new FlatFile(tmp, "rb");
			if(!file->IsOpen())
			{
				GetFullPath(tmp);
				file->Open(tmp, "rb");
				if(!file->IsOpen())
				{
					SAFE_DELETE(file);
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
		}
	}

loadfromvfs:
	VFSFile* file = new VFSFile();
	unsigned char* buffer = new unsigned char[entry->size];
	String vfspath = mBaseDirectory + entry->vfs;
	if(!file->Open(vfspath, "rb"))
		return 0;
	file->SeekVFS(entry->offset);
	file->ReadVFSData(buffer, entry->size);
	file->SetData(buffer, entry->size);
	file->CloseVFS();
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

//bool VFSFileSystem::IsCurrentSys = false;