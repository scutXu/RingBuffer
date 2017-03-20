#include "RingBuffer.h"
#include <stdlib.h>

RingBuffer::RingBuffer(int size,int readMode,int writeMode)
:mSize(size)
,mReadMode(readMode)
,mWriteMode(writeMode)
{
    //保留一个位用于区分空（writeIndex = readIndex）和满(writeIndex + 1 = readIndex）的情况
    mData = malloc(mSize + 1);
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

int RingBuffer::write(void * src,int pos,int size)
{
    int writeIndex;
    int writeSize;
    if(mWriteMode == WRITE_MODE_BLOCK) {
        if(mSize <= size) {
            
        }
        std::unique_lock<mutex> ul(mMutex);
    }
    else {
        {
            std::lock_guard<mutex> lg(mMutex);
            if(mSize < size) {
                
            }
            else if(getAvailable() < size){
                
            }
            else {
                
            }
        }
        
        
    }
}
int RingBuffer::read(void * dst,int pos,int size)
{
    
}
