#!/bin/bash

# Cria matriz com tamanhos $1x$2 e $2x$3
# dentro do diretorio /data do projeto

# linhas in1
if [ $1 ]; then
	lin_in1=$1
else
	printf 'Linhas in1: '
	read lin_in1
fi

# colunas in1/linhas in2
if [ $2 ]; then
	col_in1=$2
else
	printf 'Colunas in1/Linhas in2: '
	read col_in1
fi

# colunas in2
if [ $3 ]; then
	col_in2=$3
else
	printf 'Colunas in2: '
	read col_in2
fi

# maior inteiro aceito
if [ $4 ]; then
	max_int=$4
else
	printf 'Gerar inteiros entre 0 e : '
	read max_int
fi

printf "in1: ${lin_in1}x${col_in1}\nin2: ${col_in1}x${col_in2}\nMax int: ${max_int}\n"

dir_name="./data/m-${lin_in1}x${col_in1}-${col_in1}x${col_in2}"
mkdir -p $dir_name

# Grava in1
file_name="${dir_name}/in1.txt"

echo "LINHAS = $lin_in1" > $file_name
echo "COLUNAS = $col_in1" >> $file_name

for i in `seq 1 $lin_in1`
do
	for i in `seq 1 $col_in1`
	do
		echo -n "$(( ( RANDOM % $max_int )  + 1 )) " >> $file_name
	done

	echo "" >> $file_name
done

# Grava in2
file_name="${dir_name}/in2.txt"

echo "LINHAS = $col_in1" > $file_name
echo "COLUNAS = $col_in2" >> $file_name

for i in `seq 1 $col_in1`
do
	for i in `seq 1 $col_in2`
	do
		echo -n "$(( ( RANDOM % $max_int )  + 1 )) " >> $file_name
	done

	echo "" >> $file_name
done



