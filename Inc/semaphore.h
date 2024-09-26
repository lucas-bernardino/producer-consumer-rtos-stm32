/*
 * semaphore.h
 *
 *  Created on: Sep 12, 2024
 *      Author: lucasbernardino
 */

#ifndef SEMAPHORE_H_
#define SEMAPHORE_H_

typedef struct {
	int32_t sem_count;
} semaphore;

void sem_init(semaphore* s, int32_t init_value);

void sem_wait(semaphore* s);

void sem_post(semaphore* s);


#endif /* SEMAPHORE_H_ */
