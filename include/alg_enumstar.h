#ifndef ALG_ENUMSTAR_H_
#define ALG_ENUMSTAR_H_

#include "kesmp.h"

class EnumStar : public KESMP
{
public:
    EnumStar(string instance_dir) : KESMP(instance_dir){};

    void init_method();

    // Method 1: ENUM*
    vector<ClosedSubset> find_topk_closedsubsets(int k);

    void expand_antichain(set<int> A, vector<int> RA, int pos, int k);

    //vector<ClosedSubset> find_topK_S_queue(int k);

    void package_results(const string &results_file, double runtime, vector<ClosedSubset> kesm_results);
};

#endif