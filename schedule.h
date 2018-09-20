//
// Created by joaquin on 18/09/18.
//

#ifndef NFSSP_SCHEDULE_H
#define NFSSP_SCHEDULE_H
#include <vector>
using namespace std;

struct pseudoJob{
    int begin, end, job;
    pseudoJob(int nMachines, int job){ // possible thrash
        this->begin = 0;
        this->end = nMachines;
        this->job = job;
    }
    pseudoJob(int begin, int end, int job){
        this->begin = begin;
        this->end = end;
        this->job = job;
    }
};

struct machineInfo{
    int actTotalFlow, lastTimeJob;
    machineInfo(){
        this->actTotalFlow = 0;
        this->lastTimeJob = 0;
    }
    machineInfo(int actTotalFlow, int lastTimeJob){
        this->actTotalFlow = actTotalFlow;
        this->lastTimeJob = lastTimeJob;
    }
};


class Schedule {
    vector< vector<int> > instance;
    vector<pseudoJob> schedule; // maybe I can use a better data structure
    int nMachines,nJobs;
    int getLastTime(vector< machineInfo > & machineState, int actMachine);
public:
    Schedule(int nMachines, int nJobs, vector< vector<int> > & instance);
    bool addPseudoJob(int begin, int end, int job);
    bool addPseudoJob(pseudoJob nJob);
    int getPermutationFlowTime(int indexMachine, int indexJob);
    int getTotalFlowTime();
    void printGantt(); // expensive function possibly
    void printPermutationSchedule();
    int getSize();
    pseudoJob getPseudoJob(int x);
};


#endif //NFSSP_SCHEDULE_H
