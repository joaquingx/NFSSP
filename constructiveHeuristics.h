//
// Created by joaquin on 18/09/18.
//

#ifndef NFSSP_CONSTRUCTIVEHEURISTICS_H
#define NFSSP_CONSTRUCTIVEHEURISTICS_H
#include "schedule.h"

class NEH{
    Schedule * initialSchedule;
};


// PERMUTATION FLOW SHOP SCHEDULING CONSTRUCTIVE
class LR{
    Schedule * resultSchedule;
    int nMachines, nJobs;
    vector < vector< int> > instance;
    double weightFunction(int iJob, int iMachine);
    double wTotalMachineiTime(Schedule &S, pseudoJob nextElement);
    double artificialFlowTime(Schedule &S, pseudoJob nextElement, vector<int> & U);
    Schedule localLR(Schedule & S, vector<int> U);
public:
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

class constructiveHeuristics {

};


#endif //NFSSP_CONSTRUCTIVEHEURISTICS_H
