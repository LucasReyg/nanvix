#include<sys/sem.h>
#include <nanvix/pm.h>

/**
 *  Initializing the semaphore table
 */
void seminit(){
	struct semaphore *sem;
	for (sem = FIRST_SEM; sem <= LAST_SEM; s++){
		s->exists = 0;
		s->key = -1;
	}
  
  /**
   *  Creates a semaphore and sets its key and counter
   *  the semaphore is created in the first free spot in the semaphore table
   */
int create(int n, unsigned key){
	struct semaphore *sem = FIRST_SEM;
	int semid = 0;

	while(s->exists && s <= LAST_SEM){
		semid++;
		s++;
	}

	if(s <= LAST_SEM){
		s->key = key;
    s->exists = 1;
		s->counter = n;
		s->procList = NULL;
		return semid;
	}
	else{
		return -1;
	}
}

/**
 *  if the semaphore counter is above 0 it is decremented,
 * otherwise the process is blocked
 */
int down (int semid){
	struct semaphore *s;
	s = (&semtab[semid]);
  
	if(s->counter > 0){
    s->counter--;
	}
  else{
    sleep(&s->queue, PRIO_SEM);
  }

	return 0;
}

/**
 *  if the semaphore counter is 0 and a process is waiting, it is waken up
 * otherwise the counter is incremented
 */
int up(int semid){
	struct semaphore *s;
	s = (&semtab[semid]);

	if (s->counter==0 && s->queue != NULL)
	{
		wakeup(&s->queue);
	}

	return 0;
}

/**
 *  destroy a semaphore
 */
int destroy(int semid){
	struct semaphore *s;
	s = (&semtab[semid]);

	s->exists = 0;
	s->key = -1;
  s->counter = 0;
	wakeup(&s->queue);
	
	return 0;
}
  
}
