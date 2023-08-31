#ifndef QUERY_H_
#define QUERY_H_

#include <utility>
#include <vector>
#include <set>

#include "../include/utils.h"
#include "../include/entities_rotation.h"

#include "../include/kesmp.h"

#include "../include/alg_enum.h"
#include "../include/alg_enumstar.h"

#include "../include/alg_enumstarL.h"
#include "../include/alg_enumstarR.h"
#include "../include/alg_enumstarLR.h"

#include "../include/alg_enumstarH.h"
#include "../include/alg_enumstarHLR.h"

class Query
{
public:

    // input instance
    string instance_dir;


    clock_t start_time, end_time;
    double runtime;
    vector<ClosedSubset> results;

    Query(string instance_dir);
    
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
