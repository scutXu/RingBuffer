#include <thread>
#include "RingBuffer.h"

#define BUFFER_SIZE 100

using std::thread;

RingBuffer rBuf;

void producerFunc()
{
	int data[BUFFER_SIZE];
	for(int i=0;i<BUFFER_SIZE;++i) {
		
	}
}

void consumerFunc()
{

}

int main()
{
	thread producer(producerFunc);
	thread consumer(consumerFunc);

	producer.start();
	consumer.start();


	producer.join();
	consumer.join();
}