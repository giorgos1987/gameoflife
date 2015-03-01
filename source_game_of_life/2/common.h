#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <math.h>
#include <time.h>
#include <sys/time.h>

//Orizoume tin piknotita tou arxikou grafou
#define PIKNOTITA 15

//Orizoume to megisto varos
#define MAX_WEIGHT 100

//Vazoume mia megali timi sto infinite (megaliteri ap to MAX_WEIGHT
#define INFINITE 9999

//I metavliti tou pinaka geitniasis
char **matrix;

/*
Monodiastatos pinakas gia to sinolo V
V[i]=0, an to i korifi i den anikei sto V
V[i]=1, an anikei
*/
char *vertices;

//global metavlites
int THREADS=4;
int KORIFES=0;
int BENCHMARK=0;

//xrisimopoioume tis global g_best_i kai g_best_j 
//gia tin apothikefsi tis kaliteris akmis se kathe vima ektelesis
int g_best_i, g_best_j;

//Domi gia na anathetoume ta oria prospelasis tou kathe nimatos ston pinaka vertices
struct st_data {
	int from_i, to_i;
} *data;

//Ektipwnei ton pinaka geitniasis kai ton vertices[]
void print_matrix() {
	int i,j;
	
	printf("\n");
	for(i=0;i<KORIFES;i++) {
		for(j=0;j<KORIFES;j++) {
			printf("%d ", matrix[i][j]);
		}
		printf("\n");
	}
	
	printf("\n");
	for(i=0;i<KORIFES;i++) {
		printf("%d ", vertices[i]);
	}
	printf("\n");
}

//Ektipwnei ta epitrepta orismata
void print_usage() {
	printf("-s INT (arithmos korifwn)\n"
	       "-n INT (threads)\n"
	       "-b (benchmark mode)\n");
}

//Arxikopoiei tous pinakes kai tis global metavlites
void init(int argc, char** argv) {
	int n;
	while((n=getopt(argc, argv, "n:s:b")) != -1) {
		switch(n) {
		case 'n':
			THREADS=atoi(optarg);
			break;
		case 's':
			KORIFES=atoi(optarg);
			break;		
		case 'b':
			BENCHMARK=1;
			break;
		default:
			print_usage();
			exit(0);
		}
	}
	
	int i,j;
	
	//desmevoume xwro gia tis domes oriwn
	data=(struct st_data*)malloc(THREADS*sizeof(struct st_data));
	
	int k=0;
	for(i=0;i<THREADS;i++) {
		//Kathorizoume ta oria tou kathe nimatos
		data[k].from_i=i*KORIFES/THREADS;
		data[k].to_i=(i+1)*KORIFES/THREADS;
		k++;
	}
	
	
	int total_size=KORIFES*KORIFES;
	
	//Desmevouem xwro gia tous pinakes
	char *matrix_mem=(char*)malloc(total_size*sizeof(char));
	memset(matrix_mem, 0x0, total_size);
	
	vertices=(char*)malloc(KORIFES*sizeof(char));
	
	//Arxika V[i]=0 gia kathe i
	memset(vertices, 0x0, KORIFES);
	
	matrix=(char**)malloc(KORIFES*sizeof(char*));
	
	//Anathetoume tous deiktes stin prwti diastasi tou pinaka geitniasis
	for(i=0;i<KORIFES;i++) {
		matrix[i]=&matrix_mem[i*KORIFES];
	}
	
	srand(time(0));
	
	//Kanoume 2 loops gia na gemisoume ton pinaka geitniasis me tixaia dedomena
	for(i=0;i<KORIFES;i++) {
		for(j=0;j<i;j++) {
			//Anathetoume akmes me vari ston pinaka geitniasis me tixaio tropo (rand()%MAX_WEIGHT)
			if(rand()%PIKNOTITA==0) matrix[i][j]=matrix[j][i]=rand()%MAX_WEIGHT;
		}
		
		//Vazoume texnita mia akmi apo to stoixeio i sto stoixeio i+1
		//gia na prokipsei sigoura sinektikos grafos
		//Gia na mi figoume ektos oriwn pinaka, anti gia i+1 vazoume (i+1)%KORIFES
		matrix[i][(i+1)%KORIFES]=matrix[(i+1)%KORIFES][i]=(rand()%(MAX_WEIGHT-1))+1;
	}
	
	//Vazoume to prwto stoixeio sto sinolo V
	vertices[rand()%KORIFES]=1;
}
