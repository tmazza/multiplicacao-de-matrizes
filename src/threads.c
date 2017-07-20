#include "util.c"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>


int **out;
int out_lin = 0, out_col = 0;

typedef struct linhas_da_thread {
    int from, to;
} Lines;

void *processa_linha (void *arg) {
	Lines *lines = (Lines *) arg;

	int from = lines->from;
	int to = lines->to;

    printf("Comecei  linhas: %d a %d\n", from, to);

    int result;
	for(int i = from; i <= to; i++) { 							// Linhas atribuidas ao processo
		for(int k = 0; k < in2_col; k++) { 						// Colunas em in2
			result = 0;
			for(int j = 0; j < in1_col; j++) { 					// in1_col == in2_lin
				result += in1[i][j] * in2[j][k];
			}
			out[i][k] = result;
		}
	}

    printf("Terminei linhas: %d a %d\n", from, to);
    pthread_exit(NULL);
}

void save_out() {
    // Grava resultados
    resultFileCreate();
	for(int i = 0; i < out_lin; i++) {
		for(int j = 0; j < out_col; j++) {
			fprintf(file_out, "%d ", out[i][j]);
		}
		fprintf(file_out, "\n");
	}
	resultFileClose();
}

int main( int argc, char **argv) {
  
	init(argc, argv);

    int status;
	pthread_t thr[num_procs]; // N threads
    Lines lines[num_procs]; // Linhas que threads i deve executar

	// Matriz para resultado acessa pelas threads filhas
	out_lin = in1_lin;
	out_col = in2_col;
	out = alocaMatrizInteiros(out_lin, out_col);

    // Cria n threads 
    for(int i = 0; i < num_procs; i++) {
		
    	lines[i].from = procs_dist[i][0];
    	lines[i].to = procs_dist[i][1];

	    status = pthread_create(&thr[i], NULL, processa_linha, &lines[i]);
	    if (status != 0) {
	    	printf("Erro ao gerar thread %d, responsavel pelas linhas: %d a %d\n", i, lines[i].from, lines[i].to);
	        exit(1);
	    }
    	
    }

	// Espera por todas as threads
	for(int i = 0; i < num_procs; i++) {
	    if (pthread_join(thr[i], NULL) != 0) {
	    	printf("Erro esprando thread %d\n", i);
	        exit(1);
	    }
    }

    // Grava resultados em arquivo
    save_out();

	// Libera memória alocada
	libera_memoria_alocada();
    free(out);
}

