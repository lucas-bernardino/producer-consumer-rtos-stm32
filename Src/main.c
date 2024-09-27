#include <stdint.h>
#include "miros.h"
#include "semaphore.h"

#define BUFFER_SIZE 5

uint32_t volatile buffer_count = 0;
uint32_t volatile buffer[BUFFER_SIZE];

uint32_t stack_idleThread[40];
uint32_t stack_producer[40];
uint32_t stack_consumer[40];

semaphore semEmpty;
semaphore semFull;
semaphore mutex;

OSThread producer_thread;
OSThread consumer_thread;
uint32_t volatile consumed_value = 0;

void producer();
void consumer();

int main() {

    OS_init(stack_idleThread, sizeof(stack_idleThread));

    sem_init(&semEmpty, BUFFER_SIZE);
    sem_init(&semFull, 0);
    sem_init(&mutex, 1);

    OSThread_start(&producer_thread, 2U, &producer, stack_producer, sizeof(stack_producer));
    OSThread_start(&consumer_thread, 1U, &consumer, stack_consumer, sizeof(stack_consumer));

    OS_run();
}

void producer() {
	while (1) {
		OS_delay(TICKS_PER_SEC * 3U / 2U);
		sem_wait(&semEmpty);

		sem_wait(&mutex);

		buffer[buffer_count] = buffer_count;
		buffer_count++;
		sem_post(&mutex);

		sem_post(&semFull);

	}
}


void consumer() {
	while (1) {
		OS_delay(TICKS_PER_SEC * 3U);

		sem_wait(&semFull);

		sem_wait(&mutex);

		buffer_count--;
		consumed_value = buffer[buffer_count];
		buffer[buffer_count] = 0;
		OS_delay(TICKS_PER_SEC * 2U);
		sem_post(&mutex);

		sem_post(&semEmpty);

	}
}
