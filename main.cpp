#include <iostream>
#include "schedule.h"
#include "constructiveHeuristics.h"
#define MAXINSTANCE 500
using namespace std;
typedef vector< vector< uint > > iType;

//instance: nMachines Rows, nJobs Colums
// TODO: change matrix traverse order.
int main() {
    int nMachines,nJobs;
    cin >> nMachines >> nJobs;
    vector< vector<int> > instance(nMachines, vector<int>(nJobs));
    //traverse matrix
    for(int i = 0 ; i < nMachines; ++i){
        for(int j = 0 ; j < nJobs; ++j){
            cin >> instance[i][j];
        }
    }
    randomPermutation r(nMachines,nJobs,instance);
    LR lr(nMachines,nJobs,instance);
    arbitraryPermutation a(nMachines,nJobs,instance);
    NEH n(nMachines,nJobs,instance);
//    Schedule s = r.getRandomPermutation();
//    Schedule s = lr.getLR(3);
    Schedule s = n.getNEH();
    s.printPermutationSchedule();
    s.printGantt();
    cout << "Total Flowtime: "  << s.getTotalFlowTime() << "\n";
    return 0;
}