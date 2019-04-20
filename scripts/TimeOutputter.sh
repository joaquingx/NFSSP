#!/usr/bin/env bash

results_path="../results/*"
number_heuristics=6
index=0

for filename in ${results_path};do
    index=$((index+1))
    awk 'NR==2{printf $4 " "}' ${filename}
#    echo ${filename}
    if [[ $((index % ${number_heuristics})) -eq 0  ]]
    then
        echo ""
    fi

done
