#!/bin/bash

versao=$1
current_dir="m-8x8-8x8"
num_procs=2

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

media=`echo ${soma}/10.0  | bc -l`
echo "MEDIA: ${media}"
echo "MEDIA: ${media}" >> $result_file