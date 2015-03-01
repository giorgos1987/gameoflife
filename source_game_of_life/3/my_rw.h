#ifndef MY_RW
#define MY_RW

#include <pthread.h>

/*
I domi my_rw_t periexei oles tis aparaitites metavlites pou xreiazomaste 
gia ta read/write blocks
*/
typedef struct my_rw_t {
	pthread_cond_t cond;
	pthread_mutex_t mutex;
	pthread_mutex_t c_mutex;
} my_rw_t;

void my_rw_init(my_rw_t* lockvar);
void my_rw_readlock(my_rw_t* lockvar);
void my_rw_writelock(my_rw_t* lockvar);
void my_rw_readunlock(my_rw_t* lockvar);
void my_rw_writeunlock(my_rw_t* lockvar);
void my_rw_destroy(my_rw_t* lockvar);

#endif
