#include "common.h"

int main(int argc, char** argv) {
	int i,k;
	
	init(argc, argv);
	
	struct timeval start, stop;
	gettimeofday(&start, NULL);
	
	for(i=0;i<100;i++) {
		
	//Parallilopoioume to loop
#pragma omp parallel for num_threads(THREADS)
		for(k=0;k<THREADS;k++) {
			next_step(&limits[k]);
		}
		
		if(!BENCHMARK) print_before();
		switch_arrays();
	}
	gettimeofday(&stop, NULL);
	
	printf("%f\n", (stop.tv_sec-start.tv_sec)+((double)stop.tv_usec-start.tv_usec)/1000000);
	
	return 0;
}
