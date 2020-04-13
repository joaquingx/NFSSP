#!/usr/bin/env bash

source global_vars.sh
index=0

for filename in ${results_path_out};do
    index=$((index+1))
    awk 'NR==2{printf $4 " "}' ${filename}
    if [[ $((index % ${HEURISTICS})) -eq 0  ]]
    then
        echo ""
    fi

done
