#include "util.c"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>


typedef struct linhas_da_thread {
    int from, to;
} Lines;

void *processa_linha (void *arg) {
	Lines *thread = (Lines *) arg;

    printf("FROM: %d \n", thread->from);
    printf("TO: %d \n", thread->to);

    pthread_exit(NULL);
}

int main( int argc, char **argv) {
    
	init(argc, argv);

    pthread_t thr;
    pthread_t thr2;
    double *res;
    int status;
    int myid = 0;
    Lines thread[2];

    thread[0].from = 0;
    thread[0].to = 1;

    thread[1].from = 10;
    thread[1].to = 20;

    status = pthread_create(&thr, NULL, processa_linha, &thread[0]);
    if (status != 0) {
    	printf("ERRRRRRRRRRRO\n");
        exit(1);
    }

    sleep(1);
    myid++;

    status = pthread_create(&thr2, NULL, processa_linha, &thread[1]);
    if (status != 0) {
    	printf("ERRRRRRRRRRRO\n");
        exit(1);
    }

    printf("here!!\n");

    while ((status = pthread_join(thr, NULL)) != 0) {
    	printf("JOIN - ERRRRRRRRRRRO\n");
        exit(1);
    }
    printf("ENTE!!!!here!!\n");
    // free(res);
}
