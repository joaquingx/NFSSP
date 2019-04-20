#include <iostream>
#include "schedule.h"
#include "constructiveHeuristics.h"
#include "chrono"
#include <iomanip>
#include <memory>
#include "Matrix.h"
#include "globalConstants.h"
#include "metaheuristic.h"
#define MAXINSTANCE 500
#define NANOSECOND 1000000000
using namespace std;
typedef vector< vector< uint > > iType;
//instance: nMachines Rows, nJobs Colums


void measureMetaheuristic(const shared_ptr<ProblemInstance>& pInstance ){
    unique_ptr<ILS> meta = make_unique<ILS>(pInstance,5,5);
    chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();
    cout << "Total Flowtime = " << meta->getMetaheuristic()->getTotalFlowTime() << "\n";
    chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
    cout << "Time difference = " << std::fixed << setprecision(10) << std::chrono::duration_cast<std::chrono::nanoseconds> (end - begin).count() <<" nanoseconds\n";
}

void measureConstructive(){
    shared_ptr<ProblemInstance> pInstance(new ProblemInstance());
    cin >> pInstance;
    unique_ptr<cHeuristic> heuristic;
    int opc; cin >> opc;
    switch(opc){
        case 0:
            heuristic = make_unique<LR>(pInstance);
            break;
        case 1:
            heuristic = make_unique<NEH>(pInstance);
            break;
        case 2:
            heuristic = make_unique<LRandNEH>(pInstance);
            break;
        case 3:
            measureMetaheuristic(pInstance);
            return;
        case 4:
            heuristic = make_unique<FF>(pInstance);
            break;
    }
    chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();
    cout << "Total Flowtime = " << heuristic->getConstructive()->getTotalFlowTime() << "\n";
    chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
    cout << "Time difference = " << std::fixed << setprecision(10) << std::chrono::duration_cast<std::chrono::nanoseconds> (end - begin).count() <<" nanoseconds\n";
}


int main() {
    measureConstructive();
//    measureMetaheuristic();
    return 0;
}