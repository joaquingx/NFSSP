//
// Created by joaquin on 18/09/18.
//

#include <iostream>
#include <X11/Xlib.h>
#include "schedule.h"
#include "cassert"
#include <algorithm>

Schedule::Schedule(const shared_ptr<ProblemInstance>& pInstance) {
    this->pInstance = pInstance;
}

void Schedule::addPseudoJob(const t_machine& begin, const t_machine& end, const t_job& job) {
    pseudoJob actJob(begin,end,job);
    schedule.emplace_back(actJob);
}

void Schedule::addPseudoJob(const pseudoJob& nJob){
    schedule.emplace_back(nJob);
}

void Schedule::addPseudoJob(const t_machine& begin,const t_machine& end,const t_job& job,const t_size_type&  index) {
    auto actInsertion = schedule.begin() + index;
    pseudoJob actJob(begin,end,job);
    if(schedule.empty())
        schedule.emplace_back(actJob);
    else
        schedule.insert(actInsertion, actJob);
}

// Could be a error due how upperLastJob is obtained
t_flow_time Schedule::getLastTime(const vector<machineInfo>& machineState, const t_machine& actMachine) const {
    auto myLastJob = machineState[actMachine].lastTimeJob;
    if(actMachine == 0)
        return myLastJob;
    auto upperLastJob = machineState[actMachine-1].lastTimeJob;
    return max(myLastJob, upperLastJob);
}
//maybe can be improved with taillard?
// iJob: actual index pseudo job
// iMachine: actual index Machine
t_flow_time Schedule::getTotalFlowTime() const {
    vector<machineInfo> tCompletion(pInstance->nMachines);
    t_flow_time rTotalFlowTime = 0;
    for (auto apJob : schedule) {
        for (t_machine iMachine = apJob.begin; iMachine < apJob.end; ++iMachine) {
            tCompletion[iMachine].lastTimeJob = getLastTime(tCompletion,iMachine) + pInstance->instance(iMachine,apJob.job);
            tCompletion[iMachine].actTotalFlow += tCompletion[iMachine].lastTimeJob;
            rTotalFlowTime = max(rTotalFlowTime,tCompletion[iMachine].actTotalFlow);
        }
    }
    return rTotalFlowTime;
}

void Schedule::printGantt() const {
    vector< vector< char > > chart(pInstance->nMachines);
    vector<machineInfo> tCompletion(pInstance->nMachines);
    t_flow_time rTotalFlowTime = 0;
    for (auto apJob : schedule) {
        for (t_machine iMachine = apJob.begin; iMachine < apJob.end; ++iMachine) {
            int lastTime = getLastTime(tCompletion,iMachine);
//            cout << "Machine" << iMachine << "on Job" << apJob.job << ": " << lastTime << "\n";
            if(iMachine > 0) {
                for (t_flow_time iDraw = 0; iDraw < lastTime-tCompletion[iMachine].lastTimeJob; iDraw++)
                    chart[iMachine].emplace_back('-');
            }
            tCompletion[iMachine].lastTimeJob = getLastTime(tCompletion,iMachine) + pInstance->instance(iMachine,apJob.job);
            tCompletion[iMachine].actTotalFlow += tCompletion[iMachine].lastTimeJob;
            rTotalFlowTime = max(rTotalFlowTime,tCompletion[iMachine].actTotalFlow);

            //draw
            for(t_flow_time iDraw = 0 ;  iDraw < pInstance->instance(iMachine,apJob.job); iDraw++) {
                chart[iMachine].emplace_back(char(apJob.job + '0'));
            }
        }
    }


    cout << "\n*********************Gantt Printed*********************\n\n";
    int maxSize = 0;
    for (auto &i : chart)
        maxSize = max(maxSize, int(i.size()));
    for(int i = 0 ; i < chart.size(); ++i){
        cout << "M" << i << ": ";
        for(int j = 0 ; j < maxSize; ++j){
//            cout << chart[i][j];
            if(chart[i].size() > j)
                cout << chart[i][j];
            else
                cout << "-";
        }
        cout << "\n";
    }
    for(int j = 0 ; j < maxSize+9; ++j){
        cout << "-";
    }
    cout << "\n";
    for(int j = 0 ;j<3;++j)
        cout << " ";
    for(int j = 0 ; j < maxSize; ++j){
        if(!(j%5))
            cout << "*";
        else
            cout << " "; //wrong
    }
    cout << "\n*********************Gantt Printed*********************\n\n";
    cout << "Total Flowtime: " << rTotalFlowTime << "\n";
}

void Schedule::printPermutationSchedule() const {
    cout << "\nPermutation Schedule\n\n";
    for (auto &i : schedule) {
        cout << i.job << " ";
    }
    cout << "\n";
}

t_size_type Schedule::getSize() const {
    return schedule.size();
}

pseudoJob Schedule::getPseudoJob(const t_size_type & x) {
    return schedule[x];
}

//Assuming good input always, works  only w permutation fssp
t_flow_time Schedule::getPermutationFlowTime(const t_machine& indexMachine, const t_job& indexJob) const{
    if(indexJob < 0){
        return 0;
    }
    vector<machineInfo> tCompletion(pInstance->nMachines);
    t_job iJob=0;
    for (auto apJob : schedule) {
        for (t_machine iMachine = apJob.begin; iMachine < apJob.end; ++iMachine) {
            tCompletion[iMachine].lastTimeJob = getLastTime(tCompletion, iMachine) + pInstance->instance(iMachine,apJob.job);
            tCompletion[iMachine].actTotalFlow += tCompletion[iMachine].lastTimeJob;
            if(iJob == indexJob and iMachine == indexMachine){
                return tCompletion[iMachine].lastTimeJob;
            }
        }
        ++iJob;
    }
    static_assert(true); // Code couldn't reach this
    return -1;
}

void Schedule::removePseudoJob(const t_job& actJob ) {
    pseudoJob pseudo(0,pInstance->nMachines,actJob);
    auto iJob = find(schedule.begin(), schedule.end(), pseudo);
    schedule.erase(iJob);
}

Schedule::Schedule() = default;

void Schedule::cleanSchedule() {
    schedule.clear();
}

void Schedule::shiftPseudoJob(const t_job &firstJob, const t_job &secondJob) {
    pseudoJob firstPermJob(0,pInstance->nMachines,firstJob);
    pseudoJob secondPermJob(0,pInstance->nMachines,secondJob);
    auto iFirstJob = find(schedule.begin(), schedule.end(), firstPermJob);
    auto iSecondJob = find(schedule.begin(), schedule.end(), secondPermJob);
//    cout << "Antes:\n";
//    printPermutationSchedule();
    iter_swap(iFirstJob,iSecondJob);
//    cout << "Despues:\n";
//    printPermutationSchedule();
}

void Schedule::removePseudoJobIndex(const t_size_type &index) {
    auto actRemove = schedule.begin() + index;
    schedule.erase(actRemove);
}


istream& operator>> (istream& is, shared_ptr<ProblemInstance>& pInstance){
    is >> pInstance->nJobs >> pInstance->nMachines;
    pInstance->instance = Matrix(pInstance->nMachines, pInstance->nJobs);
    for(t_machine i = 0; i < pInstance->nMachines; ++i){
        for(t_job j = 0; j < pInstance->nJobs; ++j){
            is >> pInstance->instance(i,j);
        }
    }
    return is;
}


bool operator==(const pseudoJob & A, const pseudoJob & B) {
    return A.job == B.job;
}
