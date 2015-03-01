CC = gcc
CFLAGS = -O3 -Wall
LIBS = -lm -lpthread
OMPFLAGS = -fopenmp

all: target1 target2 target3

target1: 1/serial.c 1/openmp.c 1/pthreads.c
	$(CC) $(CFLAGS) 1/serial.c -o serial1.out $(LIBS)
	$(CC) $(CFLAGS) 1/openmp.c -o openmp1.out $(LIBS) $(OMPFLAGS) 
	$(CC) $(CFLAGS) 1/pthreads.c -o pthreads1.out $(LIBS)

target2: 2/serial.c 2/openmp.c 2/pthreads.c
	$(CC) $(CFLAGS) 2/serial.c -o serial2.out $(LIBS)
	$(CC) $(CFLAGS) 2/openmp.c -o openmp2.out $(LIBS) $(OMPFLAGS) 
	$(CC) $(CFLAGS) 2/pthreads.c -o pthreads2.out $(LIBS)

target3_lib: 3/my_rw.c
	$(CC) $(CFLAGS) 3/my_rw.c -c
	
target3: target3_lib my_rw.o 3/test.c
	$(CC) $(CFLAGS) 3/test.c my_rw.o -o 3.out $(LIBS)

clean:
	rm -f *.o *.out
