//
// Created by joaquin on 25/11/18.
//

#ifndef NFSSP_METAHEURISTIC_H
#define NFSSP_METAHEURISTIC_H
#include "constructiveHeuristics.h"
#include "localSearch.h"
#include "schedule.h"

class ILS {
    shared_ptr<ProblemInstance> pInstance;
    t_job perturbNumber;
    t_job repetitions;
//    t_job nJobs;
//    t_job nMachines;
//    LRandNEH lrCaller;
//    shiftLocalSearch shift;

public:
    ILS(const shared_ptr<ProblemInstance>& pInstance, const t_job& perturbNumber, const t_job& repetitions);
    shared_ptr<Schedule> getMetaheuristic();
    void perturbPermutation(shared_ptr<Schedule> & resultSchedule);
};


#endif //NFSSP_METAHEURISTIC_H
