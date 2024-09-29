#include <stdint.h>
#include "miros.h"
#include "semaphore.h"

#define BUFFER_SIZE 5

uint32_t volatile bufferCount = 0; // This variable counts how many elements are present in the buffer
uint32_t volatile consumedValue = 0; // This value holds the last value consumed by the consumer function
uint32_t buffer[BUFFER_SIZE];

uint32_t stackIdleThread[40];
uint32_t stackProducer[40];
uint32_t stackConsumer[40];

semaphore semEmpty;
semaphore semFull;
semaphore mutex;

OSThread producerThread;
OSThread consumerThread;

/*
 * Since the producer function is quicker than the consumer function, the buffer will be filled up and
 * after that, the last element will be changed.
 * Notice that the consumer function puts a zero after it has consumed the element. You can check it in the debugger.
 * The consumed element can be seen with the consumedValue variable.
*/

void producer();
void consumer();

int main() {

    OS_init(stackIdleThread, sizeof(stackIdleThread));

    sem_init(&semEmpty, BUFFER_SIZE);
    sem_init(&semFull, 0);
    sem_init(&mutex, 1);

    OSThread_start(&producerThread, 2U, &producer, stackProducer, sizeof(stackProducer));
    OSThread_start(&consumerThread, 1U, &consumer, stackConsumer, sizeof(stackConsumer));

    OS_run();
}


void producer() {
	while (1) {
		OS_delay(TICKS_PER_SEC * 3U / 2U);
		sem_wait(&semEmpty);

		sem_wait(&mutex);

		buffer[bufferCount] = bufferCount;
		bufferCount++;
		sem_post(&mutex);

		sem_post(&semFull);

	}
}


void consumer() {
	while (1) {
		OS_delay(TICKS_PER_SEC * 3U);

		sem_wait(&semFull);

		sem_wait(&mutex);

		bufferCount--;
		consumedValue  = buffer[bufferCount];
		buffer[bufferCount] = 0;
		OS_delay(TICKS_PER_SEC * 2U);
		sem_post(&mutex);

		sem_post(&semEmpty);

	}
}
