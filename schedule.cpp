//
// Created by joaquin on 18/09/18.
//

#include <iostream>
#include "schedule.h"

Schedule::Schedule(int nMachines, int nJobs, vector< vector<int> > & instance) {
    this->nMachines = nMachines;
    this->nJobs = nJobs;
    this->instance = instance;
}

bool Schedule::addPseudoJob(int begin, int end, int job) {
    pseudoJob actJob(begin,end,job);
    auto error = schedule.emplace_back(actJob);
    return true;
}
bool Schedule::addPseudoJob(pseudoJob nJob) {
    auto error = schedule.emplace_back(nJob);
    return true;

}


// Could be a error due how upperLastJob is obtained
int Schedule::getLastTime(vector<machineInfo> &machineState, int actMachine) {
    int myLastJob = machineState[actMachine].lastTimeJob;
    if(actMachine == 0)
        return myLastJob;
    int upperLastJob = machineState[actMachine-1].lastTimeJob;
    return max(myLastJob, upperLastJob);
}
//maybe can be improved with taillard?
// iJob: actual index pseudo job
// iMachine: actual index Machine
int Schedule::getTotalFlowTime() {
    vector<machineInfo> tCompletion(nMachines);
    int rTotalFlowTime = 0;
    for (auto apJob : schedule) {
        for (int iMachine = apJob.begin; iMachine < apJob.end; ++iMachine) {
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
    int rTotalFlowTime = 0;
    for (auto apJob : schedule) {
        for (int iMachine = apJob.begin; iMachine < apJob.end; ++iMachine) {
            int lastTime = getLastTime(tCompletion,iMachine);
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

void Schedule::printPermutationSchedule() {
    cout << "\nPermutation Schedule\n\n";
    for (auto &i : schedule) {
        cout << i.job << " ";
    }
    cout << "\n";
}

int Schedule::getSize() {
    return int(schedule.size());
}

pseudoJob Schedule::getPseudoJob(int x) {
    return schedule[x];
}

//Assuming good input always, works  only w permutation fssp
int Schedule::getPermutationFlowTime(int indexMachine, int indexJob) {
    if(indexJob < 0){
        return 0;
    }
    vector<machineInfo> tCompletion(nMachines);
    int iJob=0;
    for (auto apJob : schedule) {
        for (int iMachine = apJob.begin; iMachine < apJob.end; ++iMachine) {
            tCompletion[iMachine].lastTimeJob = getLastTime(tCompletion, iMachine) + instance[iMachine][apJob.job];
            tCompletion[iMachine].actTotalFlow += tCompletion[iMachine].lastTimeJob;
            if(iJob == indexJob and iMachine == indexMachine){
                return tCompletion[iMachine].actTotalFlow;
            }
        }
        ++iJob;
    }

}

bool Schedule::addPseudoJob(int begin, int end, int job,int  index) {
    auto actInsertion = schedule.begin() + index;
    pseudoJob actJob(begin,end,job);
    if(schedule.size() == 0)
        schedule.emplace_back(actJob);
    else
        auto error = schedule.insert(actInsertion, actJob);
    return true;
}

bool Schedule::removePseudoJob(int index) {
    auto actRemove = schedule.begin() + index;
    schedule.erase(actRemove);
    return true;
}










