/*
 * semaphore.c
 *
 *  Created on: Sep 12, 2024
 *      Author: lucasbernardino
 */

#include <stdint.h>
#include <cmsis_gcc.h>
#include "semaphore.h"
#include "miros.h"

void sem_init(semaphore* s, int32_t init_value) {
	s->sem_count = init_value;
}

void sem_wait(semaphore* s) {
	__disable_irq();
	while (s->sem_count == 0) {
		OS_delay(1U);
	}
	s->sem_count--;
	OS_sched();
	__enable_irq();
}

void sem_post(semaphore* s) {
	__disable_irq();
	s->sem_count++;
	OS_sched();
	__enable_irq();
}
