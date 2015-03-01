#include <pthread.h>
#include <stdio.h>
#include "my_rw.h"

int thevar=0;

my_rw_t the_rw_t;

//Sinartisi reader
void *rth(void *arg) {
	int id=*((int*)arg);
	printf("[R:%d]started\n", id);	
	
	int i, j=0;
	for(i=0;i<100000;i++) j++;
	
	my_rw_readlock(&the_rw_t);
	printf("[R:%d]var=%d\n", id, thevar);
	
	my_rw_readunlock(&the_rw_t);
	
	return NULL;
}

//Sinartisi writer
void *wth(void *arg) {
	int id=*((int*)arg);
	printf("[W:%d]started\n", id);	
	my_rw_writelock(&the_rw_t);
	
	int i;
	for(i=0;i<100000;i++) thevar++;
	printf("[W:%d]var=%d\n", id, thevar);
	
	my_rw_writeunlock(&the_rw_t);
	
	return NULL;
}


int main(int argc, char **argv)
{
	int i;
	pthread_t threadid[16];
	
	my_rw_init(&the_rw_t);
	
	int ids[16];
	for(i=0;i<16;i++) ids[i]=i;
	
	/*
	Dimiourgoume ta 16 nimata.
	To 5o kai to 11o nima einai writers
	*/
	
	for(i=0; i<4; ++i) {
		pthread_create(&threadid[i], NULL, rth, (void*)&ids[i]);
	}
	
	for(i=4; i<5; ++i) {
		pthread_create(&threadid[i], NULL, wth, (void*)&ids[i]);
	}
	
	for(i=5; i<10; ++i) {
		pthread_create(&threadid[i], NULL, rth, (void*)&ids[i]);
	}
	
	for(i=10; i<11; ++i) {
		pthread_create(&threadid[i], NULL, wth, (void*)&ids[i]);
	}
	
	for(i=11; i<16; ++i) {
		pthread_create(&threadid[i], NULL, rth, (void*)&ids[i]);
	}
	
	
	for (i=0; i<16; ++i) {
		pthread_join(threadid[i], NULL);
	}
	
	my_rw_destroy(&the_rw_t);
	
	return 0;
}
