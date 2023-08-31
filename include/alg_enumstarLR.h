#ifndef ALG_ENUMSTARLR_H_
#define ALG_ENUMSTARLR_H_

#include "kesmp.h"

class EnumStarLR : public KESMP
{
public:
    EnumStarLR(string instance_dir) : KESMP(instance_dir){};

    void init_method();

    //Method 4: ENUM*-LR
    vector<ClosedSubset> find_topk_closedsubsets(int k);
    void expand_antichain(set<int> A, vector<int> RA, int pos, int k);
    void package_results(const string &results_file, double runtime, vector<ClosedSubset> kesm_results);
};

#endif