//
// Created by joaquin on 18/09/18.
//

#ifndef NFSSP_CONSTRUCTIVEHEURISTICS_H
#define NFSSP_CONSTRUCTIVEHEURISTICS_H
#include "schedule.h"
#include "Matrix.h"


constexpr t_flow_time INF = 2000000000;
constexpr t_job const_a = 4;
constexpr t_job const_b = 1;

using vNEH = vector< pair< double, t_flow_time > >;
using shared_ptr_pair_vector = shared_ptr< pair< vector<t_job>, shared_ptr<Schedule> > >;
using pair_vector = pair< vector<t_job>, shared_ptr<Schedule> >;

class cHeuristic{
protected:
    const Matrix& instance; // reference to matrix to avoid indirections
    const t_job& nJobs; //necessary?
    const t_machine& nMachines;
public:
    shared_ptr<Schedule> resultSchedule;
    explicit cHeuristic(const shared_ptr<ProblemInstance>& pInstance);
    virtual shared_ptr<Schedule> getConstructive() = 0;
    void cleanSchedule() const;
};

class randomPermutation : public cHeuristic{
    using cHeuristic::cHeuristic;
public:
    shared_ptr<Schedule> getRandomPermutation();
};

class NEH : public cHeuristic{
    using cHeuristic::cHeuristic;
private:
    void NEHOuterLoop(const vNEH& orderedJobs);
    t_job NEHInnerLoop(const t_job& jobNumber);
public:
    shared_ptr<Schedule> getNEH();
    shared_ptr<Schedule> getNEH(const shared_ptr<Schedule>& S, const vector<t_job>& U);
    shared_ptr<Schedule> getConstructive() override;
};


// PERMUTATION FLOW SHOP SCHEDULING CONSTRUCTIVE
class LR : public cHeuristic{
    using cHeuristic::cHeuristic; // inherits constructor
public:
//    LR();
    double wTotalMachineiTime(const shared_ptr<Schedule>& S, const pseudoJob& nextElement) const;
    double artificialFlowTime(const shared_ptr<Schedule>& S, const pseudoJob& nextElement, const vector<t_job>& U) const;
    double weightFunction(const t_job& iJob, const t_machine& iMachine) const;
    double getIndexFunction(const shared_ptr<Schedule>& S, const t_job& iJob, const vector<t_job>& U) const;
    shared_ptr_pair_vector localLR(const vector<t_job>& remainedJobs, const t_job& jobTaken, const t_job& uJobs);
    shared_ptr<Schedule> getLR(const t_job& x); // General x LR's
    shared_ptr<Schedule> getConstructive() override;
};

class LRandNEH: public cHeuristic{
    using cHeuristic::cHeuristic;
public:
    shared_ptr<Schedule> getLRandNEH(const t_job& x);
    shared_ptr<Schedule> getConstructive() override;
};

class FF: public cHeuristic{
    using cHeuristic::cHeuristic;
public:
    double idleTime(const shared_ptr<Schedule>& S, const pseudoJob& nextElement) const;
    double completionTime(const shared_ptr<Schedule>& S, const pseudoJob& nextElement) const;
    double weightFunction(const t_job& iJob, const t_machine& iMachine) const;
    double getIndexFunction(const shared_ptr<Schedule>& S, const t_job& iJob, const vector<t_job>& U) const;
    shared_ptr_pair_vector localFF(const vector<t_job>& remainedJobs, const t_job& jobTaken, const t_job&  uJobs);
    shared_ptr<Schedule> getFF(const t_job& x);
    shared_ptr<Schedule> getConstructive() override;
};

class FFandNEH: public cHeuristic{
    using cHeuristic::cHeuristic;
public:
    shared_ptr<Schedule> getFFandNEH(const t_job& x);
    shared_ptr<Schedule> getConstructive() override;
};



//class FF{
//    Schedule * resultSchedule;
//    int nMachines, nJobs;
//    Matrixinstance;
//    double weightFunction(int iJob, int iMachine);
//public:
//    FF(int  nMachines, int nJobs, vector<vector < int> > & instance);
//    double wTotalMachineiTime(Schedule &S, pseudoJob nextElement);
//    double artificialFlowTime(Schedule &S, pseudoJob nextElement, vector<int> & U);
//    double getIndexFunction(Schedule &S, int iJob, vector<int> &U);
//    Schedule localLR(Schedule  S, vector<int> & U, int uJobs); // S is the initial schedule , assigns U jobs until only uJobs remains.
//    Schedule getLR(int x); // General x LR's
//};
//


#endif //NFSSP_CONSTRUCTIVEHEURISTICS_H
