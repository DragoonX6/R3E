#include "Index.hpp"

Index::Index() :curversion(0), basever(0), vfscount(0), vinfo(0), vfsfile(0)
{
}

Index::Index(const char* path) :curversion(0), basever(0), vfscount(0), vinfo(0), vfsfile(0)
{
	Load(path);
}

Index::~Index()
{
	SAFE_DELETE_ARRAY(vfsname);
	SAFE_DELETE(vfsfile);
	SAFE_DELETE_ARRAY(vinfo);
	if(fIndex->IsOpen())
	{
		fIndex->Close();
	}
}

bool Index::Load(const char *path)
{
	fIndex = new FlatFile(path, "rb");
	if(!fIndex->IsOpen())
		return false;
	file = path;
	fIndex->Read(basever);
	fIndex->Read(curversion);
	fIndex->Read(vfscount);
	vfsname = new String[vfscount];
	offset = new unsigned long[vfscount];
	for(unsigned long i = 0; i < vfscount; i++)
	{
		unsigned short VfsNameLenght;
		fIndex->Read(VfsNameLenght);
		char *tempname = new char[VfsNameLenght];
		fIndex->ReadData(tempname, VfsNameLenght);
		vfsname[i] = tempname;

		fIndex->Read(offset[i]);
	}
	vinfo = new VFSINFO*[vfscount];
	for(unsigned long i = 0; i < vfscount; i++)
	{
		vinfo[i] = new VFSINFO;
		fIndex->Seek(offset[i]);
		fIndex->Read(vinfo[i]->filecount);
		fIndex->Read(vinfo[i]->deletecount);
		fIndex->Read(vinfo[i]->startoffset);
	}
	return true;
}

/*
bool Index::New(const char* path)
{
	if(!fIndex->Open(path, "wb"))
	{
		return false;
	}
	file = path;
	return true;
}

void Index::WriteBaseVersion(unsigned long ver)
{
	if(fIndex->Position() != 0)
	{
		fIndex->Seek(0);
	}
	fIndex->Write(ver);
	basever = ver;
}

void Index::WriteCurrentVersion(unsigned long ver)
{
	if(fIndex->Position() != 4)
	{
		fIndex->Seek(4);
	}
	fIndex->Write(ver);
	curversion = ver;
}

void Index::WriteVFSCount(unsigned long cnt)
{
	if(fIndex->Position() != 8)
	{
		fIndex->Seek(8);
	}
	fIndex->Write(cnt);
	vfscount = cnt;
	SAFE_DELETE_ARRAY(vfsname);
	vfsname = new String[vfscount];
}

void Index::WriteVFSName(String name, unsigned long num) // not sure if works
{
	/ * <xor_mind-> I explain it in the following line, shift all data after the block you want to delete, by the size of the block you want to delete, then resize the file
	<xor_mind-> you can do that easily with file pointers
	<xor_mind-> or a memory map
	<xor_mind-> in winderz * /
	bool VfsExists = false;
	unsigned char* Buffer;
	unsigned char* Buffer2;
	unsigned long thisoffset = 0;
	unsigned long ToRead = 0;
	unsigned long vfsoffset = 0;
	unsigned long toadd = 0;
	if(num > vfscount)
		return;
	fIndex->Seek(12);
	unsigned short strlen;
	for(unsigned long l = 0; l < num; l++)
	{
		char* vfs;
		fIndex->Read(strlen);
		vfs = new char[strlen];
		fIndex->ReadData(vfs, strlen);
		fIndex->Read(vfsoffset);
		if(!name.Compare(vfs))
		{
			VfsExists = true;
			SAFE_DELETE_ARRAY(vfs);
			break;
		}
		SAFE_DELETE_ARRAY(vfs);
	}
	ToRead = fIndex->Size() - fIndex->Position();

	Buffer = (unsigned char*)malloc(ToRead); // get the end of the file
	fIndex->ReadData(Buffer, ToRead);

	if(VfsExists)
	{
		return; // we don't overwrite vfs's
	}
	Buffer2 = (unsigned char*)malloc(fIndex->Position()); // get the first part of the file
	fIndex->Seek(0); // back 2 begin
	fIndex->ReadData(Buffer2, sizeof(Buffer2));

	fIndex->Close();
	fIndex->Delete();
	fIndex->Open(file, "wb"); // reopen
	fIndex->Write(Buffer2); // write the first part of the file

	strlen = (unsigned short)name.Length();
	fIndex->Write(strlen);
	fIndex->Write(name.Str());
	vfsoffset = fIndex->Position();
	fIndex->Write<unsigned long>(0);
	fIndex->Write(Buffer);
	fIndex->Seek(vfsoffset);
	fIndex->Write(fIndex->Size());
}

void Index::WriteVFSInfo(VFSINFO* info) // not used yet
{
	if(fIndex->Position() != 12) // TODO
	{
		fIndex->Seek(12);
	}
	fIndex->Write(info);
}

void Index::WriteVFSFile(VFSFILE* file)
{

}*/

unsigned long Index::GetBaseVersion() const
{
	return basever;
}

unsigned long Index::GetCurrentVersion() const
{
	return curversion;
}

unsigned long Index::VfsCount() const
{
	return vfscount;
}

unsigned long Index::GetVfsOffet(unsigned long index) const
{
	if(index > vfscount)
	{
		return 0;
	}
	return offset[index];
}

String Index::GetVfsName(unsigned long index) const
{
	if(index > vfscount)
	{
		return NULL;
	}
	return vfsname[index];
}

VFSINFO *Index::GetVfsInfo(const char *filename) const
{
	for(unsigned long i = 0; i <= vfscount; i++)
	{
		if(!vfsname[i].Compare(filename))
		{
			return vinfo[i];
		}
	}
	return NULL;
}

VFSFILE *Index::GetVfsFile(const char* filename) 
	// function is too slow, trying to find a way to speed up the process
	// function is still too slow, the cause is the map loading algorithm, 
	// I need to find a way to check if the map is completely loaded. Or to check if it's in the map folder or not
{
	String fname = filename;
	fname.ReplaceAll("\\\\", "\\", fname.Begin());
	fname.ReplaceAll("/", "\\", fname.Begin());
	unsigned long i = 0;
	while(i < (vfscount))
	{
		fIndex->Seek(offset[i]);
		fIndex->Skip((sizeof(unsigned long) * 3)); // skip vfsinfo to read fileinfo
		bool isRoot = false;
		if(!vfsname[i].Compare("ROOT.VFS"))
		{
			isRoot = true;
		}
		unsigned long j = 0;
		while(j < vinfo[i]->filecount)
		{
			if(isRoot)
				break;
			unsigned long thisoffset = fIndex->Position();
			unsigned short len;
			fIndex->Read(len);
			char* str = new char[len];
			fIndex->ReadData(str, len);
			fIndex->Skip(12);
			bool del = false;
			fIndex->Read(del);
			fIndex->Skip(10);
			if(!fname.Compare(str, false))
			{
				if(del == true)
				{
					SAFE_DELETE_ARRAY(str);
					continue;
				}
				vfsfile = new VFSFILE;
				fIndex->Seek(thisoffset);
				vfsfile->vfs = vfsname[i].Str();
				fIndex->Read(vfsfile->strlen);
				vfsfile->path = new char[vfsfile->strlen];
				fIndex->ReadData(vfsfile->path, vfsfile->strlen);
				fIndex->Read(vfsfile->offset); // 4
				fIndex->Read(vfsfile->size); // 4
				fIndex->Read(vfsfile->blockSize); // 4
				fIndex->Read(vfsfile->deleted); // 1
				fIndex->Read(vfsfile->compressiontype); // 1
				fIndex->Read(vfsfile->encryptiontype); // 1
				fIndex->Read(vfsfile->version); // 4
				fIndex->Read(vfsfile->checksum); // 4
				// don't mind the numbers, they're just the type sizes
				return vfsfile;
			}
			SAFE_DELETE_ARRAY(str);
			j++;
		}
		i++;
	}
	LOG("Couldn't find: %s", filename);
	return NULL;
}

void Index::GetBatchFiles(String directory, String ext, std::vector<VFSFILE> *l)
{
	directory.ReplaceAll("/", "\\");
	unsigned long i = 0;
	while(i < (vfscount))
	{
		fIndex->Seek(offset[i]);
		fIndex->Skip((12)); // skip vfsinfo to read fileinfo
		bool isRoot = false;
		if(!vfsname[i].Compare("ROOT.VFS"))
		{
			isRoot = true;
		}
		unsigned long j = 0;
		while(j < vinfo[i]->filecount)
		{
			if(isRoot)
				break;
			unsigned long thisoffset = fIndex->Position();
			unsigned short len;
			fIndex->Read(len);
			char *tmp = new char[len];
			fIndex->ReadData(tmp, len);
			fIndex->Skip(12);
			bool del = false;
			fIndex->Read(del);
			fIndex->Skip(10);
			if(strstr(tmp, directory.Str()) && strstr(tmp, ext.Str()))
			{
				if(!del)
				{
					VFSFILE vfs;
					fIndex->Seek(thisoffset);
					vfs.vfs = vfsname[i].Str();
					fIndex->Read(vfs.strlen);
					vfs.path = new char[vfs.strlen];
					fIndex->ReadData(vfs.path, vfs.strlen);
					fIndex->Read(vfs.offset); // 4
					fIndex->Read(vfs.size); // 4
					fIndex->Read(vfs.blockSize); // 4
					fIndex->Read(vfs.deleted); // 1
					fIndex->Read(vfs.compressiontype); // 1
					fIndex->Read(vfs.encryptiontype); // 1
					fIndex->Read(vfs.version); // 4
					fIndex->Read(vfs.checksum); // 4
					l->push_back(vfs);
				}
			}
			SAFE_DELETE_ARRAY(tmp);
			j++;
		}
		i++;
	}
}

void Index::GetAllVfsFiles(std::vector<VFSFILE> *l)
{
	unsigned long i = 0;
	while(i < (vfscount))
	{
		fIndex->Seek(offset[i]);
		fIndex->Skip((12)); // skip vfsinfo to read fileinfo
		bool isRoot = false;
		if(!vfsname[i].Compare("ROOT.VFS"))
		{
			isRoot = true;
		}
		unsigned long j = 0;
		while(j < vinfo[i]->filecount)
		{
			if(isRoot)
				break;
			unsigned long thisoffset = fIndex->Position();
			unsigned short len;
			fIndex->Read(len);
			char *tmp = new char[len];
			fIndex->ReadData(tmp, len);
			fIndex->Skip(12);
			bool del = false;
			fIndex->Read(del);
			fIndex->Skip(10);
			if(!del)
			{
				VFSFILE vfs;
				fIndex->Seek(thisoffset);
				vfs.vfs = vfsname[i].Str();
				fIndex->Read(vfs.strlen);
				vfs.path = new char[vfs.strlen];
				fIndex->ReadData(vfs.path, vfs.strlen);
				fIndex->Read(vfs.offset); // 4
				fIndex->Read(vfs.size); // 4
				fIndex->Read(vfs.blockSize); // 4
				fIndex->Read(vfs.deleted); // 1
				fIndex->Read(vfs.compressiontype); // 1
				fIndex->Read(vfs.encryptiontype); // 1
				fIndex->Read(vfs.version); // 4
				fIndex->Read(vfs.checksum); // 4
				l->push_back(vfs);
			}
			SAFE_DELETE_ARRAY(tmp);
			j++;
		}
		i++;
	}
}

void Index::Close() const
{
	fIndex->Close();
}