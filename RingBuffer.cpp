#include "RingBuffer.h"
#include <stdlib.h>

#define _DEBUG

RingBuffer::RingBuffer(int size,int readMode,int writeMode)
:mSize(size)
,mReadMode(readMode)
,mWriteMode(writeMode)
{
    mData = (unsigned char *)malloc(mSize + 1);
    memset(mData, 0, mSize + 1);
    mReadIndex = 0;
    mWriteIndex = 0;
}

RingBuffer::~RingBuffer()
{
    free(mData);
}

int RingBuffer::getSize()
{
	return mSize;
}

void RingBuffer::setReadMode(int readMode)
{
	mReadMode = readMode;
}

void RingBuffer::setWriteMode(int writeMode)
{
	mWriteMode = writeMode;
}

int RingBuffer::getAvailable()
{
    return mSize - getUsed();
}

int RingBuffer::getUsed()
{
    if(mWriteIndex >= mReadIndex) {
       return mWriteIndex - mReadIndex; 
    }
    else {
        return mSize + 1 - mReadIndex + mWriteIndex;
    }
}

void RingBuffer::_write(unsigned char * src,int & pos,int size)
{
#ifdef _DEBUG
    std::cout<<"write "<<size;
#endif
	int chunkSize = mSize + 1 - mWriteIndex;
    if(size >= chunkSize) {
    	memcpy(mData + mWriteIndex,src + pos,chunkSize);
    	size -= chunkSize;
    	mWriteIndex = 0;
    	pos += chunkSize;
    }
    if(size > 0) {
    	memcpy(mData + mWriteIndex,src + pos,size);
    	mWriteIndex += size;
    	pos += size;
    }
    mConditionVariable.notify_all();

#ifdef _DEBUG
    dump<int>(std::cout);
#endif
}

int RingBuffer::write(unsigned char * src,int pos,int size)
{
    int writeSize;
    if(mWriteMode == WRITE_MODE_BLOCK) {
    	writeSize = size;
    	int available;
    	while(size > 0) {
     		std::unique_lock<mutex> ul(mMutex);
            while((available = getAvailable()) == 0) {
               	mConditionVariable.wait(ul);
            }
            available = available < size ? available : size;
            _write(src,pos,available);
            size -= available;
    	}
        
    }
    else if(mWriteMode == WRITE_MODE_NON_BLOCK) {
    	std::lock_guard<mutex> ul(mMutex);
    	int available = getAvailable();
    	available = available < size ? available : size;
    	writeSize = available;
    	_write(src,pos,available);
    }
    else {
     	if(mSize < size) {
    		pos += size - mSize;
    		size = mSize;
    	}
    	writeSize = size;
        {
            std::lock_guard<mutex> lg(mMutex);
            bool overWritted = getAvailable() < size;
            _write(src,pos,size);
            if(overWritted) {
                mReadIndex = (mWriteIndex + 1) % (mSize + 1);
            }
            
        } 	
    }

    return writeSize;
}

void RingBuffer::_read(unsigned char * dst,int & pos,int size)
{
#ifdef _DEBUG
    std::cout<<"read "<<size;
#endif
	int chunkSize = mSize + 1 - mReadIndex;
    if(size >= chunkSize) {
    	memcpy(dst + pos,mData + mReadIndex,chunkSize);
#ifdef _DEBUG
        memset(mData + mReadIndex, 0, chunkSize);
#endif
    	size -= chunkSize;
    	mReadIndex = 0;
    	pos += chunkSize;
    }
    if(size > 0) {
    	memcpy(dst + pos,mData + mReadIndex,size);
#ifdef _DEBUG
        memset(mData + mReadIndex, 0, size);
#endif
    	mReadIndex += size;
    	pos += size;
    }
    mConditionVariable.notify_all();
#ifdef _DEBUG
    dump<int>(std::cout);
#endif
}
int RingBuffer::read(unsigned char * dst,int pos,int size)
{
	int readSize;
    if(mReadMode == READ_MODE_BLOCK) {
    	readSize = size;
    	int used;
    	while(size > 0) {
			std::unique_lock<mutex> ul(mMutex);
			while((used = getUsed()) == 0) {
				mConditionVariable.wait(ul);
			}
			used = used < size ? used : size;
			_read(dst,pos,used);
            size -= used;
    	}
    }
    else {
    	std::lock_guard<mutex> lg(mMutex);
    	int used = getUsed();
    	used = used < size ? used : size;
    	readSize = used;
    	_read(dst,pos,used);
    }
    return readSize;
}
