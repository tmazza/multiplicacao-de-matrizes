#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
// 
#include "util.c"
#include "../inc/processos.h"

// Quantidade de processo informadas atrabés da linha de comando
int num_procs = 0;
// Distribuição das linhas entre os processos
// Lista de tuplas [inicio, fim], sendo inicio e fim interiros que
// definem quais linhas o processo i deve executar
int **procs_dist; 
// Matriz in1
int **in1 = NULL;
int in1_lin = 0, in1_col = 0;
// Matriz in2
int **in2 = NULL;
int in2_lin = 0, in2_col = 0;
// Matriz resultado
int **out = NULL;
int out_lin = 0, out_col = 0;

int** alocaMatrizInteiros(int lin, int col)
{
	int **m = (int**) malloc(lin * sizeof(int*));
	
	for (int i = 0; i < lin; i++) {
		m[i] = (int*) malloc(col * sizeof(int));
		for (int j = 0; j < col; j++) {
			m[i][j] = 0;
		}
	}
	return m;
}

/**
 * Valida parâmetros de entrada
 * Lê e processa arquivos in1 e in2 
 * Aloca matriz para in1, in2 e out
 */
void init(int argc, char *argv[]) {

	FILE *temp_file; // Ponteiro para arquivo sido processado
	int temp_n; // Inteiro lido do arquivo

	if(argc < 4) { 
		printf("Entrada inválida\n\tUse ./processos in1 in2 n\n"); 
		exit(1);
	}

	/**** Qtd processos ****/
	num_procs = (int) strtol(argv[3], NULL, 10);

	// TODO: criar função para leitura e processamento do arquivo
	/**** Lê e processa in1 ****/
	temp_file = fopen(argv[1],"r");
	if (!temp_file) {
		printf("Erro ao abrir arquivo in1\n");
	    exit(1);
	}
	// Números linha x coluna

	fscanf(temp_file, "%*s = %d\n", &in1_lin);
	fscanf(temp_file, "%*s = %d\n", &in1_col);

	if(!in1_lin || !in1_col) {
		printf("Não foi possível ler o número de linhas ou o número de colunas\n");
	    exit(1);
	}

	in1 = alocaMatrizInteiros(in1_lin, in1_col);

	for(int i = 0; i < in1_lin; i++) {
		for(int j = 0; j < in1_col; j++) {
			fscanf(temp_file, "%d", &temp_n);
			in1[i][j] = temp_n;
		}
	}
	fclose(temp_file);

	/**** Lê e processa in2 ****/
	temp_file = fopen(argv[2],"r");
	if (!temp_file) {
		printf("Erro ao abrir arquivo in2\n");
	    exit(1);
	}
	// Números linha x coluna

	fscanf(temp_file, "%*s = %d\n", &in2_lin);
	fscanf(temp_file, "%*s = %d\n", &in2_col);

	if(!in2_lin || !in2_col) {
		printf("Não foi possível ler o número de linhas ou o número de colunas\n");
	    exit(1);
	}

	in2 = alocaMatrizInteiros(in2_lin, in2_col);

	for(int i = 0; i < in2_lin; i++) {
		for(int j = 0; j < in2_col; j++) {
			fscanf(temp_file, "%d", &temp_n);
			in2[i][j] = temp_n;
		}
	}

	fclose(temp_file);

	/**** Define divisão de linhas para processos ****/
	if(!num_procs) {
		printf("*** A quandidade de processos é inválida. Somente um processo será criado.\n\n");
		num_procs = 1;
	}

	if(num_procs > in1_lin) {
		printf("*** Quantidade de maior do que a de linhas! Será utilizado um processo por linha. Utilizados %d processos dos %d solicitados\n\n", in1_lin, num_procs);
		num_procs = in1_lin;
	}

	int lin_procs = in1_lin/num_procs; // Linhas por processo

	procs_dist = alocaMatrizInteiros(num_procs, 2); // Uma tupla [inicio, fim] para cada processo

	for(int i = 0; i < num_procs; i++) {
		procs_dist[i][0] = i*lin_procs; // from line
		procs_dist[i][1] = procs_dist[i][0] + lin_procs - 1; // to line
	}

	// Divisão pode não ser inteira, linhas faltantes são atribuidas 
	// ao último processo. Por exemplo com 8 linhas e 3 processos, o 
	// ultimo ficara com 4 linhas enquanto os demais ficarão com 2
	int diff = in1_lin - (num_procs * lin_procs); 
	procs_dist[num_procs-1][1] += diff;

	/**** Alocação matriz resultado ****/
	out_lin = in1_lin;
	out_col = in2_col;
	out = alocaMatrizInteiros(out_lin, out_col);

	/**** View ****/ 
	printf("\nMatriz 1: %20s\nMatriz 2: %20s\nQtd proc: %s\n", argv[1], argv[2], argv[3]);
	printf("Por proc: %d\n", lin_procs);
	for(int i = 0; i < num_procs; i++) {
		printf("Proc %d, linhas: %d a %d\n", i, procs_dist[i][0], procs_dist[i][1]);
	}

}

/**
 * Grava matriz out em arquivo
 */
void save_out() {

	FILE *file_out;
	file_out = fopen("./out.txt","w");
	if (!file_out) {
		printf("Erro ao abrir arquivo out.txt\n");
	    exit(1);
	}

	fprintf(file_out, "LINHAS = %d\nCOLUNAS = %d\n", out_lin, out_col);
	for(int i = 0; i < out_lin; i++) {
		for(int j = 0; j < out_col; j++) {
			fprintf(file_out, "%d ", out[i][j]);
		}
		fprintf(file_out, "\n");
	}

	fclose(file_out);
}

void printMatriz(int n) {
	int lin = n==1?in1_lin : (n==2 ? in2_lin : out_lin) ;
	int col = n==1?in1_col : (n==2 ? in2_col : out_col) ;
	int **m = n==1?in1     : (n==2 ? in2     : out) ;

	printf("\n----- Matriz %d | %dx%d -----\n", n, lin, col);
	for(int i = 0; i < lin; i++) {
		for(int j = 0; j < col; j++) {
			printf("%4d", m[i][j]);
		}
		printf("\n");
	}
	printf("\n");
}

/**
 * Informações das matrizes in1 e in2 e da distribuição dos processos
 * são usadas pelos processos filhos acessando as cópias realiazidas pelo fork()
 * No final da execução os processos filhas salva da memória compartilha com
 * segment_id igual ao seu número de processo 'i' associado no momento do fork()
 */
int main(int argc, char *argv[])
{
	init(argc, argv);

	printMatriz(1);
	printMatriz(2);

	int status = -8;

	// for(int i = 0; i < in1_lin; i++) {
	// 	int result;
	// 	for(int k = 0; k < in2_col; k++) { // Colunas em in2
	// 		result = 0;
	// 		for(int j = 0; j < in1_col; j++) { // in1_col == in2_lin
	// 			result += in1[i][j] * in2[j][k];
	// 		}
	// 		out[i][k] = result;
	// 	}
	// }

	int myid = 0;
	pid_t wpid;
	int pid = fork();
	if(pid == 0) {
		
		printf("proc:%d | id:%d | Linhas atribuidas: %d a %d\n", getpid(), myid, procs_dist[myid][0], procs_dist[myid][1]);

		sleep(2);
		exit(0);

	 } else {

	 	myid++;

		int pid2 = fork();
		if(pid2 == 0) {

			printf("proc:%d | id:%d | Linhas atribuidas: %d a %d\n", getpid(), myid, procs_dist[myid][0], procs_dist[myid][1]);
			exit(0);

		} 
		// else {

		// 	// only pather room
		// 	printf("Esperando... \n");
		// 	waitpid(pid, &status, 1);
		// 	waitpid(pid2, &status, 1);
		// 	printf("Done! %d\n", status);


		// 	printMatriz(3);
		// 	save_out();

		// 	free(procs_dist);
		// 	free(in1);
		// 	free(in2);
		// 	free(out);

		// 	exit(0);

		// }

	} 

    while ((wpid = wait(&status)) > 0) {
    	printf("%d Terminou \n", (int) wpid);
    }

    // Done.

	return 0;
}
