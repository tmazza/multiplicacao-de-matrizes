#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/shm.h>
#include <sys/stat.h>
// 
#include "util.c"

#define BUFF_SIZE 4096

/**
 * Informações das matrizes in1 e in2 e da distribuição dos processos
 * são usadas pelos processos filhos acessando as cópias realiazidas pelo fork()
 * No final da execução os processos filhos salvam na memória compartilha com
 * segment_id igual ao seu número de processo 'myid' o reslutado do processamento da linha
 */
int main(int argc, char *argv[])
{
	init(argc, argv);

	printMatriz(in2_lin, in2_col, in2);

	printf("\n-------------------\n\n");

	// Uma região de memória compartilhada para cada processo
	int segment_id[num_procs]; // id do segmento do processo, acessado por myid no filhos
	char *shared_memory[num_procs];

	int status; // para uso em wait
	pid_t pid; // Controle encerrame
	int myid = 0;

	for(int i = 0; i < num_procs; i++) {

		segment_id[i] = shmget(IPC_PRIVATE, BUFF_SIZE, S_IRUSR | S_IWUSR);

		if(fork() == 0) {
			int from = procs_dist[myid][0];
			int to = procs_dist[myid][1];

			printf("proc:%d | id:%d | Linhas atribuidas: %d a %d\n", getpid(), myid, from, to);

			/* attach the shared memory segment */   
			shared_memory[myid] = (char *) shmat(segment_id[myid], NULL, 0);
			
			char *lineResult;
			lineResult = (char *) malloc( BUFF_SIZE );

			int len = 0; // tamanho da string gravada
			int result; // acumulador da linha-coluna

			for(int i = from; i <= to; i++) { // Linhas atribuidas ao processo
				for(int k = 0; k < in2_col; k++) { // Colunas em in2
					result = 0;
					for(int j = 0; j < in1_col; j++) { // in1_col == in2_lin
						result += in1[i][j] * in2[j][k];
					}
					len += sprintf( lineResult + len, "%d ", result);
				}
				if(i != to) // Envia quebra de linha extra
					len += sprintf( lineResult + len, "\n");
			}

			/** escreve na memória compartilhada **/
			strcat(shared_memory[myid], "\0");
			strcpy(shared_memory[myid], lineResult);

			/* detach the shared memory segment */    
			shmdt(shared_memory[myid]);

			sleep(1);
			exit(0);
		} else {
			myid++;
		}
	}

	// busy wainting esperando todos os processo terminarem
    while ((pid = wait(&status)) > 0) {
    	printf("%d: terminei \\o/ \n", (int) pid);
    }

    // Grava resultados
	FILE *file_out;
	file_out = fopen("./out.txt","w");
	if (!file_out) {
		printf("Erro ao abrir arquivo out.txt\n");
	    exit(1);
	}

	fprintf(file_out, "LINHAS = %d\nCOLUNAS = %d\n", in1_lin, in2_col);
	
	for(int i = 0; i < num_procs; i++) {
		shared_memory[i] = (char *) shmat(segment_id[i], NULL, 0);
		fprintf(file_out, "%s\n", shared_memory[i]);
		shmdt(shared_memory[i]);
	}

	fclose(file_out);

	// Libera memória alocada
	free(procs_dist);
	free(in1);
	free(in2);
	
	// Libera memórias compartilhadas
	for(int i = 0; i < num_procs; i++) {
		shmctl(segment_id[i], IPC_RMID, NULL);
	}

	return 0;
}
