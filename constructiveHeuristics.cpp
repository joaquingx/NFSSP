#include <utility>

#include <random>
#include <cassert>
//
// Created by joaquin on 18/09/18.
//

#include <algorithm>
#include <iostream>
#include <map>
#include <variant>
#include "constructiveHeuristics.h"
#include "schedule.h"

cHeuristic::cHeuristic(const shared_ptr<ProblemInstance> &pInstance): resultSchedule(make_shared<Schedule>(pInstance)),
                                                                      instance(pInstance->instance), nMachines(pInstance->nMachines),
                                                                      nJobs(pInstance->nJobs){}



shared_ptr<Schedule> randomPermutation::getRandomPermutation() {
    vector<t_job> toShuffle(resultSchedule->pInstance->nJobs);
    //generates 1 to nJobs permutation;
    generate(toShuffle.begin(), toShuffle.end(), []{
        static t_job i = 0;
        return i++;
    });
    shuffle(toShuffle.begin(), toShuffle.end(), std::mt19937(std::random_device()()));
    for(t_job i = 0 ; i < resultSchedule->pInstance->nJobs; ++i){
        resultSchedule->addPseudoJob(0,resultSchedule->pInstance->nMachines,toShuffle[i]);
    }
    return resultSchedule;
}

shared_ptr<Schedule> NEH::getNEH(const shared_ptr<Schedule>& S, const vector<t_job>& U) {
    vNEH orderedJobs;
    resultSchedule = S;
    map< double, double >  totalProcessingTimes;
    for(t_machine iMachine = 0 ; iMachine < nMachines ; ++iMachine){
        for(t_job actJob : U){
            totalProcessingTimes[actJob] += instance(iMachine,actJob);
        }
    }

    for(t_job actJob: U){
        orderedJobs.emplace_back(totalProcessingTimes[actJob],actJob);
    }
    sort(orderedJobs.begin(), orderedJobs.end());
    NEHOuterLoop(orderedJobs);
    return resultSchedule;
}

shared_ptr<Schedule> NEH::getNEH() {
    vector< pair< double, t_flow_time > > orderedJobs;
    vector< double > totalProcessingTimes(nJobs);
    for(t_machine iMachine = 0 ; iMachine < nMachines ; ++iMachine){
        for(t_job iJob = 0; iJob < nJobs ; ++iJob){
            totalProcessingTimes[iJob] += instance(iMachine,iJob);
        }
    }

    for(t_job iJob = 0 ; iJob < nJobs ; ++iJob){
        orderedJobs.emplace_back(totalProcessingTimes[iJob],iJob);
    }
    sort(orderedJobs.begin(), orderedJobs.end());
    NEHOuterLoop(orderedJobs);
    return resultSchedule;
}


t_job NEH::NEHInnerLoop(const t_job& jobNumber) {
    t_flow_time iWinner = 0, tftWinner = INF;
    for(t_size_type iJobs = 0 ; iJobs < resultSchedule->getSize() ; ++iJobs){
        resultSchedule->addPseudoJob(0,nMachines,jobNumber,iJobs);
        t_flow_time actualTFT = resultSchedule->getTotalFlowTime();
        if(actualTFT < tftWinner){
            iWinner = iJobs;
            tftWinner = actualTFT;
        }
        resultSchedule->removePseudoJobIndex(iJobs);
    }
    return iWinner;
}

void NEH::NEHOuterLoop(const vNEH& orderedJobs) {
    //O(n^3*m)
    // non-descending order better for tft
    for(t_job iJobs = orderedJobs.size()-1 ; iJobs >= 0 ; --iJobs){
        t_job jobNumber = orderedJobs[iJobs].second;
        t_job posInsert = NEHInnerLoop(jobNumber);
        resultSchedule->addPseudoJob(0,nMachines,jobNumber,posInsert);
    }
}

shared_ptr<Schedule> NEH::getConstructive() {
    return getNEH();
}

shared_ptr<Schedule> LR::getLR(const t_job& x) {
    vector<t_job> initialList(nJobs);
    //generates 1 to nJobs permutation;
    generate(initialList.begin(), initialList.end(), [] { //es rapidoe sto?
        static t_job i = 0;
        return i++;
    });

    // Lamba function to sort by index function
    auto comp = [&](const t_job& iJob,const t_job& kJob)-> bool {
        double fnci = this->getIndexFunction(resultSchedule, iJob, initialList);
        double fnck = this->getIndexFunction(resultSchedule, kJob, initialList);
        if(fnci == fnck){
            double ITi = this->wTotalMachineiTime(resultSchedule, pseudoJob(0,nMachines,iJob));
            double ITk = this->wTotalMachineiTime(resultSchedule, pseudoJob(0,nMachines,kJob));
            return ITi < ITk;
        }
        return fnci < fnck;
    };

    sort(initialList.begin(), initialList.end(), comp);

//    cout << "Ordered List:\n";
//    for(auto u : initialList){
//        cout << u << " ";
//    }
//    cout << "\n";
    double minFlowTime = 100000000;


    for (t_job jobIndex = 0; jobIndex < x; ++jobIndex) {
        shared_ptr_pair_vector pair_vector_lr = localLR(initialList,jobIndex,0);
        shared_ptr<Schedule> NSchedule = pair_vector_lr->second;
        double thisTotalTime = NSchedule->getTotalFlowTime();
        if (thisTotalTime < minFlowTime) {
            minFlowTime = thisTotalTime;
            resultSchedule = NSchedule;
        }
    }
    return resultSchedule;
}

// checkea3
double LR::wTotalMachineiTime(const shared_ptr<Schedule>& S, const pseudoJob& nextElement) const{
    double IT = 0;
    double kFlow = S->getPermutationFlowTime(0,S->getSize()-1);
    double lastNewFlowTime = kFlow + instance(0,nextElement.job);
    for(t_machine iMachine = 1 ; iMachine < nMachines; ++iMachine){
        double weight = weightFunction(S->getSize()-1, iMachine);
        kFlow = S->getPermutationFlowTime(iMachine,S->getSize()-1);
        IT += weight * fmax(lastNewFlowTime - kFlow,0);
        // iJob is job to insert
        // Where to insert job, max of :
        // * After iJob on iMachine-1(lastNewFlowTime)
        // * After iJob-1 on iMachine(kFlow)
        lastNewFlowTime = max(lastNewFlowTime, kFlow) +
                instance(iMachine,nextElement.job);
    }
    return IT;
}

double LR::weightFunction(const t_job& iJob, const t_machine& iMachine) const{
    double dividend = nMachines;
    double divisor  = (iMachine+1) + (iJob+1) * (nMachines - (iMachine+1))/(nJobs - 2); // precision error?
    return dividend/divisor;
}

double getDivisor(double n){
    if(n==0)
        return 1;
    return n;
}

double LR::artificialFlowTime(const shared_ptr<Schedule>& S, const pseudoJob& nextElement, const vector<t_job>& U) const{
    assert(S->getSize() + U.size() == nJobs); // Correct number of elements
    double timeJob = 0;
    vector<double> nTimes;
    for(t_machine iMachine = 0 ; iMachine < nMachines ; ++iMachine) {
        timeJob = 0;
        for (auto iJob : U) {
            timeJob += instance(iMachine,iJob);
        }
        nTimes.push_back(timeJob/(nJobs - S->getSize() - 1)); // div by zero?
    }

    shared_ptr<Schedule> NS = make_shared<Schedule>(*(S));
    NS->addPseudoJob(nextElement);

    double lastFlowTime = NS->getPermutationFlowTime(0,NS->getSize()-1) + nTimes[0];
    for(t_machine iMachine = 1; iMachine < nMachines; ++iMachine){
        double kFlowtime = NS->getPermutationFlowTime(iMachine, NS->getSize()-1); //check
        lastFlowTime = max(lastFlowTime,kFlowtime) + nTimes[iMachine];
    }
    double kTotalTime = NS->getTotalFlowTime(); // C_{m,i}
    return kTotalTime + lastFlowTime;
}

double LR::getIndexFunction(const shared_ptr<Schedule>& S, const t_job& iJob, const vector<t_job>& U) const {
    double IT, AT, fnc;
    IT = wTotalMachineiTime(S, pseudoJob(0, nMachines, iJob));
    // Actual schedule + actual job is creating in artificial flowtime process
    AT = artificialFlowTime(S, pseudoJob(0, nMachines, iJob), U);
    fnc = (nJobs - S->getSize() - 2) * IT + AT;
    return fnc;
}

shared_ptr_pair_vector LR::localLR(const vector<t_job>& remainedJobs, const t_job& jobTaken, const t_job& uJobs) {

    shared_ptr<Schedule> S = make_shared<Schedule>(this->resultSchedule->pInstance);
    vector<t_job> U = remainedJobs;
    S->addPseudoJob(0, nMachines, U[jobTaken]);
    U.erase(U.begin() + jobTaken);

    double mFnc,fnc;
    t_job indexJob;
    while(U.size() > uJobs) {
        mFnc = 20000000000;
        if(U.size() > 1) {// to avoid errors
            for (t_job job = 0 ; job < U.size() ; ++job) {
                fnc = getIndexFunction(S, U[job], U);
                if (mFnc > fnc) {
                    mFnc = fnc;
                    indexJob = job;
                }
            }
        }
        else{
            indexJob = 0;
        }
        S->addPseudoJob(0,nMachines,U[indexJob]);
        U.erase( U.begin() + indexJob);

    }
    return make_shared< pair_vector >(make_pair(U,S));
}

shared_ptr<Schedule> LR::getConstructive() {
    t_job x; cin >> x;
    return getLR(x);
}

shared_ptr<Schedule> LRandNEH::getLRandNEH(const t_job& x) {
    vector<t_job> initialList(nJobs);
    //generates 1 to nJobs permutation;
    generate(initialList.begin(), initialList.end(), []{
        static t_job i = 0;
        return i++;
    });
    LR lrCaller(this->resultSchedule->pInstance);
    NEH nehCaller(this->resultSchedule->pInstance);

    // Lamba function to sort by index function
    auto comp = [&](const t_job& iJob,const t_job& kJob)-> bool {
        double fnci = lrCaller.getIndexFunction(resultSchedule, iJob, initialList);
        double fnck = lrCaller.getIndexFunction(resultSchedule, kJob, initialList);
        if(fnci == fnck){
            double ITi = lrCaller.wTotalMachineiTime(resultSchedule, pseudoJob(0,nMachines,iJob));
            double ITk = lrCaller.wTotalMachineiTime(resultSchedule, pseudoJob(0,nMachines,kJob));
            return ITi < ITk;
        }
        return fnci < fnck;
    };

    // Sorting by index function
    sort(initialList.begin(), initialList.end(), comp);


    double minFlowTime = 2000000000;
    for(t_job i = 0 ; i < x ; ++i){
        // LR Step
        t_job jobTaken = initialList[i];
        t_job portion = nJobs/4;
        lrCaller.resultSchedule->cleanSchedule();
        shared_ptr_pair_vector pair_vector_lrneh = lrCaller.localLR(initialList, jobTaken, portion); // Possible error
        shared_ptr<Schedule> initialSchedule = pair_vector_lrneh->second;
        //NEH Step
        nehCaller.resultSchedule->cleanSchedule();
        initialSchedule = nehCaller.getNEH(initialSchedule, pair_vector_lrneh->first);

        double thisTotalTime = initialSchedule->getTotalFlowTime();
        if(thisTotalTime < minFlowTime){
            minFlowTime = thisTotalTime;
            resultSchedule = make_shared<Schedule>(*initialSchedule);
//            resultSchedule = initialSchedule;
        }
    }
    return resultSchedule;
}

shared_ptr<Schedule> LRandNEH::getConstructive() {
    t_job x; cin >> x;
    return getLRandNEH(x);
}


shared_ptr<Schedule> FF::getFF(const t_job &x) {

    vector<t_job> initialList(nJobs);

    generate(initialList.begin(), initialList.end(), []{
        static t_job i = 0;
        return i++;
    });

    auto comp = [&](const t_job& iJob, const t_job& kJob)-> bool{
        double fnci = this->getIndexFunction(resultSchedule, iJob, initialList);
        double fnck = this->getIndexFunction(resultSchedule, kJob, initialList);
        if(fnci == fnck){
            double ITi = this->idleTime(resultSchedule, pseudoJob(0, nMachines, iJob));
            double ITk = this->idleTime(resultSchedule, pseudoJob(0, nMachines, kJob));
            return ITi < ITk;
        }
        return fnci < fnck;
    };

    sort(initialList.begin(), initialList.end(), comp);

    double minFlowTime = 100000000;

    for (t_job jobIndex = 0; jobIndex < x; ++jobIndex) {
        shared_ptr_pair_vector pair_vector_lr = localFF(initialList, jobIndex, 0);
        shared_ptr<Schedule> NSchedule = pair_vector_lr->second;
        double thisTotalTime = NSchedule->getTotalFlowTime();
        if (thisTotalTime < minFlowTime) {
            minFlowTime = thisTotalTime;
            resultSchedule = NSchedule;
        }
    }
    return resultSchedule;
}

shared_ptr_pair_vector FF::localFF(const vector<t_job> &remainedJobs, const t_job &jobTaken, const t_job &uJobs) {

    shared_ptr<Schedule> S = make_shared<Schedule>(this->resultSchedule->pInstance);
    vector<t_job> U = remainedJobs;
    S->addPseudoJob(0, nMachines, U[jobTaken]);
    U.erase(U.begin() + jobTaken);

    double mFnc, fnc;
    t_job indexJob;

    while(U.size() > uJobs){
        mFnc = 20000000000; // TODO: Check non-constant
        if(U.size() > 1){ // is it really necessary?
            for(t_job job = 0; job < U.size(); ++job){
                fnc = getIndexFunction(S, U[job], U);
                if (mFnc > fnc){
                    mFnc = fnc;
                    indexJob = job;
                }
            }
        }

        else {
            indexJob = 0;
        }
        S->addPseudoJob(0, nMachines, U[indexJob]);
        U.erase(U.begin() + indexJob);
    }

    return make_shared<pair_vector>(make_pair(U,S));

}

double FF::getIndexFunction(const shared_ptr<Schedule> &S, const t_job &iJob, const vector<t_job> &U) const {
    double IT, AT, fnc;
    IT = idleTime(S, pseudoJob(0, nMachines, iJob));
    // Actual schedule + actual job is creating in artificial flowtime process
    AT = completionTime(S, pseudoJob(0, nMachines, iJob));
    fnc = (double(nJobs - S->getSize() - 2)/double(const_a)) * IT + AT;
    return fnc;
}

double FF::completionTime(const shared_ptr<Schedule> &S, const pseudoJob &nextElement) const {
    shared_ptr<Schedule> NS = make_shared<Schedule>(*(S));
    NS->addPseudoJob(nextElement);
    return NS->getTotalFlowTime();
}

double FF::idleTime(const shared_ptr<Schedule> &S, const pseudoJob &nextElement) const {
    double IT = 0;
    double kFlow = S->getPermutationFlowTime(0, S->getSize()-1);
    double lastNewFlowTime = kFlow + instance(0, nextElement.job);

    for(t_machine iMachine = 1; iMachine < nMachines; ++iMachine){
        double weight = weightFunction(S->getSize()-1, iMachine);
        kFlow = S->getPermutationFlowTime(iMachine, S->getSize()-1);
        IT += weight * fmax(lastNewFlowTime - kFlow, 0);
        lastNewFlowTime = max(lastNewFlowTime, kFlow) + instance(iMachine, nextElement.job);
    }
    return IT;
}


double FF::weightFunction(const t_job& iJob, const t_machine& iMachine) const{
    double dividend = nMachines;
    double divisor = (iMachine+1) - const_b + (iJob+1) * (nMachines - (iMachine+1) + const_b)/(nJobs-2);
    return dividend/divisor;
}

shared_ptr<Schedule> FF::getConstructive() {
    t_job x; cin >> x;
    return getFF(x);
}