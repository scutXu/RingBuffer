#ifndef RingBuffer_h
#define RingBuffer_h

#include <condition_variable>
#include <mutex>

using std::mutex;
using std::condition_variable;

#define DEFAULT_RING_BUFFER_SIZE 4096

class RingBuffer
{
public:
    enum WriteMode {
        WRITE_MODE_BLOCK,
        WRITE_MODE_NON_BLOCK,
        WRITE_MODE_OVER_WRITE
    };
    enum ReadMode {
        READ_MODE_BLOCK,
        READ_MODE_NON_BLOCK
    };
    
    RingBuffer(int size = DEFAULT_RING_BUFFER_SIZE,int readMode = READ_MODE_BLOCK,int writeMode = WRITE_MODE_OVER_WRITE);
    ~RingBuffer();
    
    int getSize();
    
    int write(unsigned char * src,int pos,int size);
    int read(unsigned char * dst,int pos,int size);
    
    void setReadMode(int readMode);
    void setWriteMode(int writeMode);
    
private:
    int getUsed();
    int getAvailable();
    void _write(unsigned char * src,int & pos,int size);
    void _read(unsigned char * dst,int & pos,int size);
    
    unsigned char * mData;
    int mSize;
    int mReadMode;
    int mWriteMode;
    int mWriteIndex;
    int mReadIndex;
    
    condition_variable mConditionVariable;
    mutex mMutex;
};

#endif /* RingBuffer_h */
