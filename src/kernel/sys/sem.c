#include<sys/sem.h>
#include <nanvix/pm.h>

/**
 *  Initializing the semaphore table
 */
void seminit(){
	struct semaphore *sem;
	for (sem = FIRST_SEM; sem <= LAST_SEM; sem++){
		sem->exists = 0;
		sem->key = -1;
	}
}  
  /**
   *  Creates a semaphore and sets its key and counter
   *  the semaphore is created in the first free spot in the semaphore table
   */
int create(int n, unsigned key){
	struct semaphore *sem = FIRST_SEM;
	int semid = 0;

	while(sem->exists && sem <= LAST_SEM){
		semid++;
		sem++;
	}

	if(sem <= LAST_SEM){
		sem->key = key;
    sem->exists = 1;
		sem->counter = n;
		sem->list = NULL;
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
    sleep(&s->list, PRIO_USER);
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

	if (s->counter==0 && s->list != NULL)
	{
		wakeup(&s->list);
	}

	return 0;
}

/**
 *  destroy a semaphore
 */
int destroy(int semid){
	struct semaphore *sem;
	sem = (&semtab[semid]);

	sem->exists = 0;
	sem->key = -1;
  sem->counter = 0;
	wakeup(&sem->list);
	
	return 0;
}
