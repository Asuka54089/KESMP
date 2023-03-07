#ifndef KESMP_SUBSET_H_
#define KESMP_SUBSET_H_

#include "kesmp.h"
#include "bound_rotation.h"

//using namespace std;

class KESMP_Rotation : virtual public KESMP
{
public:
    void init_method();
    KESMP_Rotation() : KESMP(){};

    //// Method 3: ENUM*-R
    vector<ClosedSubset> find_topk_S(int k);
    void expand_antichain(set<int> A, vector<int> RA, int pos, int k);
    
    void package_results(const string &results_file, double runtime, vector<ClosedSubset> kesm_results);
};

#endif