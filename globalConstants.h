//
// Created by joaquin on 05/11/18.
//

#ifndef NFSSP_GLOBALCONSTANTS_H
#define NFSSP_GLOBALCONSTANTS_H

#include <cstdint>

//extern const int maxLength;
//typedef uint_fast32_t t_time;      // 0,..., (n+m)tmax
using t_time = uint_fast64_t;
using t_job = int_fast16_t;
using t_machine = int_fast16_t;
using t_flow_time = int_fast64_t;
using t_size_type = uint_fast32_t;

//
//typedef uint_fast32_t t_time;      // 0,..., (n+m)tmax
//typedef int_fast32_t t_operation;  // 1,2,..., mn   // 1: first operation; mn: last operation;
//typedef int_fast32_t st_operation; // used to check signed conversions
//typedef int_fast16_t t_job;        // 0,1,...,n-1   // j = (o-1)/mMach
//typedef int_fast8_t t_mach;        // 1,2,..., m    // m = (o-1)%mMach +1
//typedef uint_fast32_t t_size_type; // unsigned value in vector resize and reserve

//using
#endif //NFSSP_GLOBALCONSTANTS_H
