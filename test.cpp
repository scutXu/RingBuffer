#include <thread>
#include <iostream>
#include <assert.h>
#include "RingBuffer.h"

#define INPUT_BUFFER_SIZE 100
#define LOOP 100
#define OUTPUT_BUFFER_SIZE 150
#define RING_BUFFER_SIZE 2048

const int READ_MODE = RingBuffer::READ_MODE_BLOCK;
const int WRITE_MODE = RingBuffer::WRITE_MODE_BLOCK;

using std::thread;
using std::cout;
using std::endl;

RingBuffer * rBuf;

void producerFunc()
{
	int data[INPUT_BUFFER_SIZE];
    int num = 0;
	
    
    for(int i=0;i<LOOP;++i) {
        for(int i=0;i<INPUT_BUFFER_SIZE;++i) {
            data[i] = num ++;
        }
        rBuf->write((unsigned char *)data, 0, sizeof(data));
    }
    cout<<"done producer"<<endl;
}

void consumerFunc()
{
    int data[OUTPUT_BUFFER_SIZE];
    for(int i=0;i<OUTPUT_BUFFER_SIZE;++i) {
        data[i] = -1;
    }
    int totalSize = INPUT_BUFFER_SIZE * LOOP * sizeof(int);
    int totalReadSize = 0;
    
    int num = 0;
    while(totalReadSize < totalSize) {
        totalReadSize += rBuf->read((unsigned char *)data, 0, sizeof(data));
        for(int i=0;i<OUTPUT_BUFFER_SIZE;++i) {
            assert(data[i] == num);
            ++ num;
            //cout<<data[i]<<endl;
        }
    }
    cout<<"done consumer"<<endl;
}

int main()
{
    rBuf = new RingBuffer(RING_BUFFER_SIZE,READ_MODE,WRITE_MODE);
    
	thread producer(producerFunc);
	thread consumer(consumerFunc);

	producer.join();
	consumer.join();
    
    delete rBuf;
}
