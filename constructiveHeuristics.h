//
// Created by joaquin on 18/09/18.
//

#ifndef NFSSP_CONSTRUCTIVEHEURISTICS_H
#define NFSSP_CONSTRUCTIVEHEURISTICS_H
#include "schedule.h"
#include "Matrix.h"
#define INF 2000000000

class cHeuristic{
protected:
    Schedule * resultSchedule;
    int nMachines, nJobs;
    Matrix instance;
public:
    cHeuristic(int nMachines, int nJobs, const Matrix & instance);
};


// PERMUTATION FLOW SHOP SCHEDULING CONSTRUCTIVE
class LR : public cHeuristic{
//    using cHeuristic::cHeuristic; // inherits constructor
public:
//    LR();
LR(int nMachine, int nJobs, const Matrix& instance);
    double wTotalMachineiTime(Schedule &S, pseudoJob nextElement);
    double artificialFlowTime(Schedule &S, pseudoJob nextElement, vector<int> & U);
    double getIndexFunction(Schedule &S, int iJob, vector<int> &U);
    Schedule localLR(Schedule  S, vector<int> & U, int uJobs); // S is the initial schedule , assigns U jobs until only uJobs remains.
    double weightFunction(int iJob, int iMachine);
    Schedule getLR(int x); // General x LR's
};

class randomPermutation : public cHeuristic{
    using cHeuristic::cHeuristic;
public:
    Schedule getRandomPermutation();
};

class arbitraryPermutation : public cHeuristic{
    using cHeuristic::cHeuristic;
public:
    Schedule getArbitraryPermutation();
};


class NEH : public cHeuristic{
    using cHeuristic::cHeuristic;
private:
    void NEHOuterLoop(vector< pair<double, int> > orderedJobs);
    int NEHInnerLoop(int jobNumber);
public:
    Schedule getNEH();
    Schedule getNEH(Schedule & S, vector<int> & U);
    arbitraryPermutation * a;
};


class LRandNEH: public cHeuristic{
    using cHeuristic::cHeuristic;
public:
    Schedule getLRandNEH(int x);
};


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
