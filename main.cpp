#include <iostream>
#include "schedule.h"
#include "constructiveHeuristics.h"
#include "chrono"
#include <iomanip>

#define MAXINSTANCE 500
#define NANOSECOND 1000000000
using namespace std;
typedef vector< vector< uint > > iType;

//instance: nMachines Rows, nJobs Colums
// TODO: change matrix traverse order.
int main() {
    int nMachines,nJobs;
    cin >>  nJobs >> nMachines;
    vector< vector<int> > instance(nMachines, vector<int>(nJobs));
    //traverse matrix
    for(int i = 0 ; i < nMachines; ++i){
        for(int j = 0 ; j < nJobs; ++j){
            cin >> instance[i][j];
        }
    }
    int option; cin >> option;
// Option 0: LR
// Option 1: NEH
// Option 2: LR-NEH
    if(option == 0){
        int nSequences; cin >> nSequences;
        std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();
        LR lr(nMachines,nJobs,instance);
        cout << "Total Flowtime = " <<  lr.getLR(nSequences).getTotalFlowTime() << "\n";
        std::chrono::steady_clock::time_point end= std::chrono::steady_clock::now();
        cout << "Time difference = " << std::fixed << setprecision(10) << std::chrono::duration_cast<std::chrono::nanoseconds> (end - begin).count() <<" nanoseconds\n";
    }
    else if(option == 1){
        NEH n(nMachines,nJobs,instance);
        std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();
        cout << "Total Flowtime = " <<  n.getNEH().getTotalFlowTime() << "\n";
        std::chrono::steady_clock::time_point end= std::chrono::steady_clock::now();
        cout << "Time difference = " << std::fixed << setprecision(10) << std::chrono::duration_cast<std::chrono::nanoseconds> (end - begin).count() <<" nanoseconds\n";
    }
    else if(option == 2){
        int nSequences; cin >> nSequences;
        LRandNEH lrneh(nMachines,nJobs,instance);
        std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();
        cout << "Total Flowtime = " <<  lrneh.getLRandNEH(nSequences).getTotalFlowTime() << "\n";
        std::chrono::steady_clock::time_point end= std::chrono::steady_clock::now();
        cout << "Time difference = " << std::fixed << setprecision(10) << std::chrono::duration_cast<std::chrono::nanoseconds> (end - begin).count() <<" nanoseconds\n";

    }
    return 0;
}