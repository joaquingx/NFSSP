//
// Created by joaquin on 25/11/18.
//

#include "localSearch.h"
#include <algorithm>

shiftLocalSearch::shiftLocalSearch(const shared_ptr<Schedule> &resultSchedule, const t_machine &repetitions):
        resultSchedule(resultSchedule), repetitions(repetitions){};


shared_ptr<Schedule> shiftLocalSearch::getLocalSearch() {
    t_job nJobs = resultSchedule->pInstance->nJobs;
    vector<t_job> initialList(nJobs);
    for(int i = 0 ;i < initialList.size(); ++i){
        initialList[i] = i;
    }
    random_shuffle(initialList.begin(), initialList.end());
    for(auto i = 0 ; i <  repetitions; ++i) {
        for(t_job iJob = 0 ; iJob < nJobs; iJob++){
            t_job selJob = initialList[iJob];
            t_job toChange = shiftJob(selJob);
            if(toChange != selJob){ //There's better result
                resultSchedule->shiftPseudoJob(selJob, toChange);
            }
        }
    }
    return resultSchedule;
}

t_job shiftLocalSearch::shiftJob(const t_job &actJob) {
    t_flow_time betterFlowTime = resultSchedule->getTotalFlowTime();
    t_job toChange = actJob;
    for(t_job iJob = 0 ; iJob < resultSchedule->pInstance->nJobs and actJob != iJob; ++iJob){
        resultSchedule->shiftPseudoJob(actJob,iJob);
        t_flow_time resultFlowTime = resultSchedule->getTotalFlowTime();
        if(resultFlowTime < betterFlowTime){
            betterFlowTime = resultFlowTime;
            toChange = iJob;
        }
        resultSchedule->shiftPseudoJob(actJob,iJob); // went to normality
    }
    return toChange;
};



