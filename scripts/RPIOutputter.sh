#!/usr/bin/env bash

source global_vars.sh
index=0
aux_index=0
for filename in ${results_path_out};do
   aux_index=$((aux_index+1))
   name_prev=${filename#*-}
   name_final=${name_prev%.out}
   echo -n $name_final " "
   if [[ ${aux_index} == ${HEURISTICS} ]]
   then
       break
   fi
done
echo ""
for filename in ${results_path_out};do
    index=$((index+1))
    awk 'NR==1{printf $4 " "}' ${filename}
    #echo ${filename}
    if [[ $((index % ${HEURISTICS})) -eq 0  ]]
    then
        echo ""
    fi

done
