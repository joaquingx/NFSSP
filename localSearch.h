//
// Created by joaquin on 25/11/18.
//
#ifndef NFSSP_LOCALSEARCH_H
#define NFSSP_LOCALSEARCH_H

#include <memory>
#include "constructiveHeuristics.h"
#include "schedule.h"

class localSearch {

};

class shiftLocalSearch{
    shared_ptr<Schedule> resultSchedule;
    t_machine repetitions;
    t_job shiftJob(const t_job& actJob); // returns the best shift option
public:
    shiftLocalSearch(const shared_ptr<Schedule>& resultSchedule, const t_machine& repetitions);
    shared_ptr<Schedule> getLocalSearch();
};


#endif //NFSSP_LOCALSEARCH_H
