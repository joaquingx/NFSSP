//
// Created by joaquin on 25/11/18.
//

#include "metaheuristic.h"
#include <memory>
#include <algorithm>

constexpr int max_loop = 2;
constexpr int lr_number = 5;

ILS::ILS(const shared_ptr<ProblemInstance> &pInstance, const t_job &perturbNumber, const t_job &repetitions):
        pInstance(pInstance), perturbNumber(perturbNumber), repetitions(repetitions){}

shared_ptr<Schedule> ILS::getMetaheuristic() {
    LR lrCaller(pInstance);

    shared_ptr<Schedule> resultSchedule = lrCaller.getLR(lr_number); // constructive heuristic
//    resultSchedule->printPermutationSchedule();
    for(int i = 0; i < max_loop; ++i){
        perturbPermutation(resultSchedule);
//        cout << "\nDespues de perturbacion:\n";
//        resultSchedule->printPermutationSchedule();
        shiftLocalSearch shiftCaller(make_shared<Schedule>(*(resultSchedule)), repetitions);
        shared_ptr<Schedule> localSchedule = shiftCaller.getLocalSearch(); // localsearch
        if(localSchedule->getTotalFlowTime() < resultSchedule->getTotalFlowTime()){
            resultSchedule = localSchedule;
        }
//        if(localSchedule->getTotalFlowTime() )
//        if(resultSchedule)
    }
    return resultSchedule;
}

void ILS::perturbPermutation(shared_ptr<Schedule>& resultSchedule) {
    t_job nJobs = pInstance->nJobs;
    t_machine nMachines = pInstance->nMachines;
    vector<t_job> initialList;
    initialList.clear();
    initialList.resize(nJobs);
    for(int i = 0 ;i < initialList.size() ; ++i){
        initialList[i] = i;
    }
    random_shuffle(initialList.begin(), initialList.end());
//    for(int i = 0  ; i < initialList.size() ; ++i){
//        cout << initialList[i] << " ";
//    }
//    cout << "\n";
//    resultSchedule->printPermutationSchedule();
    for(auto i = 0; i < perturbNumber; ++i){
        t_job actJob = initialList[i];
        resultSchedule->removePseudoJob(actJob);
    }
//    resultSchedule->printPermutationSchedule();
    for(auto i = 0 ; i < perturbNumber; ++i){
        t_job actJob = initialList[i];
        t_flow_time betterFlowTime = INF;
        t_job betterPosition = 0;
//        cout << "\nentra asi:\n";
//        resultSchedule->printPermutationSchedule();
        for(int j = 0 ; j <  resultSchedule->getSize(); ++j){
            resultSchedule->addPseudoJob(0, nMachines, actJob, j);
            t_flow_time resultFlowTime = resultSchedule->getTotalFlowTime();
            if(resultFlowTime < betterFlowTime){
                betterFlowTime = resultFlowTime;
                betterPosition = j;
            }
            resultSchedule->removePseudoJobIndex(j);
        }
//        cout << "\nsale asi:\n";
//        resultSchedule->printPermutationSchedule();
        resultSchedule->addPseudoJob(0, nMachines, actJob, betterPosition);
    }
};


