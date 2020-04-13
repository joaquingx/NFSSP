#!/bin/bash

source global_vars.sh
#LR:0; NEH:1; LR-NEH:2; META:3; FF: 4
methods=(0 1 2 3 4 5)
methods_name=(LR NEH LR-NEH META FF FF-NEH)
sequences=(5 10)

for file_name in ${tests_path};do
    test_name=`basename ${file_name}`
#    [ ${test_name} != ta001 ] && exit;
    for method in "${methods[@]}";do
        method_name="${methods_name[$method]}"
       	if [ ${method} -eq 1 ] || [ ${method} -eq 3 ];
	    then
            inp_name="/tmp/${test_name}-${method_name}.in"
            out_name="${results_path}/${test_name}-${method_name}.out"
#            echo ${out_name}
            touch ${out_name}
		    touch ${inp_name}
		    ./transpose.sh ${file_name} ${inp_name}
		    echo ${method} >> ${inp_name}
#		    ``
            echo "Running  ${inp_name} ..."
		    "${nfssp_exec}" < "${inp_name}" > "${out_name}" &
		else
		    for sequence in ${sequences[@]}; do
       		    inp_name="/tmp/${test_name}-${method_name}-${sequence}.in"
                out_name="${results_path}/${test_name}-${method_name}-${sequence}.out"
                touch ${out_name}
		        touch ${inp_name}
		        ./transpose.sh ${file_name} ${inp_name}
		        echo ${method} >> ${inp_name}
		        echo ${sequence} >> ${inp_name}
		        n_jobs=`head -n 1 ${inp_name} | awk '{print $2}'`
		        [ ${n_jobs} -lt 10 ] && sequence=5
			echo "Running ${inp_name} ..."
		    "${nfssp_exec}" < "${inp_name}" > "${out_name}" &
		    done
	    fi
    done
#    test_name=`basename ${file_name}`
#    touch /tmp/${test_name}-with-lr.in
#    cat ${file_name} > /tmp/${test_name}-with-lr.in
#    echo "1" >> /tmp/${test_name}-with-lr.in
#    echo "1" >> /tmp/${test_name}-with-lr.in
done
