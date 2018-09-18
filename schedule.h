//
// Created by joaquin on 18/09/18.
//

#ifndef NFSSP_SCHEDULE_H
#define NFSSP_SCHEDULE_H
#include <vector>
using namespace std;
typedef unsigned int uint;

struct pseudoJob{
    uint begin, end, job;
    pseudoJob(uint nMachines, uint job){ // possible thrash
        this->begin = 0;
        this->end = nMachines;
        this->job = job;
    }
    pseudoJob(uint begin, uint end, uint job){
        this->begin = begin;
        this->end = end;
        this->job = job;
    }
};

struct machineInfo{
    uint actTotalFlow, lastTimeJob;
    machineInfo(){
        this->actTotalFlow = 0;
        this->lastTimeJob = 0;
    }
    machineInfo(uint actTotalFlow, uint lastTimeJob){
        this->actTotalFlow = actTotalFlow;
        this->lastTimeJob = lastTimeJob;
    }
};


class Schedule {
    vector< vector<uint> > instance;
    vector<pseudoJob> schedule; // maybe I can use a better data structure
    uint nMachines,nJobs;
    uint getLastTime(vector< machineInfo > & machineState, uint actMachine);
public:
    Schedule(uint nMachines, uint nJobs, vector< vector<uint> > & instance);
    bool addPseudoJob(uint begin, uint end, uint job);
    uint getTotalFlowTime();
    void printGantt(); // expensive function possibly
    void printPermutationSchedule();
};


#endif //NFSSP_SCHEDULE_H
