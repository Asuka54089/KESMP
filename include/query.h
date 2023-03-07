#ifndef QUERY_H_
#define QUERY_H_

#include <utility>
#include <vector>
#include <set>

#include "../include/utils.h"
#include "../include/rotation.h"

#include "../include/kesmp.h"

#include "../include/kesmp_enum.h"
#include "../include/kesmp_enumstar.h"

#include "../include/kesmp_enumstarL.h"
#include "../include/kesmp_enumstarR.h"
#include "../include/kesmp_enumstarLR.h"

#include "../include/kesmp_enumstarH.h"
#include "../include/kesmp_enumstarHLR.h"

class Query
{
public:

    // input instance
    vector<vector<int>> male_prefers;
    vector<vector<int>> female_prefers;
    vector<int> male_opt_matching,female_opt_matching;
    set<pair<int, int>> rotation_edges;
    vector<Rotation> rotations;

    string results_file;

    clock_t start_time, end_time;
    double runtime;
    vector<ClosedSubset> results;

    Query(string preference_file,string rotation_file,string matching_file, string results_file);
    
    void query(int method, int k);

    void enumeration(int k);
    void enumstar(int k);
    void enumstar_layer(int k);
    void enumstar_rotation(int k);

    void enumstar_lr(int k);

    void enumstar_heuristic(int k);
    void enumstar_hlr(int k);
    

};

#endif
