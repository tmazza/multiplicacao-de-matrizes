#include <stdio.h>
#include <stdlib.h>
#include "../inc/util.h"

void init(int argc, char *argv[]) {

	if(argc < 4) { 
		printf("Entrada inválida\n\tUse ./processos in1 in2 n\n"); 
		exit(1);
	}

	num_procs = (int) strtol(argv[3], NULL, 10);
	carrega_arquivos(argv[1], argv[2]);
	define_linhas_cada_processo();

	/**** View ****/ 
	printf("\nMatriz 1: %20s\nMatriz 2: %20s\nQtd proc: %s\n", argv[1], argv[2], argv[3]);
	for(int i = 0; i < num_procs; i++) {
		printf("Proc %d, linhas: %d a %d\n", i, procs_dist[i][0], procs_dist[i][1]);
	}

}

void carrega_arquivos(char *path_in1, char *path_in2) {

	FILE *temp_file; // Ponteiro para arquivo sendo processado
	int temp_n; // Inteiro lido do arquivo

	/**** Lê e processa in1 ****/
	temp_file = fopen(path_in1, "r");
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
	temp_file = fopen(path_in2, "r");
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
}

void define_linhas_cada_processo() {

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

}

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

void printMatriz(int lin, int col, int **m)
{
	printf("\n----- %dx%d -----\n", lin, col);
	for(int i = 0; i < lin; i++) {
		for(int j = 0; j < col; j++) {
			printf("%4d", m[i][j]);
		}
		printf("\n");
	}
	printf("\n");
}

