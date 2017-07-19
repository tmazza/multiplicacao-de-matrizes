#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
// 
#include "util.c"
#include "../inc/processos.h"

int **in1 = NULL;
int in1_lin = 0, in1_col = 0;

int **in2 = NULL;
int in2_lin = 0, in2_col = 0;

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

	printf("\nMatriz 1: %20s\nMatriz 2: %20s\nQtd proc: %s\n\n", argv[2], argv[3], argv[1]);


	// TODO: criar função para leitura e processamento do arquivo
	/**** Lê e processa in1 ****/
	temp_file = fopen(argv[2],"r");
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
	temp_file = fopen(argv[3],"r");
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

	// Alocação matriz resultado
	out_lin = in1_lin;
	out_col = in2_col;
	out = alocaMatrizInteiros(out_lin, out_col);

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

int main(int argc, char *argv[])
{
	init(argc, argv);

	printMatriz(1);
	printMatriz(2);

	int status = -8;
	int pid;

	for(int i = 0; i < in1_lin; i++) {
		int result;
		for(int k = 0; k < in2_col; k++) { // Colunas em in2
			result = 0;
			for(int j = 0; j < in1_col; j++) { // in1_col == in2_lin
				result += in1[i][j] * in2[j][k];
			}
			out[i][k] = result;
		}
	}

	printMatriz(3);

	save_out();

	// pid = fork();
	// if(pid == 0) {
	// 	printf("enter the ninja\n");
	// 	sleep(5);
	// 	printf("get out the ninja\n");
	// } else {

	// 	int pid2 = fork();
	// 	if(pid2 == 0) {
	// 		printf("enter yoland\n");
	// 		sleep(5);
	// 		printf("get out yoland\n");
	// 	} else {

	// 		// only pather room
	// 		printf("before wait\n");
	// 		wait();
	// 		printf("done of wating --- %d \n", status);

	// 	}

	// } 

	return 0;
}
