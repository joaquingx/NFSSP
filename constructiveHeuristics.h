//
// Created by joaquin on 18/09/18.
//

#ifndef NFSSP_CONSTRUCTIVEHEURISTICS_H
#define NFSSP_CONSTRUCTIVEHEURISTICS_H
#include "schedule.h"

class NEH{
    Schedule * initialSchedule;
};

class randomPermutation{
    Schedule * initialSchedule;
    uint nMachines,nJobs;
    vector< vector <uint> >  instance;
public:
    randomPermutation(uint nMachines, uint nJobs, vector<vector < uint> > & instance);
    Schedule getRandomPermutation();
};

class arbitraryPermutation{
    Schedule * initialSchedule;
    uint nMachines,nJobs;
    vector< vector <uint> >  instance;
public:
    arbitraryPermutation(uint nMachines, uint nJobs, vector<vector < uint> > & instance);
    Schedule getArbitraryPermutation();
};

class constructiveHeuristics {

};


#endif //NFSSP_CONSTRUCTIVEHEURISTICS_H
