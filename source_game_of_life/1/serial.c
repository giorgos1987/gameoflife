#include "common.h"

int main(int argc, char** argv) {
	int i,k;
	
	//Kaloume ti sinartisi arxikopoiisis
	init(argc, argv);
	
	struct timeval start, stop;
	
	//Ksekiname ti xronometrisi
	gettimeofday(&start, NULL);
	
	//Trexoume gia 100 epanalipseis
	for(i=0;i<100;i++) {
		//Ananewnoume seiriaka ta tmimata tou pinaka
		for(k=0;k<THREADS;k++) {
			next_step(&limits[k]);
		}
		
		if(!BENCHMARK) print_before();
		
		//before <-> after
		switch_arrays();
	}
	gettimeofday(&stop, NULL);
	
	//Ektipwnoume to apotelesma tis xronometrisis
	printf("%f seconds\n", (stop.tv_sec-start.tv_sec)+((double)stop.tv_usec-start.tv_usec)/1000000);
	
	return 0;
}
