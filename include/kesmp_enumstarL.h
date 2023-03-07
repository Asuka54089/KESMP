#ifndef KESMP_LAYER_H_
#define KESMP_LAYER_H_

#include "kesmp.h"
#include "bound_mincut.h"


class KESMP_Layer : virtual public KESMP
{
public:
    KESMP_Layer() : KESMP(){};

    void init_method();

    // Method 2: ENUM*-L
    vector<ClosedSubset> find_topk_S(int k);
    void expand_antichain(set<int> A, vector<int> RA, int pos, int k);

    void package_results(const string &results_file, double runtime, vector<ClosedSubset> kesm_results);
};

#endif