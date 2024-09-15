/*
 * semaphore.c
 *
 *  Created on: Sep 12, 2024
 *      Author: lucasbernardino
 */

#include <stdint.h>
#include <cmsis_gcc.h>
#include "semaphore.h"

void sem_init(semaphore* s, uint8_t init_value) {
	s->sem_count = init_value;
}

void sem_wait(semaphore* s) {
	// precisa ter um "mutex lock"
	__disable_irq();
	while (s->sem_count <= 0) {
		// Precisa achar um jeito de ficar esperando até ele for maior que zero ?
	}
	s->sem_count--;
	__enable_irq();
	// precisa ter um "mutex unlock"
}

void sem_post(semaphore* s) {
	// precisa ter um "mutex lock"
	__disable_irq();
	s->sem_count++;
	// precisa ter um "mutex unlock"
	__enable_irq();
}
