#include <iostream>
#include "schedule.h"
#include "constructiveHeuristics.h"
#define MAXINSTANCE 500
using namespace std;
typedef vector< vector< uint > > iType;

//instance: nMachines Rows, nJobs Colums

int main() {
    uint nMachines,nJobs;
    cin >> nMachines >> nJobs;
    vector< vector<uint> > instance(nMachines, vector<uint>(nJobs));
    //traverse matrix
    for(int i = 0 ; i < nMachines; ++i){
        for(int j = 0 ; j < nJobs; ++j){
            cin >> instance[i][j];
        }
    }
    randomPermutation r(nMachines,nJobs,instance);
//    arbitraryPermutation a(nMachines,nJobs,instance);
    Schedule s = r.getRandomPermutation();
    s.printPermutationSchedule();
    s.printGantt();
    cout << "My clear mehtod obtains: "  << s.getTotalFlowTime() << "\n";
    return 0;
}