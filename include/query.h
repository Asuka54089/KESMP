#ifndef QUERY_H_
#define QUERY_H_

#include "../include/data.h"
#include "../include/kesmp.h"
#include "../include/kesmp_enum.h"
#include "../include/kesmp_enumstar.h"
#include "../include/kesmp_layer.h"
#include "../include/kesmp_subset.h"
#include "../include/kesmp_hybrid.h"
#include "../include/kesmp_approx.h"

class Query
{
public:
    vector<int> rotation_weights;
    vector<int> rotation_depths;
    set<pair<int, int>> rotation_edges;

    clock_t start_time, end_time;
    double runtime;
    vector<ClosedSubset> results;

    Query(string rotation_file, string results_file);
    string results_file;

    //method0
    void query_enum(int k, bool verbose_case, bool verbose_result);
    //method1
    void query_enumstar(int k, bool verbose_case, bool verbose_result);
    //method2
    void query_layer(int k, bool verbose_case, bool verbose_result);
    //method3
    void query_subset(int k, bool verbose_case, bool verbose_result);
    //method4
    void query_hybrid(int k, bool verbose_case, bool verbose_result);
    //method5 Heuristic positive rotations
    void query_hpr(int k, bool verbose_case, bool verbose_result);
    //method6 Heuristic positive rotations + subset
    void query_hprs(int k, bool verbose_case, bool verbose_result);

    void query(int k, int method, string rotation_file, string result_file);
};

#endif
