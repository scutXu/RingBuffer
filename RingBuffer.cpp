#include "RingBuffer.h"
#include <stdlib.h>

RingBuffer::RingBuffer(int size,int readMode,int writeMode)
:mSize(size)
,mReadMode(readMode)
,mWriteMode(writeMode)
{
    /*保留一个位用于区分空（writeIndex = readIndex）和满(writeIndex + 1 = readIndex）的情况*/
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

int RingBuffer::write(unsigned char * src,int pos,int size)
{
    if(mSize < size) {
        pos += size - mSize;
        size = mSize;
    }
    
    int writeIndex = mWriteIndex;
    
    if(mWriteMode == WRITE_MODE_BLOCK) {
        {
            std::unique_lock<mutex> ul(mMutex);
            while(getAvailable() < size) {
                mConditionVariable.wait(ul);
            }
            
        }
        
    }
    else {
        {
            std::lock_guard<mutex> lg(mMutex);
            mWriteIndex = (mWriteIndex + size) % (mSize + 1);
            if(getAvailable() < size) {
                mReadIndex = (mWriteIndex + 1) % (mSize + 1);
            }
            
        }
    }
    
    memcpy(src + pos, mData + writeIndex, size);
}
int RingBuffer::read(unsigned char * dst,int pos,int size)
{
    
}
