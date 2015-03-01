#include "common.h"


void next_step() {
	int i, j;
	int best_i=-1, best_j=-1, best=INFINITE;
	
	for(i=0;i<KORIFES;i++) {
		//An i korifi einai sto sinolo V, tote elegxoume ton pinaka geitniasis
		if(vertices[i]) {
			
			for(j=0;j<KORIFES;j++) {
				//An i akmi (i,j) iparxei idi sto sinolo E, tote sinexizoume
				if(vertices[j]) continue;
				
				//An to varos einai kalitero apo to best, tote kanoume tin ananewsi tou best
				if(matrix[i][j]
				   && matrix[i][j]<best) {
					   best_i=i;
					   best_j=j;
					   best=matrix[best_i][best_j];
				   }
			}
		}
	}
	
	//An best==INFINITE simainei oti de vrethike kamia akmi ston pinaka geitniasis
	if(best!=INFINITE) {
		g_best_i=best_i;
		g_best_j=best_j;
	}
}

int main(int argc, char** argv) {	
	init(argc, argv);
	
	struct timeval start, stop;
	gettimeofday(&start, NULL);
	
	while(1) {
		g_best_i=-1;
		next_step();
		
		//An de vrethike kapoia akmi tote termatizoume
		if(g_best_i==-1) break;
		
		//Alliws, diagrafoume tin akmi ap ton pinaka kai enimerwnoume to sinolo V
		matrix[g_best_i][g_best_j]=matrix[g_best_j][g_best_i]=0;
		vertices[g_best_j]=1;
		
		if(!BENCHMARK) print_matrix();
	}
	gettimeofday(&stop, NULL);
	printf("%f seconds\n", (stop.tv_sec-start.tv_sec)+((double)stop.tv_usec-start.tv_usec)/1000000);

	return 0;
}
