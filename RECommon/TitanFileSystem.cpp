#include "TitanFileSystem.hpp"

VFSFileSystem::VFSFileSystem()
{
}
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
		if(Filebuff.size() != 0) // Try to find the file in the list buffer, this is faster then looking in the vfs this mainly used for loading maps quicker
		{
			for(unsigned int i = 0; i < Filebuff.size(); ++i)
			{
				String tmp = Filebuff[i].path;
				if(!tmp.Compare(path, false))
				{
					FlatFile* file = new FlatFile(tmp, "rb");
					if(!file->IsOpen())
					{
						String tmp2 = tmp;
						GetFullPath(tmp2);
						file->Open(tmp2, "rb");
						if(!file->IsOpen())
						{
							SAFE_DELETE(file);
							entry = &Filebuff[i];
							goto loadfromvfs;
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
			return 0;
		}
		if(Filebuff.size() == 0)
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

void VFSFileSystem::PreLoad()
{
	mIndex.GetAllVfsFiles(&Filebuff);
}

void VFSFileSystem::GetBatchFiles(String directory, String ext, std::vector<VFSFILE> *l)
{
	directory.ReplaceAll("/", "\\");
	directory.ToUpper();
	for(unsigned int i = 0; i < Filebuff.size(); ++i)
	{
		if(strstr(Filebuff[i].path, directory.Str()) && strstr(Filebuff[i].path, ext.Str()))
		{
			l->push_back(Filebuff[i]);
		}
	}
}

void VFSFileSystem::ClearBatchBuffer()
{
	Filebuff.clear();
}

bool VFSFileSystem::IsCurrentSys = false;