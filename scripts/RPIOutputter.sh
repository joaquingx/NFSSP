#!/usr/bin/env bash

results_path="../results/*"
number_heuristics=5
index=0

for filename in ${results_path};do
    index=$((index+1))
#    awk 'NR==1{printf $4 " "}' ${filename}
    echo ${filename}
    if [[ $((index % 5)) -eq 0  ]]
    then
        echo ""
    fi

done