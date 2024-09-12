/*
 * semaphore.c
 *
 *  Created on: Sep 12, 2024
 *      Author: lucasbernardino
 */

#include <stdint.h>
#include "semaphore.h"


void sem_init(semaphore* s, uint8_t init_value) {
	s->sem_count = init_value;
}

void sem_wait(semaphore* s) {
	s->sem_count--;
}

void sem_post(semaphore* s) {
	s->sem_count++;
}
