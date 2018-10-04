//
// Created by joaquin on 18/09/18.
//

#ifndef NFSSP_CONSTRUCTIVEHEURISTICS_H
#define NFSSP_CONSTRUCTIVEHEURISTICS_H
#include "schedule.h"
#define INF 2000000000

// PERMUTATION FLOW SHOP SCHEDULING CONSTRUCTIVE
class LR{
    Schedule * resultSchedule;
    int nMachines, nJobs;
    vector < vector< int> > instance;
    double weightFunction(int iJob, int iMachine);
    Schedule localLR(Schedule  S, vector<int> U);
public:
    LR();
    double wTotalMachineiTime(Schedule &S, pseudoJob nextElement);
    double artificialFlowTime(Schedule &S, pseudoJob nextElement, vector<int> & U);
    double getIndexFunction(Schedule &S, int iJob, vector<int> &U);
    LR(int nMachines, int nJobs, vector<vector < int> > & instance);
//    LR();
    Schedule getLR(int x); // General x LR's
};


class randomPermutation{
    Schedule * initialSchedule;
    int nMachines,nJobs;
    vector< vector <int> >  instance;
public:
    randomPermutation(int nMachines, int nJobs, vector<vector < int> > & instance);
    Schedule getRandomPermutation();
};

class arbitraryPermutation{
    Schedule * initialSchedule;
    int nMachines,nJobs;
    vector< vector <int> >  instance;
public:
    arbitraryPermutation(int nMachines, int nJobs, vector<vector < int> > & instance);
    Schedule getArbitraryPermutation();
};


class NEH{
    Schedule * initialSchedule;
    int nMachines, nJobs;
    vector < vector< int> > instance;
    void NEHOuterLoop(vector< pair<double, int> > orderedJobs);
    int NEHInnerLoop(int jobNumber);
public:
    NEH(int nMachines, int nJobs, vector< vector< int> > & instance);
    Schedule getNEH();
    arbitraryPermutation * a;
};


class LRandNEH{
    Schedule * initialSchedule;
    int nMachines, nJobs;
    vector < vector< int> > instance;
public:
    LRandNEH(int nMachines, int nJobs, vector< vector< int> > & instance);
    Schedule getLRandNEH();
};
class constructiveHeuristics {

};


#endif //NFSSP_CONSTRUCTIVEHEURISTICS_H
