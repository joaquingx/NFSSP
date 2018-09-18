#include <random>

//
// Created by joaquin on 18/09/18.
//

#include <algorithm>
#include <iostream>
#include "constructiveHeuristics.h"

randomPermutation::randomPermutation(uint nMachines, uint nJobs, vector<vector<uint> > &instance) {
    this->nMachines = nMachines;
    this->nJobs = nJobs;
    this->instance = instance;
    initialSchedule = new Schedule(nMachines,nJobs,instance);

}

Schedule randomPermutation::getRandomPermutation() {
    vector<uint> toShuffle(nJobs);
    //generates 1 to nJobs permutation;
    generate(toShuffle.begin(), toShuffle.end(), []{
        static uint i = 0;
        return i++;
    });
    shuffle(toShuffle.begin(), toShuffle.end(), std::mt19937(std::random_device()()));
    for(int i = 0 ; i < nJobs; ++i){
        initialSchedule->addPseudoJob(0,nMachines,toShuffle[i]);
    }
    return *initialSchedule;
}

arbitraryPermutation::arbitraryPermutation(uint nMachines, uint nJobs, vector<vector<uint> > &instance) {
    this->nMachines = nMachines;
    this->nJobs = nJobs;
    this->instance = instance;
    initialSchedule = new Schedule(nMachines,nJobs,instance);
}

Schedule arbitraryPermutation::getArbitraryPermutation() {
    for(int i = 0 ; i < nJobs; ++i){
        int job; cin >>job;
        initialSchedule->addPseudoJob(0,nMachines,job);
    }
    return *initialSchedule;
}

