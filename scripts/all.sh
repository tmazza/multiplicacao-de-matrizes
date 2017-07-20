#!/bin/bash

# ./scripts/exec.sh processos m-16x20-20x32 8

if [ $1 ]; then
	if [ $2 ]; then 

		versao=$1
		num_procs=$2
		
		for dir in ./data/*
		do
			dir_name=`basename $dir`
			
			echo "----------- START ${dir_name} --------------------"
			echo ""
			
			eval "./scripts/exec.sh ${versao} m-16x20-20x32 ${num_procs}"

			echo ""
			echo "----------- END ${dir_name} --------------------"
			echo ""

		done


	else
		echo "Informe a quantidade de processo!!" 
	fi
else
	echo "Informe o tipo: threads ou processos" 
fi