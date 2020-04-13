//
// Created by joaquin on 16/10/18.
//

#include <bits/stdc++.h>
#define N 80
#define DIV 10
#define HEURISTICS 1
#define INF 1000000000
using namespace std;

string pruebas[] = {"20x5", "20x10", "20x20", "50x5", "50x10", "50x20", "100x5",
                    "100x10", "100x20", "200x10", "200x20", "500x20"};

void fillWithZeros(double * arr){
    for(int i = 0 ;i < HEURISTICS ; ++i){
        arr[i] = 0;
    }
}

void getTimes(){
    double rpiHeuristics[HEURISTICS], totalRpiHeuristics[HEURISTICS];
    double results[HEURISTICS];
    int clusters = N/DIV;
    fillWithZeros(rpiHeuristics);
    cout << "Times:\n";
    cout << "        LR-10    LR-5  LR-NEH-10   LR-NEH-5  ILS NEH\n";
    for(int i = 0 ; i < clusters ; ++i) {
        fillWithZeros(rpiHeuristics);
        for(int k = 0; k < DIV ; ++k) {
            double mini = INF, rpi;
            for (int j = 0; j < HEURISTICS; ++j) {
                cin >> results[j];
                mini = min(mini,results[j]);
            }
            for (int j = 0; j < HEURISTICS; ++j) {
                rpiHeuristics[j] += results[j];
            }
        }
        cout << pruebas[i] << ": ";
        for (int j = 0; j < HEURISTICS; ++j) {
            printf("%.3f ", rpiHeuristics[j]/DIV/INF/35);
            totalRpiHeuristics[j] += rpiHeuristics[j]/DIV/INF/35;
        }
        cout << "\n";
    }
    cout << "LR-10    LR-5  LR-NEH-10   LR-NEH-5  ILS    NEH\n";
    for(int i = 0 ; i < HEURISTICS; ++i){
        printf("%.3f ",totalRpiHeuristics[i]/clusters);
    }
    cout << "\n";
}

void getRPIs() {
    double rpiHeuristics[HEURISTICS], totalRpiHeuristics[HEURISTICS];
    double results[HEURISTICS];
    int clusters = N/DIV;
    fillWithZeros(totalRpiHeuristics);
    cout << "RPI:\n";
    cout << "        BSCH  FF-10  FF-5  FF-NEH-10  FF-NEH-5  LR-10  LR-5  LR-NEH-10  LR-NEH-5  META  NEH\n";
    for(int i = 0 ; i < clusters ; ++i) {
        fillWithZeros(rpiHeuristics);
        for(int k = 0; k < DIV ; ++k) {
            double mini = INF, rpi;
            for (int j = 0; j < HEURISTICS; ++j) {
                cin >> results[j];
//                cout << j << ": " << results[j] << "\n";
                mini = min(mini, results[j]);
            }
            for (int j = 0; j < HEURISTICS; ++j) {
                rpiHeuristics[j] += (results[j]-mini)/mini * 100;
            }
        }
        cout << pruebas[i] << ": ";
        for (int j = 0; j < HEURISTICS; ++j) {
            printf("%.3f ", rpiHeuristics[j]/DIV);
            totalRpiHeuristics[j] += rpiHeuristics[j]/DIV;
        }
        cout << "\n";
    }
    cout << "        LR-10    LR-5  LR-NEH-10   LR-NEH-5   ILS     NEH\n";
    for(int i = 0 ; i < HEURISTICS; ++i){
        printf("%.3f ", totalRpiHeuristics[i]/clusters);
    }
    cout << "\n";
}

int main(){
    getTimes();
//    getRPIs();
    return 0;
}
