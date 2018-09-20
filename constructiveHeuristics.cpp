#include <random>

//
// Created by joaquin on 18/09/18.
//

#include <algorithm>
#include <iostream>
#include "constructiveHeuristics.h"

randomPermutation::randomPermutation(int nMachines, int nJobs, vector<vector<int> > &instance) {
    this->nMachines = nMachines;
    this->nJobs = nJobs;
    this->instance = instance;
    initialSchedule = new Schedule(nMachines,nJobs,instance);
}

Schedule randomPermutation::getRandomPermutation() {
    vector<int> toShuffle(nJobs);
    //generates 1 to nJobs permutation;
    generate(toShuffle.begin(), toShuffle.end(), []{
        static int i = 0;
        return i++;
    });
    shuffle(toShuffle.begin(), toShuffle.end(), std::mt19937(std::random_device()()));
    for(int i = 0 ; i < nJobs; ++i){
        initialSchedule->addPseudoJob(0,nMachines,toShuffle[i]);
    }
    return *initialSchedule;
}

arbitraryPermutation::arbitraryPermutation(int nMachines, int nJobs, vector<vector<int> > &instance) {
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

LR::LR(int nMachines, int nJobs, vector<vector<int> > &instance) {
    this->nMachines = nMachines;
    this->nJobs = nJobs;
    this->instance = instance;
}

Schedule LR::getLR(int x) {
    vector<int> initialList(nJobs);
    //generates 1 to nJobs permutation;
    generate(initialList.begin(), initialList.end(), []{
        static int i = 0;
        return i++;
    });
    // falta la parte donde elijo los x mejores elementos.
    int minFlowTime = 100000000;
    Schedule * finalSchedule;
    for(int i = 0 ;i < x ; ++i){
        Schedule NSchedule(nMachines,nJobs,instance);
        NSchedule.addPseudoJob(0,nMachines,initialList[i]);
        int aux = initialList[i];
        initialList.erase(find(initialList.begin(), initialList.end(), aux));
        NSchedule = localLR(NSchedule,initialList);
        int thisTotalTime = NSchedule.getTotalFlowTime();
        if(thisTotalTime < minFlowTime){
            minFlowTime = thisTotalTime;
            (*finalSchedule) = NSchedule;
        }
        initialList.emplace_back(aux);//error?
    }
    return (*finalSchedule);
}

double LR::wTotalMachineiTime(Schedule &S, pseudoJob  nextElement) {
    double IT = 0;
//    pseudoJob kJob = S.getPseudoJob(S.getSize() - 1);
    //it is a fact.
    int kFlow = S.getPermutationFlowTime(0,S.getSize()-1);
    int lastNewFlowTime = kFlow + instance[0][nextElement.job];
    for(int iMachine = 1 ; iMachine < nMachines; ++iMachine){
        double weight = weightFunction(S.getSize()-1, iMachine);
        kFlow = S.getPermutationFlowTime(0,S.getSize()-1);// can cause troubles
        IT += weight * max(lastNewFlowTime - kFlow,0);
        lastNewFlowTime = max(lastNewFlowTime, kFlow) +
                instance[iMachine][nextElement.job];
    }
    return IT;
}

double LR::weightFunction(int iJob, int iMachine) {
    double dividend = nMachines;
    double divisor  = iMachine + iJob * (nMachines - iMachine)/(nJobs - 2); // precision error
    return dividend/divisor;
}

double getDivisor(double n){
    if(n==0)
        return 1;
    return n;
}

double LR::artificialFlowTime(Schedule &S, pseudoJob nextElement,  vector<int> & U) {
//    pseudoJob * nJob;
    double timeJob = 0;
    vector<double> nTimes;
    for(int iMachine = 0 ; iMachine < nMachines ; ++iMachine) {
        timeJob = 0;
        for (auto iJob : U) {
            timeJob += instance[iMachine][iJob];
        }
        nTimes.push_back(timeJob/getDivisor(nJobs - S.getSize() - 1 - 1)); // div by zero?
    }
    Schedule NS = S;
    NS.addPseudoJob(nextElement);
    double lastFlowTime = NS.getPermutationFlowTime(0,NS.getSize()-1) + nTimes[0];
    for(int iMachine = 1; iMachine < nMachines; ++iMachine){
        double kFlowtime = NS.getPermutationFlowTime(iMachine, NS.getSize()-1);
        lastFlowTime = max(lastFlowTime,kFlowtime) + nTimes[iMachine];
    }
    double kTotalTime = NS.getTotalFlowTime();
    return kTotalTime + lastFlowTime;
}

Schedule LR::localLR(Schedule &S, vector<int> U) {
    double IT,AT,fnc,mFnc;
    int indexJob;
    while(!U.empty()) {
        mFnc = 100000000;
        for (auto job: U) {
            IT = wTotalMachineiTime(S, pseudoJob(0, nMachines, job));
            AT = artificialFlowTime(S, pseudoJob(0, nMachines, job), U);
            fnc = (nJobs - S.getSize() - 1 - 2) * IT + AT;
            if (mFnc > fnc) {
                mFnc = fnc;
                indexJob = job;

            }
        }
        U.erase( find(U.begin(),U.end(),indexJob) );//error?
        S.addPseudoJob(0,nMachines,indexJob);
    }
    return S;
}