#ifndef ALG_ENUMSTARR_H_
#define ALG_ENUMSTARR_H_

#include "kesmp.h"
#include "bound_rotation.h"

//using namespace std;

class EnumStarR : public KESMP
{
public:

    EnumStarR(string instance_dir) : KESMP(instance_dir){};
    
    void init_method();


    //// Method 3: ENUM*-R
    vector<ClosedSubset> find_topk_closedsubsets(int k);
    void expand_antichain(set<int> A, vector<int> RA, int pos, int k);
    
    void package_results(const string &results_file, double runtime, vector<ClosedSubset> kesm_results);
};

#endif