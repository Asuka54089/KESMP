#ifndef KESMP_ENUMSTARLR_H_
#define KESMP_ENUMSTARLR_H_

#include "kesmp.h"

class KESMP_LR : public KESMP
{
public:
    KESMP_LR() : KESMP(){};
    void init_method();

    //Method 4: ENUM*-LR
    vector<ClosedSubset> find_topk_S(int k);
    void expand_antichain(set<int> A, vector<int> RA, int pos, int k);
    void package_results(const string &results_file, double runtime, vector<ClosedSubset> kesm_results);
};

#endif