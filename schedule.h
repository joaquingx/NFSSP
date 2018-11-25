//
// Created by joaquin on 18/09/18.
//

#ifndef NFSSP_SCHEDULE_H
#define NFSSP_SCHEDULE_H
#include <vector>
#include "Matrix.h"
#include <memory>
#include "globalConstants.h"
using namespace std;


struct pseudoJob{
    t_machine begin, end;
    t_job job;
    pseudoJob(const t_machine& nMachines, const t_job& job){ // possible thrash
        this->begin = 0;
        this->end = nMachines;
        this->job = job;
    }
    pseudoJob(const t_machine& begin, const t_machine& end, const t_job& job){
        this->begin = begin;
        this->end = end;
        this->job = job;
    }
    friend bool operator==(const pseudoJob&, const pseudoJob&);
};

struct machineInfo{
    t_flow_time actTotalFlow, lastTimeJob;
    machineInfo(){
        this->actTotalFlow = 0;
        this->lastTimeJob = 0;
    }
    machineInfo(const t_flow_time& actTotalFlow, const t_flow_time& lastTimeJob){
        this->actTotalFlow = actTotalFlow;
        this->lastTimeJob = lastTimeJob;
    }
};

struct ProblemInstance{
    t_machine nMachines{};
    t_job nJobs{};
    Matrix instance;

    ProblemInstance(const t_machine& nMachines, const t_job& nJobs, const Matrix& instance){
        this->nJobs = nJobs;
        this->nMachines = nMachines;
        this->instance = instance;
    }

    ProblemInstance() = default;
    friend istream& operator>> (istream& is, shared_ptr<ProblemInstance>& pInstance);
};


class Schedule {
    vector<pseudoJob> schedule; // maybe I can use a better data structure
    t_flow_time getLastTime(const vector< machineInfo > & machineState, const t_machine& actMachine) const;
public:
    shared_ptr<ProblemInstance> pInstance;
    Schedule();

    explicit Schedule(const shared_ptr<ProblemInstance>& pInstance);

    void addPseudoJob(const t_machine& begin, const t_machine& end, const t_job& job);
    void addPseudoJob(const pseudoJob& nJob);
    void addPseudoJob(const t_machine& begin, const t_machine& end, const t_job& job, const t_size_type& index);
    void shiftPseudoJob(const t_job& firstJob, const t_job& secondJob);
    void removePseudoJobIndex(const t_size_type& index);
    void removePseudoJob(const t_job& actJob);
    void cleanSchedule();


    t_flow_time getPermutationFlowTime(const t_machine& indexMachine, const t_job& indexJob) const;
    t_flow_time getTotalFlowTime() const;
    t_size_type getSize() const;


    void printGantt() const; // expensive function possibly
    void printPermutationSchedule() const;

    pseudoJob getPseudoJob(const t_size_type& x);

    //    vector<pseudoJob>::iterator getBegin();
};

#endif //NFSSP_SCHEDULE_H
