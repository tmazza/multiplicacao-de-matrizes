/**
 *  Quantidade de processo, informadas através da linha de comando
 */
int num_procs = 0;

/** 
 * Distribuição das linhas entre os processos
 * Lista de tuplas [inicio, fim], sendo inicio e fim interiros que
 * definem quais linhas o processo i deve executar
 */ 
int **procs_dist; 

/**
 * Matriz in1
 */
int **in1;

/**
 * Qtd. linhas e colunas in1
 */
int in1_lin = 0, in1_col = 0;

/**
 * Matriz in2
 */
int **in2;

/**
 * Qtd. linhas e colunas in2
 */
int in2_lin = 0, in2_col = 0;

/**
 * Ponteiro para arquivo com resultados
 */
FILE *file_out;

/**
 * Valida parâmetros de entrada
 * Lê e processa arquivos in1 e in2 
 * Aloca matriz para in1, in2 e out
 */
void init(int argc, char *argv[]);

/**
 * Aloca matriz de inteiros de tamanho lin x col
 */
int** alocaMatrizInteiros(int lin, int col);

/** 
 * Imprime matriz formatando linhas e colunas
 */
void printMatriz(int lin, int col, int **m);

/**
 * Carega matrizes dos arquivos path_in1 e path_in2 para
 * as variáveis in1 e in2, respectivamente.
 */
void carrega_arquivos(char *path_in1, char *path_in2);

/**
 * Define divisão de linhas para processos.
 * Caso resultado da divisão não seja inteira as linhas que sobrarem
 * serão atribuídas ao último processo
 */
void define_linhas_cada_processo();

/**
 * Abre arquivo de resultado e escreve cabeçalho.
 */
void resultFileCreate();

/**
 * Fechar arquivo de resultado
 */
void resultFileClose();