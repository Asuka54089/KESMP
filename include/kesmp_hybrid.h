#ifndef KESMP_HYBRID_H_
#define KESMP_HYBRID_H_

#include "kesmp.h"

class KESMP_Hybrid : public KESMP
{
public:
    KESMP_Hybrid(bool verbose_case, bool verbose_result) : KESMP(verbose_case, verbose_result){};
    void init_method();

    //3.maxflow bound
    vector<ClosedSubset> find_posets(int k);
    void expand_antichain(Antichain antichain, vector<int> candidates, int pos, int k);
    void package_results(const string &results_file, double runtime, vector<ClosedSubset> kesm_results);
};

#endif