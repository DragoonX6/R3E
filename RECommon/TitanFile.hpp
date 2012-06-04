#ifndef TITAN_FILE_H
#define TITAN_FILE_H

#include "File.hpp"
#include "SafeDelete.hpp"

class VFSFile : public File {
public:
	VFSFile() :fh(0), mBuffer(0), mPosition(0), mSize(0) {}
	VFSFile(const char* path, const char* method)
		:fh(0), mBuffer(0), mPosition(0), mSize(0)
	{
		Open(path, method);
	}

	~VFSFile(){
		Close();
	}

	void SetData(unsigned char* buffer, long size){
		mPosition = 0;
		mSize = size;
		mBuffer = (unsigned char*)malloc(size);
		mBuffer = buffer;
	}

	bool Open(const char* path, const char* method)
	{
		fopen_s(&fh, path, method);
		if(!fh)
			return false;
		fseek(fh, 0, SEEK_END);
		mPosition = 0;
		mSize = ftell(fh);
		mBuffer = 0;
		fseek(fh, 0, SEEK_SET);
		return true;
	}

	bool VFSIsOpen()
	{
		return (fh != NULL);
	}

	bool IsOpen(){
		return (mBuffer != NULL);
	}

	void CloseVFS()
	{
		fclose(fh);
	}

	void Close()
	{
		SAFE_DELETE_ARRAY(mBuffer);
	}

	int ReadVFSData(void* data, int size)
	{
		return fread(data, size, 1, fh);
	}

	int ReadData(void* data, int size)
	{
		if(mPosition + size > mSize) size = mSize - mPosition;
		memcpy_s(data, size, mBuffer + mPosition, size);
		mPosition += size;
		return size;
	}

	int WriteData(void* data, int size){
		memcpy_s(mBuffer + mPosition, mSize - mPosition, data, size);
		return size;
	}

	int WriteData(void* data, int size, int pos){
		memcpy_s(mBuffer + pos, mSize - pos, data, size);
		return size;
	}

	void Skip(int bytes){
		mPosition += bytes;
	}

	void SeekVFS(int position)
	{
		fseek(fh, position, SEEK_SET);
	}

	void Seek(int position){
		mPosition = position;
	}

	long Position(){
		return mPosition;
	}

	long Size(){
		return mSize;
	}

	bool IsEOF(){
		return mPosition >= mSize;
	}

private:
	FILE *fh;
	long mSize;
	long mPosition;
	unsigned char* mBuffer;
};

#endif