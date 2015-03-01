#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <math.h>
#include <time.h>
#include <sys/time.h>

//Orizoume tin anaparastasi twn stoixeiwn ston pinaka
#define DEAD 0
#define ALIVE 1

//Orizoume tin piknotita tou arxikou board
#define PIKNOTITA 2

//Pinakes before kai after gia tin apothikefsi tou board
char **before, **after;

//Dinoume default times stis global metavlites
int THREADS=4;
int BOARD_WIDTH=10;
int BOARD_HEIGHT=10;

int BENCHMARK=0;


//Domi gia ton kathorismo twn oriwn pou anatithetai se kathe nima
struct struct_limits {
	int from_i, to_i, from_j, to_j;
} *limits;


//Enallagi pinakwn before kai after
void switch_arrays() {
	char** temp;
	
	temp=before;
	before=after;
	after=temp;
}

//Epistrefei ton arithmo twn geitonikwn zwntanwn stoixeiwn
int count_alive(int i, int j) {
	int cnt=0;
	if(before[i][j-1]==ALIVE) cnt++;
	if(before[i][j+1]==ALIVE) cnt++;
	if(before[i-1][j]==ALIVE) cnt++;
	if(before[i+1][j]==ALIVE) cnt++;
	
	return cnt;
}


void next_step(struct struct_limits *my_limits) {
	int cnt;
	int i,j;
	
	//Loop apo from_i mexri to_i kai from_j mexri to_j
	for(i=my_limits->from_i+1;i<my_limits->to_i+1;i++) {
		for(j=my_limits->from_j+1;j<my_limits->to_j+1;j++) {
			cnt=count_alive(i,j);
			
			//Ilopoioume tous kanones tou game of life
			if(before[i][j]==ALIVE) {
				if(cnt<2 || cnt>3) after[i][j]=DEAD;
				else after[i][j]=before[i][j];
			} else {
				if(cnt==3) after[i][j]=ALIVE;
				else after[i][j]=before[i][j];
			}			
		}
	}
}

//Ektipwnei ton pinaka before
void print_before() {
	int i,j;
	
	printf("\n");
	for(i=0;i<BOARD_HEIGHT+2;i++) {
		for(j=0;j<BOARD_WIDTH+2;j++) {
			//Gia kathe simvolo ALIVE vazoume x enw gia DEAD vazoume _
			if(before[i][j]==ALIVE) printf("x ");
			else printf("_ ");
		}
		printf("\n");
	}
}

/*
Vriskoume tin kaliteri dinati diaspasi se disdiastato pinaka
me vasi ena prokathorismeno plithos apo threads

INPUT:
threads=arithmos nimatwn

OUTPUT:
r=rows
c=columns
*/
void find_best(int *r, int *c, int threads) {
	int i,j;
	
	//Arxikopoioume ta r kai c
	*r=threads;
	*c=1;
	
	//Sti metavliti best apothikevoume ti mikroteri diafora |r-c| pou vrikame
	int best=*r-*c;
	
	for(i=1;i<threads;i++) {
		for(j=i;j<threads;j++) {
			//An i*j isoutai me ton arithmo twn nimatwn tote elegxoume ti diafora |r-c|
			if(i*j==threads
			   && i-j<best) {
				   *r=j;
				   *c=i;
				   best=abs(*r-*c);
			   }
		}
	}
}

//Ektipwnei ta epitrepta orismata
void print_usage() {
	printf("-w INT (width)\n"
	       "-h INT (height)\n"
	       "-n INT (threads)\n"
	       "-b (benchmark mode)\n");
}

//Arxikopoiei to programma
void init(int argc, char** argv) {
	int n;
	
	//Arxikopoioume tis global metavlites
	while((n=getopt(argc, argv, "w:h:n:b")) != -1) {
		switch(n) {
		case 'n':
			THREADS=atoi(optarg);
			break;
		case 'w':
			BOARD_WIDTH=atoi(optarg);
			break;
		case 'h':
			BOARD_HEIGHT=atoi(optarg);
			break;
		case 'b':
			BENCHMARK=1;
			break;
		default:
			print_usage();
			exit(0);
			break;
		}
	}
	
	int i,j;
	int r, c;
	
	//Vriskoume ta r kai c
	find_best(&r, &c, THREADS);
	
	//Dimiourgoume toses domes limits osa kai ta threads
	limits=(struct struct_limits*)malloc(THREADS*sizeof(struct struct_limits));
	
	//Anathetoume times stis domes limits
	int k=0;
	for(i=0;i<r;i++) {
		for(j=0;j<c;j++) {
			//Pollaplasiazoume to deikti i me to ipsos tou board 
			//kai diairoume me ton arithmo twn rows
			limits[k].from_i=i*BOARD_HEIGHT/r;
			limits[k].to_i=(i+1)*BOARD_HEIGHT/r;
			
			limits[k].from_j=j*BOARD_WIDTH/c;
			limits[k].to_j=(j+1)*BOARD_WIDTH/c;
			k++;
		}
	}
	
	
	int total_size=(BOARD_WIDTH+2)*(BOARD_HEIGHT+2);
	
	//Desmevoume mnimi gia tous pinakes before kai after
	char *before_mem=(char*)malloc(total_size*sizeof(char));
	char *after_mem=(char*)malloc(total_size*sizeof(char));
	
	before=(char**)malloc((BOARD_HEIGHT+2)*sizeof(char*));
	after=(char**)malloc((BOARD_HEIGHT+2)*sizeof(char*));
	
	//Anathetoume times stous deiktes tis prwtis diastasis
	for(i=0;i<BOARD_HEIGHT+2;i++) {
		before[i]=&before_mem[i*(BOARD_WIDTH+2)];
		after[i]=&after_mem[i*(BOARD_WIDTH+2)];
	}
	
	//Arxikopoioume tixaia to board
	srand(time(0));
	for(i=0;i<BOARD_HEIGHT+2;i++) {
		for(j=0;j<BOARD_WIDTH+2;j++) {
			//Sta oria tou pinaka vazoume mono DEAD stoixeia
			//Sta ipoloipa dinoume tixaia timi ALIVE i DEAD vasi tis pithanotitas (rand()%PIKNOTITA!=0)
			if(i==0 || i==BOARD_HEIGHT+1 
			   || j==0 || j==BOARD_WIDTH+1
			   || (rand()%PIKNOTITA!=0)) before[i][j]=DEAD;
			else before[i][j]=ALIVE;
			
			//Arxika after=before
			after[i][j]=before[i][j];
		}
	}
}
