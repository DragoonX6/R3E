#include "Index.hpp"

Index::Index() :curversion(0), basever(0), vfscount(0), vinfo(0), vfsfile(0)
{
}

/*Index::Index(const char* path) :curversion(0), basever(0), vfscount(0), vinfo(0), vfsfile(0)
{
	Load(path);
}*/

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

bool Index::Load(const char *path, std::vector<VFSFILE*> &Entry, CHashTable<VFSFILE*> &Table)
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
				VFSFILE* vfs = new VFSFILE;
				fIndex->Seek(thisoffset);
				vfs->vfs = vfsname[i].Str();
				fIndex->Read(vfs->strlen);
				vfs->path = new char[vfs->strlen];
				fIndex->ReadData(vfs->path, vfs->strlen);
				fIndex->Read(vfs->offset); // 4
				fIndex->Read(vfs->size); // 4
				fIndex->Read(vfs->blockSize); // 4
				fIndex->Read(vfs->deleted); // 1
				fIndex->Read(vfs->compressiontype); // 1
				fIndex->Read(vfs->encryptiontype); // 1
				fIndex->Read(vfs->version); // 4
				fIndex->Read(vfs->checksum); // 4
				Entry.push_back(vfs);
				Table.addHash(toHash(vfs->path), vfs);
			}
			SAFE_DELETE_ARRAY(tmp);
			j++;
		}
		i++;
	}
	return true;
}

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