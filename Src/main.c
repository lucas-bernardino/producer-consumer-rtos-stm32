#include <stdint.h>
#include "miros.h"
#include "semaphore.h"

uint32_t conta0=0, conta1=0, conta2=0;

uint32_t stack_blinky1[40];
OSThread blinky1;
void main_blinky1() {
    while (1) {
    	conta0++;
        OS_delay(TICKS_PER_SEC * 3U / 4U);
    }
}

uint32_t stack_blinky2[40];
OSThread blinky2;
void main_blinky2() {
    while (1) {
    	conta1++;
        OS_delay(TICKS_PER_SEC / 3U);
    }
}

uint32_t stack_blinky3[40];
OSThread blinky3;
void main_blinky3() {
    while (1) {
    	conta2++;
        OS_delay(TICKS_PER_SEC * 3U / 5U);
    }
}

uint32_t stack_idleThread[40];

// IMPLEMENTACAO

#define BUFFER_SIZE 10
uint8_t buffer_count = 0;
uint8_t buffer[BUFFER_SIZE];
semaphore* semEmpty;
semaphore* semFull;
semaphore* mutex;

uint32_t stack_producer[40];
OSThread producer_thread;
void producer() {
	while (1) {
		sem_wait(semEmpty);
		sem_wait(mutex);

		buffer[buffer_count] = buffer_count;
		// Printar o elemento produzido ? (buffer_count)
		buffer_count++;

		sem_post(mutex);
		sem_post(semFull);
	}
}

uint32_t stack_consumer[40];
OSThread consumer_thread;
void consumer() {
	while (1) {
		sem_wait(semFull);
		sem_wait(mutex);

		uint8_t x = buffer[buffer_count - 1];
		// Printar o elemento consumido ? (x)
		buffer_count--;

		sem_post(mutex);
		sem_post(semEmpty);
	}
}

// FIM IMPLEMENTACAO

int main() {
    OS_init(stack_idleThread, sizeof(stack_idleThread));

    /* start blinky1 thread */
    OSThread_start(&blinky1,
                   5U, /* priority */
                   &main_blinky1,
                   stack_blinky1, sizeof(stack_blinky1));

    /* start blinky2 thread */
    OSThread_start(&blinky2,
                   2U, /* priority */
                   &main_blinky2,
                   stack_blinky2, sizeof(stack_blinky2));

    /* start blinky3 thread */
    OSThread_start(&blinky3,
                   1U, /* priority */
                   &main_blinky3,
                   stack_blinky3, sizeof(stack_blinky3));

    // IMPLEMENTACAO
    sem_init(semEmpty, BUFFER_SIZE);
    sem_init(semFull, 0);
    sem_init(mutex, 1);

    OSThread_start(&producer_thread, 1U, &producer, stack_producer, sizeof(stack_producer));
    OSThread_start(&consumer_thread, 1U, &consumer, stack_consumer, sizeof(stack_consumer));

    // FIM IMPLEMENTACAO

    /* transfer control to the RTOS to run the threads */
    OS_run();

    //return 0;
}
