#ifndef KESMP_ENUMSTARHLR_H_
#define KESMP_ENUMSTARHLR_H_

#include "kesmp.h"

class KESMP_HLR : public KESMP
{
public:
    KESMP_HLR() : KESMP(){};
    void init_method();

    int candidate_size; 

    //Method 6: ENUM*-H-LR
    vector<ClosedSubset> find_topk_S(int k);
    void expand_antichain(set<int> A, vector<int> RA, int pos, int k);
    void package_results(const string &results_file, double runtime, vector<ClosedSubset> kesm_results);
};

#endif