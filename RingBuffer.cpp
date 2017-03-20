#include "RingBuffer.h"
#include <stdlib.h>

RingBuffer::RingBuffer(int size,int readMode,int writeMode)
:mSize(size)
,mReadMode(readMode)
,mWriteMode(writeMode)
{
    mData = (unsigned char *)malloc(mSize + 1);
}

RingBuffer::~RingBuffer()
{
    free(mData);
}

int RingBuffer::getAvailable()
{
    return mSize - getUsed();
}

int RingBuffer::getUsed()
{
    if(mWriteIndex > mReadIndex) {
        
    }
    else {
        
    }
}

void RingBuffer::_write(unsigned char * src,int & pos,int & size)
{
	int chunkSize = mSize - mWriteIndex;
    if(size >= chunkSize) {
    	memcpy(src + pos,mData + mWriteIndex,chunkSize);
    	size -= chunkSize;
    	mWriteIndex = 0;
    	pos += chunkSize;
    }
    if(size > 0) {
    	memcpy(src + pos,mData + mWriteIndex,size);
    	mWriteIndex += size;
    	pos += size;
    }
    mConditionVariable.notify();
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
    	}
        
    }
    else if(mWriteMode == WRITE_MODE_NON_BLOCK) {
    	std::unique_lock<mutex> ul(mMutex);
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

void RingBuffer::_read(unsigned char * dst,int & pos,int & size)
{
	int chunkSize = mSize - mReadIndex;
    if(size >= chunkSize) {
    	memcpy(mData + mReadIndex,dst + pos,chunkSize);
    	size -= chunkSize;
    	mReadIndex = 0;
    	pos += chunkSize;
    }
    if(size > 0) {
    	memcpy(mData + mReadIndex,dst + pos,size);
    	mReadIndex += size;
    	pos += size;
    }
    mConditionVariable.notify();	
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
				mConditionVariable.wait();
			}
			used = used < size ? used : size;
			
    	}
    }
    else {

    }
}
