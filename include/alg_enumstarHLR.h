#ifndef ALG_ENUMSTARHLR_H_
#define ALG_ENUMSTARHLR_H_

#include "kesmp.h"

class EnumStarHLR : public KESMP
{
public:
    EnumStarHLR(string instance_dir) : KESMP(instance_dir){};
    void init_method();

    int candidate_size;

    //Method 6: ENUM*-H-LR
    vector<ClosedSubset> find_topk_closedsubsets(int k);
    void expand_antichain(set<int> A, vector<int> RA, int pos, int k);
    void package_results(const string &results_file, double runtime, vector<ClosedSubset> kesm_results);
};

#endif