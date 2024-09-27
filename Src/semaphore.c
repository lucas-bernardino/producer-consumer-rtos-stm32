/*
 * semaphore.c
 *
 *  Created on: Sep 12, 2024
 *      Author: lucasbernardino
 */

#include <stdbool.h>
#include <stdint.h>
#include <cmsis_gcc.h>
#include "semaphore.h"
#include "miros.h"

void sem_init(semaphore* s, int32_t init_value) {
	s->sem_count = init_value;
	s->isBlocked = false;
}

void sem_wait(semaphore* s) {
	__disable_irq();
	if (s->sem_count == 0) {
		s->isBlocked = true;
	}
	while (s->sem_count == 0) {
		OS_delay(1);
		OS_sched();
		if (s->isBlocked == false) {
			break;
		}
	}
	s->sem_count--;
	OS_sched();
}

void sem_post(semaphore* s) {
	__disable_irq();
	s->sem_count++;
	if (s->isBlocked == true) {
		s->isBlocked = false;
	}
	__enable_irq();
	OS_sched();
}
