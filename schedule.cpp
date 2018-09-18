//
// Created by joaquin on 18/09/18.
//

#include <iostream>
#include "schedule.h"

Schedule::Schedule(uint nMachines, uint nJobs, vector< vector<uint> > & instance) {
    this->nMachines = nMachines;
    this->nJobs = nJobs;
    this->instance = instance;
}

bool Schedule::addPseudoJob(uint begin, uint end, uint job) {
    pseudoJob actJob(begin,end,job);
    auto error = schedule.emplace_back(actJob);
    return true;
}


// Could be a error due how upperLastJob is obtained
uint Schedule::getLastTime(vector<machineInfo> &machineState, uint actMachine) {
    uint myLastJob = machineState[actMachine].lastTimeJob;
    if(actMachine == 0)
        return myLastJob;
    uint upperLastJob = machineState[actMachine-1].lastTimeJob;
    return max(myLastJob, upperLastJob);
}
//maybe can be improved with taillard?
// iJob: actual index pseudo job
// iMachine: actual index Machine
uint Schedule::getTotalFlowTime() {
    vector<machineInfo> tCompletion(nMachines);
    uint rTotalFlowTime = 0;
    for (auto apJob : schedule) {
        for (uint iMachine = apJob.begin; iMachine < apJob.end; ++iMachine) {
            tCompletion[iMachine].lastTimeJob = getLastTime(tCompletion,iMachine) + instance[iMachine][apJob.job];
            tCompletion[iMachine].actTotalFlow += tCompletion[iMachine].lastTimeJob;
            rTotalFlowTime = max(rTotalFlowTime,tCompletion[iMachine].actTotalFlow);
        }
    }
    return rTotalFlowTime;
}

void Schedule::printGantt() {
    vector< vector< char > > chart(nMachines);
    vector<machineInfo> tCompletion(nMachines);
    uint rTotalFlowTime = 0;
    for (auto apJob : schedule) {
        for (uint iMachine = apJob.begin; iMachine < apJob.end; ++iMachine) {
            uint lastTime = getLastTime(tCompletion,iMachine);
//            cout << "Machine" << iMachine << "on Job" << apJob.job << ": " << lastTime << "\n";
            if(iMachine > 0) {
                for (int iDraw = 0; iDraw < lastTime-tCompletion[iMachine].lastTimeJob; iDraw++)
                    chart[iMachine].emplace_back('-');
            }
            tCompletion[iMachine].lastTimeJob = getLastTime(tCompletion,iMachine) + instance[iMachine][apJob.job];
            tCompletion[iMachine].actTotalFlow += tCompletion[iMachine].lastTimeJob;
            rTotalFlowTime = max(rTotalFlowTime,tCompletion[iMachine].actTotalFlow);

            //draw
            for(int  iDraw = 0 ;  iDraw < instance[iMachine][apJob.job]; iDraw++) {
                chart[iMachine].emplace_back(char(apJob.job + '0'));
            }
        }
    }


    cout << "\n*********************Gantt Printed*********************\n\n";
    uint maxSize = 0;
    for (auto &i : chart)
        maxSize = max(maxSize, uint(i.size()));
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

void Schedule::printPermutationSchedule() {
    cout << "\nPermutation Schedule\n\n";
    for (auto &i : schedule) {
        cout << i.job << " ";
    }
    cout << "\n";
}






