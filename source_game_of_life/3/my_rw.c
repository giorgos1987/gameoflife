#include "my_rw.h"
#include <stdio.h>

//Apothikevoume ton arithmo twn diergasiwn pou theloun write lock
//simperilamvanomenis kai ekeinis pou pithanon exei to write lock
int writers;

//Apothikevoume ton arithmo twn diergasiwn pou exoun to read lock
int readers;


void my_rw_init(my_rw_t* lockvar) {
	//Arxikopoioume to condition variable kai ta mutex tou lockvar
	pthread_cond_init(&lockvar->cond, NULL);
	pthread_mutex_init(&lockvar->c_mutex, NULL);
	pthread_mutex_init(&lockvar->mutex, NULL);
	
	writers=0;
	readers=0;
}

void my_rw_readlock(my_rw_t* lockvar) {
	//Kleidwnoume to c_mutex gia na exoume prosvasi sti metavliti writers
	pthread_mutex_lock(&lockvar->c_mutex);
	
	while (writers!=0) {
		//perimenoume mexri na apodesmefsei to lock o writer
		pthread_cond_wait(&lockvar->cond, &lockvar->c_mutex);
	}
	
	//Gnwrizoume oti den iparxei writer
	//opote kanoume trylock
	//diladi an den iparxei reader ginetai lock
	//enw an iparxoun readers apla sinexizei i ektelesi
	pthread_mutex_trylock(&lockvar->mutex);
	
	//Afksanoume ton metriti twn readers
	readers++;
	printf("[my_rw lib]readers: %d\n", readers);
	
	pthread_mutex_unlock(&lockvar->c_mutex);
}


void my_rw_writelock(my_rw_t* lockvar) {
	//Afksanoume apo twra ton arithmo twn writers
	//giati theloume na exoun proteraiotita ta write locks
	pthread_mutex_lock(&lockvar->c_mutex);
	writers++;
	pthread_mutex_unlock(&lockvar->c_mutex);
	
	//Kanoume lock to mutex.
	//An iparxoun kai alloi writers tha perimenoume
	pthread_mutex_lock(&lockvar->mutex);
}

void my_rw_readunlock(my_rw_t* lockvar) {
	pthread_mutex_lock(&lockvar->c_mutex);
	
	//Meiwnoume tous readers
	readers--;
	printf("[my_rw lib]readers: %d\n", readers);
	
	//An den iparxoun alloi readers kanoume unlock to mutex
	if(readers==0) {
		pthread_mutex_unlock(&lockvar->mutex);
	}
	
	pthread_mutex_unlock(&lockvar->c_mutex);
}


void my_rw_writeunlock(my_rw_t* lockvar) {
	pthread_mutex_lock(&lockvar->c_mutex);
	
	//Meiwnoume tous writers
	writers--;
	
	//Apeleftherwnoume to mutex
	pthread_mutex_unlock(&lockvar->mutex);
	
	//An den iparxei allos writer kanoume broadcast to condition variable
	if(writers==0) {
		pthread_cond_broadcast(&lockvar->cond);
	}
	
	pthread_mutex_unlock(&lockvar->c_mutex);
}


void my_rw_destroy(my_rw_t* lockvar) {
	//Katastrefoume to condition variable kai ta mutex
	pthread_cond_destroy(&lockvar->cond);
	pthread_mutex_destroy(&lockvar->c_mutex);
	pthread_mutex_destroy(&lockvar->mutex);
}

