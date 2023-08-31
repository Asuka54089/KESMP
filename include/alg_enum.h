#ifndef ALG_ENUM_H_
#define ALG_ENUM_H_

#include "kesmp.h"

class Enum : public KESMP
{
public:
    Enum(string instance_dir) : KESMP(instance_dir){};

    void init_method();

    // Method 0: ENUM
    vector<ClosedSubset> find_topk_closedsubsets(int k);

    void init_Rexposed(vector<int> &D, deque<int> &Rexposed);

    void enumerate_stable_marriages(vector<int> &D, deque<int> &Rexposed, set<int> &S, int k);

    void package_results(const string &results_file, double runtime, vector<ClosedSubset> kesm_results);
};

#endif