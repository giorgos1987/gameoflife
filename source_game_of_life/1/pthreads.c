#include "common.h"

pthread_barrier_t barrier;

//Sinartisi twn nimatwn
void* main_loop(void* arg) {
	//Dinoume ws orisma ta oria pou tha koitaei kathe thread
	struct struct_limits* my_limits=(struct struct_limits*)arg;
	int i;
	
	int master=0;
	
	//To nima pou elegxei to panw aristera meros tou pinaka, einai to master thread
	if(my_limits->from_i==0 && my_limits->from_j==0) master=1;
	
	for(i=0;i<100;i++) {
		//Ekteloume ti next_step gia ta sigkekrimena oria tou pinaka
		next_step(my_limits);
		if(!BENCHMARK) print_before();
		
		//Kanoume barrier_wait prin to switch_arrays
		pthread_barrier_wait(&barrier);
		if(master) switch_arrays();
		
		//Kanoume barrier_wait prin ti nea ektelesi
		pthread_barrier_wait(&barrier);
	}
	
	return NULL;
}

int main(int argc, char** argv) {
	int k;
	
	init(argc, argv);
	
	//Arxikopoioume tis metavlites twn nimatwn kai to barrier
	pthread_t threads[THREADS];
	pthread_barrier_init(&barrier, NULL, THREADS);
	
	struct timeval start, stop;
	gettimeofday(&start, NULL);
	
	//Ftiaxnoume ta threads
	for(k=0;k<THREADS;k++) pthread_create(&threads[k], NULL, main_loop, (void*)&limits[k]);
	
	//Kanoume join se kathe thread kai perimenoume na termatisei
	for(k=0;k<THREADS;k++) pthread_join(threads[k],NULL);

	gettimeofday(&stop, NULL);
	
	printf("%f seconds\n", (stop.tv_sec-start.tv_sec)+((double)stop.tv_usec-start.tv_usec)/1000000);
	
	//Katastrefoume to barrier
	pthread_barrier_destroy(&barrier);
	
	return 0;
}
