#ifndef ALG_ENUMSTARL_H_
#define ALG_ENUMSTARL_H_

#include "kesmp.h"
#include "bound_mincut.h"


class EnumStarL : public KESMP
{
public:

    EnumStarL(string instance_dir) : KESMP(instance_dir){};

    void init_method();

    double mincut_time;

    // Method 2: ENUM*-L
    vector<ClosedSubset> find_topk_closedsubsets(int k);
    void expand_antichain(set<int> A, vector<int> RA, int pos, int k);

    void package_results(const string &results_file, double runtime, vector<ClosedSubset> kesm_results);
};

#endif