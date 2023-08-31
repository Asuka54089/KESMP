#ifndef ALG_ENUMSTARH_H_
#define ALG_ENUMSTARH_H_

#include "kesmp.h"

class EnumStarH : public KESMP
{
public:
    EnumStarH(string instance_dir) : KESMP(instance_dir){};

    int candidate_size; //# (w+ rotations + w0 rotations)
    
    void init_method();

    
    // Method 5: ENUM*-H
    vector<ClosedSubset> find_topk_closedsubsets(int k);
    void expand_antichain(set<int> A, vector<int> RA, int pos, int k);
    

    void package_results(const string &results_file, double runtime, vector<ClosedSubset> kesm_results);
};

#endif