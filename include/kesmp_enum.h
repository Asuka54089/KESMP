#ifndef KESMP_ENUM_H_
#define KESMP_ENUM_H_

#include "kesmp.h"


class KESMP_Enum : public KESMP
{
public:
    void init_method();

    KESMP_Enum() : KESMP(){};

    // Method 0: ENUM
    vector<ClosedSubset> find_topk_S(int k);
    
    void init_Rexposed(vector<int> &D, deque<int> &Rexposed);

    void enumerate_stable_marriages(vector<int> &D,deque<int> &Rexposed, set<int> &S, int k);
    
    
    void package_results(const string &results_file,double runtime,vector<ClosedSubset> kesm_results);
};

#endif