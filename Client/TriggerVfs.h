/*
    Open source TriggerVFS.dll
    Copyright (C) Guegant Jean
    
    This program is free software; you can redistribute it and/or
    modify it under the terms of the GNU General Public License
    as published by the Free Software Foundation; either version 2
    of the License, or (at your option) any later version.
    
    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.
    
    You should have received a copy of the GNU General Public License
    along with this program; if not, write to the Free Software
    Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.

    depeloped with with a hash table you can found at : http://www.flipcode.com/archives/Hash_Table.shtml    
*/
#ifndef _TRIGGERVFS_H_
#define _TRIGGERVFS_H_
#include "CDATA.h"
#include "Log.h"
namespace TriggerVfs
{
#define DllExport   extern "C" __declspec( dllexport )
	typedef unsigned long INDEXHANDLE;
	typedef unsigned long FILEHANDLE;
	DllExport  unsigned long _stdcall VClearBlankAll(unsigned int unknow1 , unsigned int unknow2){return 0;}
	DllExport  unsigned long _stdcall vfeof(unsigned int unknow){return 0;}
	DllExport  unsigned long _stdcall VGetFileLength(unsigned int unknow1 , unsigned int unknow2){return 0;}
	DllExport  unsigned long _stdcall VSetFileInfo(unsigned int unknow1 , unsigned int unknow2, unsigned int unknow3){return 0;}
	DllExport  unsigned long _stdcall VTestFile(unsigned int unknow1 , unsigned int unknow2){return 0;}

	DllExport  CData* _stdcall OpenVFS(const char* idxfile, const char* mode);
	DllExport  unsigned long _stdcall CloseVFS(CData *data);
	DllExport  unsigned long _stdcall VGetVfsCount(CData* handle);
	DllExport  void _stdcall VGetVfsNames(CData* handle, char** names, unsigned long maxcount, unsigned long maxlen);
	DllExport  unsigned long _stdcall VGetFileCount(CData* handle, const char* vfsfile);
	DllExport  void _stdcall VGetFileNames(CData* handle, const char* name, char** files, unsigned long maxfiles, unsigned long maxlen);
	DllExport  void _stdcall VGetFileInfo(INDEXHANDLE handle, const char* filename, char* info, unsigned long somehandle);
	DllExport  unsigned long _stdcall VAddFile(INDEXHANDLE handle, const char* insertvfs, const char* localpath, const char* vfspath, unsigned long attribute, unsigned long checksum, unsigned long reserved, unsigned long reserved2, unsigned long reserved3);
	DllExport  bool _stdcall VFileExists(CData* handle, const char* filename);
	DllExport  bool _stdcall VFileExistsInVfs(INDEXHANDLE handle, const char* filename);
	DllExport  unsigned long _stdcall VRemoveFile(CData* handle, const char* filename);
	DllExport  unsigned short _stdcall VGetError();
	DllExport  unsigned long _stdcall VGetTotFileCount(INDEXHANDLE handle);
	DllExport  unsigned long _stdcall VGetCurVersion(CData* handle);
	DllExport  unsigned long _stdcall VSetCurVersion(INDEXHANDLE handle, unsigned long newver);
	DllExport  unsigned long _stdcall VGetStdVersion(INDEXHANDLE handle);
	DllExport  unsigned long _stdcall VSetStdVersion(INDEXHANDLE handle, unsigned long newver);
	DllExport  bool _stdcall VAddVfs(INDEXHANDLE handle, const char* vfsname);
	DllExport CVFSFile::File* _stdcall VOpenFile(const char* path, CData* handle);
	DllExport  void _stdcall VCloseFile(CVFSFile::File* file);
	DllExport  unsigned long _stdcall vfgetsize(CVFSFile::File* file);
	DllExport  unsigned long _stdcall vfread(void* buffer, unsigned long size, unsigned long count, CVFSFile::File* file);
	DllExport  unsigned long  _stdcall vfseek(CVFSFile::File* file, unsigned long offset, unsigned long origin);
	DllExport  unsigned long _stdcall vfgetdata(CVFSFile::File* file, char* buffer);
	DllExport  unsigned long _stdcall vftell(CVFSFile::File* file);
	DllExport  unsigned long _stdcall ComputeCrc(INDEXHANDLE handle, const char* filename);
}
#endif