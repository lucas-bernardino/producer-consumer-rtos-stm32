/*
 * semaphore.c
 *
 *  Created on: Sep 12, 2024
 *      Author: lucasbernardino
 */

#include <stdbool.h>
#include <stdint.h>
#include <cmsis_gcc.h>
#include "qassert.h"
#include "miros.h"
#include "semaphore.h"

Q_DEFINE_THIS_FILE

void sem_init(semaphore* s, int32_t initValue) {
	Q_ASSERT(s);
	s->semCount = initValue;
	s->isBlocked = false;
}

void sem_wait(semaphore* s) {
	Q_ASSERT(s);
	__disable_irq();
	if (s->semCount == 0) {
		s->isBlocked = true;
	}
	while (s->semCount == 0) {
		OS_delay(1);
		OS_sched();
		if (s->isBlocked == false) {
			break;
		}
	}
	s->semCount--;
	OS_sched();
}

void sem_post(semaphore* s) {
	Q_ASSERT(s);
	__disable_irq();
	s->semCount++;
	if (s->isBlocked == true) {
		s->isBlocked = false;
	}
	__enable_irq();
	OS_sched();
}
