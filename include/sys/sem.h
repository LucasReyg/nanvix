/*
 * Copyright(C) 2011-2016 Pedro H. Penna <pedrohenriquepenna@gmail.com>
 * 
 * This file is part of Nanvix.
 * 
 * Nanvix is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 * 
 * Nanvix is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with Nanvix. If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef SEM_H_
#define SEM_H_

#include <nanvix/pm.h>

	#define FIRST_SEM ((&semtab[0]))           /**< First semaphore. */
	#define LAST_SEM ((&semtab[PROC_MAX - 1])) /**< Last semaphore.  */

	struct procList{
		struct process proc;
		procList *next;
	};

	struct semaphore{
		int exists;
		int key;
		int counter;
		procList *list;
	};
	
	struct semaphore semtab[PROC_MAX];
	
	void init_sem();
	int create(int n, unsigned key);
	int down (int semid);
	int up(int semid);
	int destroy(int semid);

	/**
	 * @brief Comand values for semaphores.
	 */
	/**@{*/
	#define GETVAL   0 /**< Returns the value of a semaphore. */
	#define SETVAL   1 /**< Sets the value of a semaphore.    */
	#define IPC_RMID 3 /**< Destroys a semaphore.            */
	#define COUNT_INIT 0 // initial value of a counter when a semaphore is created without this information
	/**@}*/

	/* Forward definitions. */
	extern int semget(unsigned);
	extern int semctl(int, int, int);
	extern int semop(int, int);

#endif /* SEM_H_ */
