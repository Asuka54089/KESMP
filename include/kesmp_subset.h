#ifndef KESMP_SUBSET_H_
#define KESMP_SUBSET_H_

#include "kesmp.h"
#include "topk_bounds.h"

//using namespace std;

class KESMP_Subset : virtual public KESMP
{
public:
    void init_method();
    KESMP_Subset(bool verbose_case, bool verbose_result) : KESMP(verbose_case, verbose_result){};

    //4.subset bound
    vector<ClosedSubset> find_posets(int k);
    void expand_antichain(Antichain antichain, vector<int> candidates, int pos, int k);
    
    void package_results(const string &results_file, double runtime, vector<ClosedSubset> kesm_results);
};

#endif