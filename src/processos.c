#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
// 
#include "util.c"
#include "../inc/processos.h"

int **in1;
int in1_lin = 0, in1_col = 0;

int **in2;
int in2_lin = 0, in2_col = 0;

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
 */
void init(int argc, char *argv[]) {
	if(argc < 4) { 
		printf("Entrada inválida\n\tUse ./processos in1 in2 n\n"); 
		exit(1);
	}

	printf("\nMatriz 1: %20s\nMatriz 2: %20s\nQtd proc: %s\n\n", argv[2], argv[3], argv[1]);

	/* Lê e processa in1 */
	FILE *file_in1;
	file_in1 = fopen(argv[2],"r");
	if (!file_in1) {
		printf("Erro ao abrir arquivo in1\n");
	    exit(1);
	}
	// Números linha x coluna

	fscanf(file_in1, "%*s = %d\n", &in1_lin);
	fscanf(file_in1, "%*s = %d\n", &in1_col);

	if(!in1_lin || !in1_col) {
		printf("Não foi possível ler o número de linhas ou o número de colunas\n");
	    exit(1);
	}

	printf("in1 - Linhas : %d\n", in1_lin);
	printf("in1 - Colunas: %d\n", in1_col);

	in1 = alocaMatrizInteiros(in1_lin, in1_col);

	int n;
	for(int i = 0; i < in1_lin; i++) {
		for(int j = 0; j < in1_col; j++) {
			fscanf(file_in1, "%d", &n);
			in1[i][j] = n;
    		printf("%d, %d --- %d \n", i, j, in1[i][j]);
		}
	}

	// char * format = malloc(sizeof(char) * 3 * in1_col); // 3 --> '%d '
	// strcpy(format, "");

	// for(int i = 0; i < in1_lin; i++) {
	// 	strcat(format, "%d ");
	// } 
	// printf("--- %s\n", format);

	// // Lê linhas da matriz

	// free(format);


	// Número de linhas
	// while (r != EOF)
	// {
	//     line++;
	//     if (r == 2)
	//     {
	//         if(strcmp(argv[2], loc) == 0)
	//         {
	//             t_tot += temp;
	//             found++;
	//         }
	//     }
	//     else
	//         printf ("Error, line %d in wrong format!\n\n", line);
	//     r = fscanf(fp, "%f %s\n", &temp, loc);
	// }


	fclose(file_in1);


	// 

}


int main(int argc, char *argv[])
{
	init(argc, argv);

	printf("------------\n");

	for(int i = 0; i < in1_lin; i++) {
		for(int j = 0; j < in1_col; j++) {
			printf("%d,%d - %d\n", i, j, in1[i][j]);
		}
	}

	return 0;
}
