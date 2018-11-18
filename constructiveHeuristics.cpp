#include <utility>

#include <random>
#include <assert.h>
//
// Created by joaquin on 18/09/18.
//

#include <algorithm>
#include <iostream>
#include <map>
#include "constructiveHeuristics.h"
#include "schedule.h"
//
//Schedule arbitraryPermutation::getArbitraryPermutation() {
//    int resp[] = {89,73,46,83,145,87,138,104,164,124,92,125,38,4,148,188,65,58,139,1,78,22,64,24,159,53,195,151,32,191,18,167,86,194,26,120,199,23,175,29,130,174,55,81,59,131,117,189,187,43,9,160,33,108,71,75,28,16,141,84,2,90,66,115,25,121,17,99,176,178,152,122,91,10,161,156,193,119,80,127,7,95,79,181,82,142,114,129,69,133,56,42,109,102,146,170,45,13,98,36,52,113,20,155,150,72,179,154,112,140,27,97,61,6,3,166,15,47,50,126,54,93,177,123,153,96,135,8,41,39,186,169,62,165,198,94,0,34,182,103,100,190,70,157,196,192,134,163,137,57,49,5,60,132,106,76,143,185,44,11,136,74,180,31,88,128,12,147,35,77,101,162,14,21,30,67,68,184,105,116,110,85,171,40,107,37,173,51,19,197,183,111,118,149,158,144,168,48,172,63};
////    int resp[] = {89,73,63,120,22,167,38,164,104,199,125,148,45,194,90,115,178,188,82,154,17,4,145,58,32,98,152,26,83,193,195,72,75,76,79,84,57,176,151,130,97,78,187,23,198,25,129,102,189,16,179,140,10,66,157,121,36,127,50,131,141,169,71,159,113,139,1,95,59,172,110,133,61,13,93,143,86,165,28,92,168,190,8,111,100,144,137,186,74,116,33,182,153,81,67,77,99,91,171,24,35,101,68,114,166,191,9,70,62,181,48,65,12,175,196,27,170,49,103,150,180,40,54,136,107,53,34,155,2,108,94,15,3,197,11,134,19,52,96,105,56,160,42,106,126,156,162,39,149,161,146,43,0,173,158,177,123,64,55,128,118,138,184,119,31,80,47,7,20,21,163,109,135,51,60,41,142,192,5,174,87,185,132,29,69,112,85,147,18,117,122,44,14,30,88,37,6,183,124,46};
//    for (int r: resp){
//        resultSchedule->addPseudoJob(0,nMachines,r);
//    }
////    for(int i = 0 ; i < nJobs; ++i){
////        int job; cin >>job;
////        resultSchedule->addPseudoJob(0,nMachines,job);
////    }
//    return *resultSchedule;
//}
//

//
//
//Schedule LRandNEH::getLRandNEH(int x) {
//    vector<int> initialList(nJobs);
//    //generates 1 to nJobs permutation;
//    generate(initialList.begin(), initialList.end(), []{
//        static int i = 0;
//        return i++;
//    });
//    LR lrCaller(nMachines,nJobs,instance);
//    NEH nehCaller(nMachines,nJobs,instance);
//    // Sorting by index function
//    sort(initialList.begin(), initialList.end(), cLR(nMachines,nJobs,initialList,instance));
////    cout << "Ordered List:\n";
////    for(auto u : initialList){
////        cout << u << " ";
////    }
////    cout << "\n";
//    Schedule * finalSchedule = new Schedule();
//    double minFlowTime = 2000000000;
//    for(int i = 0 ; i < x ; ++i){
//        vector<int> cpyInitialList = initialList;
//        Schedule initialSchedule(nMachines,nJobs,instance);
//
//        // LR Step
//        int jobTaken = initialList[i];
//        initialSchedule.addPseudoJob(0,nMachines,jobTaken);
//        cpyInitialList.erase( find(cpyInitialList.begin(),cpyInitialList.end(),jobTaken) );
//        int portion = nJobs/4;
////        initialSchedule = lrCaller.localLR(initialSchedule,cpyInitialList,portion); //TODO: change this, idk what happens here
//
////        cout << "Despues:\n";
////        for(int job: cpyInitialList){
////            cout << job << " ";
////        }
////        cout << "\n";
//        //NEH Step
//        initialSchedule = nehCaller.getNEH(initialSchedule,cpyInitialList);
//
//        double thisTotalTime = initialSchedule.getTotalFlowTime();
//        if(thisTotalTime < minFlowTime){
//            minFlowTime = thisTotalTime;
//            (*finalSchedule) = initialSchedule;
//        }
//    }
//    return (*finalSchedule);
//}

cHeuristic::cHeuristic(const shared_ptr<ProblemInstance> &pInstance): resultSchedule(make_shared<Schedule>(pInstance)),
                                                                      instance(pInstance->instance), nMachines(pInstance->nMachines),
                                                                      nJobs(pInstance->nJobs){}



shared_ptr<Schedule> randomPermutation::getRandomPermutation() {
    vector<int> toShuffle(resultSchedule->pInstance->nJobs);
    //generates 1 to nJobs permutation;
    generate(toShuffle.begin(), toShuffle.end(), []{
        static int i = 0;
        return i++;
    });
    shuffle(toShuffle.begin(), toShuffle.end(), std::mt19937(std::random_device()()));
    for(t_job i = 0 ; i < resultSchedule->pInstance->nJobs; ++i){
        resultSchedule->addPseudoJob(0,resultSchedule->pInstance->nMachines,toShuffle[i]);
    }
    return resultSchedule;
}

shared_ptr<Schedule> NEH::getNEH(Schedule & S, vector<t_job> & U) {
    vNEH orderedJobs;
    (*resultSchedule) = S;
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
    for(int iMachine = 0 ; iMachine < nMachines ; ++iMachine){
        for(int iJob = 0; iJob < nJobs ; ++iJob){
            totalProcessingTimes[iJob] += instance(iMachine,iJob);
        }
    }

    for(int iJob = 0 ; iJob < nJobs ; ++iJob){
        orderedJobs.emplace_back(totalProcessingTimes[iJob],iJob);
    }
    sort(orderedJobs.begin(), orderedJobs.end());
    NEHOuterLoop(orderedJobs);
    return resultSchedule;
//    return  new Schedule();
//    resultSchedule = nullptr;
//    return 5;
}


t_job NEH::NEHInnerLoop(const t_job& jobNumber) {
    int iWinner = 0, tftWinner = INF;
//    Schedule * copySchedule = resultSchedule;
    for(t_job iJobs = 0 ; iJobs < resultSchedule->getSize() ; ++iJobs){
        resultSchedule->addPseudoJob(0,nMachines,jobNumber,iJobs);
        t_flow_time actualTFT = resultSchedule->getTotalFlowTime();
        if(actualTFT < tftWinner){
            iWinner = iJobs;
            tftWinner = actualTFT;
        }
        resultSchedule->removePseudoJob(iJobs);
    }
    return iWinner;
}

void NEH::NEHOuterLoop(const vNEH& orderedJobs) {
    //O(n^3*m)
    // non-descending order better for tft
    for(int iJobs = orderedJobs.size()-1 ; iJobs >= 0 ; --iJobs){
//    for(int iJobs = 0 ; iJobs < orderedJobs.size() ; ++iJobs){ // trying
        int jobNumber = orderedJobs[iJobs].second;
        int posInsert = NEHInnerLoop(jobNumber);
        resultSchedule->addPseudoJob(0,nMachines,jobNumber,posInsert);
    }
}
//struct cLR {
//    Schedule * S;
//    int nJobs,nMachines;
//    vector<int> U;
//    LR * caller;
//    cLR (int nMachines, int nJobs, vector<int> U, const Matrix & instance) {
//        this->nJobs = nJobs;
//        this->nMachines = nMachines;
//        this->U = std::move(U);
//        this->S = new Schedule(nMachines,nJobs,instance);
//        this->caller = new LR(nMachines,nJobs,instance);
//    }
//    bool operator () (int iJob, int kJob) {
//        double fnci = caller->getIndexFunction((*S),iJob,U);
//        double fnck = caller->getIndexFunction((*S),kJob,U);
//        if(fnci == fnci){ // break ties by IT's
//            double ITi = caller->wTotalMachineiTime((*S),pseudoJob(0,nMachines,iJob));
//            double ITk = caller->wTotalMachineiTime((*S),pseudoJob(0,nMachines,kJob));
//            return ITi < ITk;
//        }
//        return fnci < fnck;
//    }
//};

shared_ptr<Schedule> LR::getLR(const t_job& x) {
    vector<t_job> initialList(nJobs);
    //generates 1 to nJobs permutation;
    generate(initialList.begin(), initialList.end(), [] { //es rapidoe sto?
        static int i = 0;
        return i++;
    });

    // Ordenando segun index function
//    sort(initialList.begin(), initialList.end(), cLR(nMachines,nJobs,initialList,instance));

//    cout << "Ordered List:\n";
//    for(auto u : initialList){
//        cout << u << " ";
//    }
//    cout << "\n";
    double minFlowTime = 100000000;
    for (int i = 0; i < x; ++i) {
        int jobTaken = initialList[i];
        shared_ptr<Schedule> NSchedule = localLR(initialList,jobTaken,0);
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
    //it's a fact.
    //init of flowtimes.
    // getPermutationflow is the problem?
    double kFlow = S->getPermutationFlowTime(0,S->getSize()-1);// can cause troubles? check
    double lastNewFlowTime = kFlow + instance(0,nextElement.job);
    for(int iMachine = 1 ; iMachine < nMachines; ++iMachine){
        double weight = weightFunction(S->getSize()-1, iMachine);
        kFlow = S->getPermutationFlowTime(iMachine,S->getSize()-1);// can cause troubles? check
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
//    ++iJob;++iMachine; // To avoid div by 0
    double dividend = nMachines;
    double divisor  = iMachine + (iJob+1) * (nMachines - (iMachine+1))/(nJobs - 2); // precision error?
    return dividend/divisor;
}

double getDivisor(double n){
    if(n==0)
        return 1;
    return n;
}

double LR::artificialFlowTime(const shared_ptr<Schedule>& S, const pseudoJob& nextElement, const vector<t_job>& U) const{
    // "Creating" artificial Job
    cout << S->getSize() << "\n";
    cout << U.size() << "\n";
    assert(S->getSize() + U.size() == nJobs); // Correct number of elements
    double timeJob = 0;
    vector<double> nTimes;
    int siz = S->getSize();
    for(int iMachine = 0 ; iMachine < nMachines ; ++iMachine) {
        timeJob = 0;
        for (auto iJob : U) {
            timeJob += instance(iMachine,iJob);
        }
        nTimes.push_back(timeJob/(nJobs - S->getSize() - 1)); // div by zero?
    }

    // Calculating C_{m,p}
    shared_ptr<Schedule> NS = make_shared<Schedule>(Schedule(this->resultSchedule->pInstance));
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
    double IT,AT,fnc;
//    cout << "\n";
    IT = wTotalMachineiTime(S, pseudoJob(0, nMachines, iJob));
    // Actual schedule + actual job is creating in artificial flowtime process
    AT = artificialFlowTime(S, pseudoJob(0, nMachines, iJob), U);
    fnc = (nJobs - S->getSize() - 2) * IT + AT;
    return fnc;
}

shared_ptr<Schedule> LR::localLR(const vector<t_job>& remainedJobs, const t_job& jobTaken, const t_job& uJobs) {

    shared_ptr<Schedule> S = make_shared<Schedule>(this->resultSchedule->pInstance);
    S->addPseudoJob(0, nMachines, jobTaken);
    vector<t_job> U = remainedJobs;
    U.erase(find(U.begin(), U.end(), jobTaken));

    double mFnc,fnc;
    int indexJob;
    while(U.size() > uJobs) {
        mFnc = 20000000000;
        if(U.size() > 1) {// to avoid errors
            for (auto job: U) {
                fnc = getIndexFunction(S,job,U);
                if (mFnc > fnc) {
                    mFnc = fnc;
                    indexJob = job;

                }
            }
        }
        else{
            indexJob = U[0];
        }
        U.erase( find(U.begin(),U.end(),indexJob) );//error?
        S->addPseudoJob(0,nMachines,indexJob);
    }
    return S;
}

//LR::LR() = default;

