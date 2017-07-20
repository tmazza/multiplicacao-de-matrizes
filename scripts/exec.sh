#!/bin/bash

if [ $1 ]; then
	if [ $2 ]; then 
		if [ $3 ]; then 

			versao=$1
			current_dir=$2
			num_procs=$3

			cmd="./bin/${versao} ./data/${current_dir}/in1.txt ./data/${current_dir}/in2.txt ${num_procs}"

			result_file="./results/${versao}-${num_procs}-${current_dir}.txt"

			echo -n '' > $result_file

			soma=0.0

			for i in `seq 1 10`
			do
				TIMEFORMAT=%R
				TIME=$( { time $cmd > /dev/null; } 2>&1 )
				echo "${i}: ${TIME} | '${cmd}'"
				echo "${i}: ${TIME} | '${cmd}'" >> $result_file
				soma=`echo $soma + $TIME | bc`
			done

			media=`echo ${soma}/10  | bc -l`
			echo "MEDIA: ${media}"
			echo "MEDIA: ${media}" >> $result_file
		
		else
			echo "Informe a quantidade de processo!!" 
		fi
	else
		echo "Informe o diretorio que contem os arquivos in1.txt e in2.txt" 
	fi
else
	echo "Informe o tipo: threads ou processos" 
fi