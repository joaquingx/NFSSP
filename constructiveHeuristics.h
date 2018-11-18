//
// Created by joaquin on 18/09/18.
//

#ifndef NFSSP_CONSTRUCTIVEHEURISTICS_H
#define NFSSP_CONSTRUCTIVEHEURISTICS_H
#include "schedule.h"
#include "Matrix.h"
#define INF 2000000000

using vNEH = vector< pair< double, t_flow_time > >;

class cHeuristic{
protected:
    shared_ptr<Schedule> resultSchedule;
    const Matrix& instance; // reference to matrix to avoid indirections
    const t_job& nJobs; //necessary?
    const t_machine& nMachines;
public:
    explicit cHeuristic(const shared_ptr<ProblemInstance>& pInstance);
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
    shared_ptr<Schedule> getNEH(Schedule & S, vector<t_job> & U);
};


// PERMUTATION FLOW SHOP SCHEDULING CONSTRUCTIVE
class LR : public cHeuristic{
    using cHeuristic::cHeuristic; // inherits constructor
public:
//    LR();
    double wTotalMachineiTime(Schedule& S, pseudoJob nextElement);
    double artificialFlowTime(Schedule& S, pseudoJob nextElement, vector<int> & U);
    double getIndexFunction(Schedule& S, int iJob, vector<int> &U);
    Schedule *localLR(Schedule&  S, vector<int> & U, int uJobs); // S is the initial schedule , assigns U jobs until only uJobs remains.
    double weightFunction(int iJob, int iMachine);
    Schedule * getLR(int x); // General x LR's
};

//class arbitraryPermutation : public cHeuristic{
//    using cHeuristic::cHeuristic;
//public:
//    Schedule getArbitraryPermutation();
//};



//
//
//class LRandNEH: public cHeuristic{
//    using cHeuristic::cHeuristic;
//public:
//    Schedule getLRandNEH(int x);
//};


//class FF{
//    Schedule * resultSchedule;
//    int nMachines, nJobs;
//    Matrixinstance;
//    double weightFunction(int iJob, int iMachine);
//public:
//    FF(int nMachines, int nJobs, vector<vector < int> > & instance);
//    double wTotalMachineiTime(Schedule &S, pseudoJob nextElement);
//    double artificialFlowTime(Schedule &S, pseudoJob nextElement, vector<int> & U);
//    double getIndexFunction(Schedule &S, int iJob, vector<int> &U);
//    Schedule localLR(Schedule  S, vector<int> & U, int uJobs); // S is the initial schedule , assigns U jobs until only uJobs remains.
//    Schedule getLR(int x); // General x LR's
//};
//


#endif //NFSSP_CONSTRUCTIVEHEURISTICS_H
