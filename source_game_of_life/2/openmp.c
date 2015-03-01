#include "common.h"


int main(int argc, char** argv) {
	int i,j,k;
	init(argc, argv);
	
	struct timeval start, stop;
	gettimeofday(&start, NULL);
	
	while(1) {
		g_best_i=-1;
		
		//Ta i kai j einai private metavlites mesa sto block
		//enw ta g_best_i, g_best_j einai diamoirazomenes
#pragma omp parallel for private(i,j) shared(g_best_i, g_best_j) num_threads(THREADS)
		for(k=0;k<THREADS;k++) {
			struct st_data *my_data=&data[k];
			
			//Ta best_i, best_j einai topika veltista gia to sigkekrimeno nima
			int best_i=-1, best_j=-1, best=INFINITE;
			
			//Loop ston pinaka me tis korifes gia tis theseis tou kathe nimatos
			for(i=my_data->from_i;i<my_data->to_i;i++) {
				
				//An i korifi einai sto sinolo V, tote elegxoume ton pinaka geitniasis
				if(vertices[i]) {
					
					for(j=0;j<KORIFES;j++) {
						//An i akmi (i,j) iparxei idi sto sinolo E, tote sinexizoume
						if(vertices[j]) continue;
						
						//An to varos einai kalitero apo to topiko veltisto (best_i, best_j)
						//tote ananewnoume tis metavlites best
						if(matrix[i][j]
						   && matrix[i][j]<best) {
							   best_i=i;
							   best_j=j;
							   best=matrix[best_i][best_j];
						   }
					}
				}
			}
			
			
			//An vrethike kapoia akmi, tote elegxoume an prokeitai gia oliko veltisto
			if(best!=INFINITE) {
				
				//Mpainoume stin krisimi perioxi gia na sigkrinoume me to g_best_i,j
#pragma omp critical
				{
					//An iparxei alli akmi sti global metavliti, tote elegxoume
					//gia na doume poia einai kaliteri
					if(g_best_i!=-1) {
						//Se periptwsi pou kai oi dio akmes exoun idia vari
						//dinoume proteraiotita stin diastasi i kai meta sti j
						if(matrix[g_best_i][g_best_j]>best
						   || (matrix[g_best_i][g_best_j]==best && (g_best_i>best_i || (g_best_i==best_i && g_best_j>best_j)))) {
							   g_best_i=best_i;
							   g_best_j=best_j;
						   }
					} else {
						g_best_i=best_i;
						g_best_j=best_j;
					}
				}
			}
		}
		
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
