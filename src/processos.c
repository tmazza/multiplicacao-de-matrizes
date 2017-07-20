#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include "util.c"

#define BUFF_SIZE 4096

/**
 * Processo filho criado.
 * Processa as linhas de  procs_dist[myid][0] até procs_dist[myid][1]
 * definidas durante a distribuição das linhas
 */
void processa_linhas(int myid, int segment_id, char *shared_memory) {
	int from = procs_dist[myid][0];
	int to = procs_dist[myid][1];
	char *lineResult; 											// Concatenação de resultados a serem gravados na shared memory
	int len = 0; 												// tamanho da string gravada
	int result; 												// acumulador da linha-coluna

	printf("proc:%d | id:%d | Linhas atribuidas: %d a %d\n", getpid(), myid, from, to);

	shared_memory = (char *) shmat(segment_id, NULL, 0);
	
	lineResult = (char *) malloc( BUFF_SIZE );

	for(int i = from; i <= to; i++) { 							// Linhas atribuidas ao processo
		for(int k = 0; k < in2_col; k++) { 						// Colunas em in2
			result = 0;
			for(int j = 0; j < in1_col; j++) { 					// in1_col == in2_lin
				result += in1[i][j] * in2[j][k];
			}
			len += sprintf( lineResult + len, "%d ", result);
		}
		if(i != to){											// Enviata quebra de linha extra
			len += sprintf( lineResult + len, "\n");
		}
	}

	strcat(shared_memory, "\0");
	strcpy(shared_memory, lineResult);							// Grava resultados na memória compartilhada

	shmdt(shared_memory);

	sleep(1); // TODO: REMOVER!!!!
	exit(0);
}

/**
 * Informações das matrizes in1 e in2 e da distribuição dos processos
 * são usadas pelos processos filhos acessando as cópias realiazidas pelo fork()
 * No final da execução os processos filhos salvam na memória compartilha com
 * segment_id igual ao seu número de processo 'myid' o reslutado do processamento da linha
 */
int main(int argc, char *argv[])
{
	init(argc, argv);

	// Uma região de memória compartilhada para cada processo
	int segment_id[num_procs]; 
	char *shared_memory[num_procs];

	int status; 												// para uso em wait
	pid_t pid; 													// Controle encerramento
	int myid = 0;

	for(int i = 0; i < num_procs; i++) {
		segment_id[i] = shmget(IPC_PRIVATE, BUFF_SIZE, S_IRUSR | S_IWUSR);
		if(fork() == 0) {
			processa_linhas(myid, segment_id[myid], shared_memory[myid]);
		} else {
			myid++;
		}
	}

	// Aguardando todos os filhos encerrarem
    while ((pid = wait(&status)) > 0) {
    	printf("%d: terminei \\o/ \n", (int) pid);
    }

    // Grava resultados
    resultFileCreate();
	for(int i = 0; i < num_procs; i++) {
		shared_memory[i] = (char *) shmat(segment_id[i], NULL, 0);
		fprintf(file_out, "%s\n", shared_memory[i]);
		shmdt(shared_memory[i]);
	}
	resultFileClose();

	// Libera memória alocada
	libera_memoria_alocada();
	
	// Libera memórias compartilhadas
	for(int i = 0; i < num_procs; i++) {
		shmctl(segment_id[i], IPC_RMID, NULL);
	}

	return 0;
}