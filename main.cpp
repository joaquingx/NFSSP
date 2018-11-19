#include <iostream>
#include "schedule.h"
#include "constructiveHeuristics.h"
#include "chrono"
#include <iomanip>
#include <memory>
#include "Matrix.h"
#include "globalConstants.h"

#define MAXINSTANCE 500
#define NANOSECOND 1000000000
using namespace std;
typedef vector< vector< uint > > iType;

//constexpr int maxLength = 1000000;

//instance: nMachines Rows, nJobs Colums
// TODO: change matrix traverse order.
int main() {
//    t_time a;
    shared_ptr<ProblemInstance> pibe(new ProblemInstance());
    cin >> pibe;

//    for(int i = 0 ; i < pibe->nMachines; ++i){
//        for(int j = 0 ; j < pibe->nJobs; ++j){
//            cin >> pibe->instance(i,j);
//        }
//    }
//    NEH r(pibe);
    LR r(pibe);
//    randomPermutation r(pibe);
    shared_ptr<Schedule> p = r.getLR(1);
//    shared_ptr<Schedule> p = r.getNEH();
    p->printPermutationSchedule();
    p->printGantt();
    cout << p->getTotalFlowTime() <<  "\n";
//    cout << "pero que recarajeanos\n";

    return 0;
    int nMachines,nJobs, nSequences;
    cin >>  nJobs >> nMachines;
    Matrix instance(nMachines, nJobs);
    //traverse matrix
    for(int i = 0 ; i < nMachines; ++i){
        for(int j = 0 ; j < nJobs; ++j){
            cin >> instance(i,j);
        }
    }
//    instance(100,100);
    cout << instance;

//    NEH n(nMachines,nJobs,instance);
//    std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();
//    Schedule * lele = n.getNEH();
//    cout << lele->getTotalFlowTime() << "\n";
//
//    cout << "Total Flowtime = " <<  n.getNEH().getTotalFlowTime() << "\n";
//    std::chrono::steady_clock::time_point end= std::chrono::steady_clock::now();
//    cout << "Time difference = " << std::fixed << setprecision(10) << std::chrono::duration_cast<std::chrono::nanoseconds> (end - begin).count() <<" nanoseconds\n";

//    nSequences=2;
//    LR * lr = new LR(nMachines,nJobs,instance);
//    std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();

//    cout << "Total Flowtime = " <<  lr->getLR(3).getTotalFlowTime() << "\n";
//    std::chrono::steady_clock::time_point end= std::chrono::steady_clock::now();
//    cout << "Time difference = " << std::fixed << setprecision(10) << std::chrono::duration_cast<std::chrono::nanoseconds> (end - begin).count() <<" nanoseconds\n";
//    int option; cin >> option;

//    cin >> nSequences;
//    LR lr(nMachines,nJobs,instance);
//    lr.getLR(nSequences);
//    cout << "Total Flowtime = " <<  lr.getLR(nSequences)->getTotalFlowTime() << "\n";
//    cout << "\n";
// Option 0: LR
// Option 1: NEH
// Option 2: LR-NEH
//    if(option == 0){
//        int nSequences; cin >> nSequences;
//        std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();
//        LR lr(nMachines,nJobs,instance);
//        cout << "Total Flowtime = " <<  lr.getLR(nSequences).getTotalFlowTime() << "\n";
//        std::chrono::steady_clock::time_point end= std::chrono::steady_clock::now();
//        cout << "Time difference = " << std::fixed << setprecision(10) << std::chrono::duration_cast<std::chrono::nanoseconds> (end - begin).count() <<" nanoseconds\n";
//    }
//    else if(option == 1){
//        NEH n(nMachines,nJobs,instance);
//        std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();
//        cout << "Total Flowtime = " <<  n.getNEH().getTotalFlowTime() << "\n";
//        std::chrono::steady_clock::time_point end= std::chrono::steady_clock::now();
//        cout << "Time difference = " << std::fixed << setprecision(10) << std::chrono::duration_cast<std::chrono::nanoseconds> (end - begin).count() <<" nanoseconds\n";
//    }
//    else if(option == 2){
//        int nSequences; cin >> nSequences;
//        LRandNEH lrneh(nMachines,nJobs,instance);
//        std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();
//        cout << "Total Flowtime = " <<  lrneh.getLRandNEH(nSequences).getTotalFlowTime() << "\n";
//        std::chrono::steady_clock::time_point end= std::chrono::steady_clock::now();
//        cout << "Time difference = " << std::fixed << setprecision(10) << std::chrono::duration_cast<std::chrono::nanoseconds> (end - begin).count() <<" nanoseconds\n";
//
//    }
    return 0;
}