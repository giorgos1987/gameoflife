#include "common.h"

//Orizoume to barrier kai to mutex
pthread_barrier_t barrier;
pthread_mutex_t mutex=PTHREAD_MUTEX_INITIALIZER;

//Sinartisi tou nimatos
void* next_step(void* arg) {
	int i, j;
	//Pairnoume ws orisma ta oria ston pinaka vertices gia to sigkekrimeno nima
	struct st_data* my_data=(struct st_data*)arg;
	
	int master=0;
	if(my_data->from_i==0) master=1;
	
	while(1) {
		//Arxikopoioume ta topika veltista
		int best_i=-1, best_j=-1, best=INFINITE;
		if(master) g_best_i=-1;
		
		//Sigxronizoume etsi na arxisoun ola ta nimata to for loop
		pthread_barrier_wait(&barrier);
		
		//Loop sta oria pou orizei i domi my_data
		for(i=my_data->from_i;i<my_data->to_i;i++) {
			//An i korifi einai sto sinolo V, tote elegxoume ton pinaka geitniasis
			if(vertices[i]) {
				
				for(j=0;j<KORIFES;j++) {
					//An i akmi (i,j) iparxei idi sto sinolo E, tote sinexizoume
					if(vertices[j]) continue;
					
					if(matrix[i][j]
					   && matrix[i][j]<best) {
						   //Enimerwnoume to topiko veltisto
						   best_i=i;
						   best_j=j;
						   best=matrix[best_i][best_j];
					   }
				}
			}
		}
		
		
		
		if(best!=INFINITE) {
			//Kleidwnoume to mutex gia na diavasoume kai na enimerwsoume ti global best
			pthread_mutex_lock(&mutex);
			
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
			
			//Ksekleidwnoume to mutex
			pthread_mutex_unlock(&mutex);
		}
	
		//Perimenoume me barrier gia na exoun ola ta threads tin idia timi g_best_i
		pthread_barrier_wait(&barrier);
		
		//An de vrethike kapoia akmi termatizoume
		if(g_best_i==-1) {
			return NULL;
		}
		
		//Mono to master thread tha enimerwsei tous  pinakes
		if(master) {
			matrix[g_best_i][g_best_j]=matrix[g_best_j][g_best_i]=0;
			vertices[g_best_j]=1;
			
			if(!BENCHMARK) print_matrix();
		}
		
		//Perimenoume me barrier_wait gia na vevaiwthoume oti ola ta nimata diavasan to oliko veltisto g_best_i
		pthread_barrier_wait(&barrier);
	}
	
	return NULL;
	
}


int main(int argc, char** argv) {
	int k;
	
	init(argc, argv);
	
	//Dimiourgoume tis metavlites gia ta threads
	pthread_t threads[THREADS];
	pthread_barrier_init(&barrier, NULL, THREADS);
	
	struct timeval start, stop;
	gettimeofday(&start, NULL);
	
	//Dimiourgoume ta nimata
	for(k=0;k<THREADS;k++) pthread_create(&threads[k], NULL, next_step, (void*)&data[k]);
	
	//Kanoume join gia na perimenoume na termatisoun ola
	for(k=0;k<THREADS;k++) pthread_join(threads[k],NULL);
	
	gettimeofday(&stop, NULL);
	printf("%f seconds\n", (stop.tv_sec-start.tv_sec)+((double)stop.tv_usec-start.tv_usec)/1000000);
	
	return 0;
}
