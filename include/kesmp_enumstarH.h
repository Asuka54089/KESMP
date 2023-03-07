#ifndef KESMP_ENUMSTARH_H_
#define KESMP_ENUMSTARH_H_

#include "kesmp.h"

class KESMP_Heuristic : public KESMP
{
public:
    int candidate_size; //# (w+ rotations + w0 rotations)
    
    void init_method();

    KESMP_Heuristic() : KESMP(){};

    // Method 5: ENUM*-H
    vector<ClosedSubset> find_topk_S(int k);
    void expand_antichain(set<int> A, vector<int> RA, int pos, int k);
    

    void package_results(const string &results_file, double runtime, vector<ClosedSubset> kesm_results);
};

#endif