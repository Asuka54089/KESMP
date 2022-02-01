#ifndef KESMP_LAYER_H_
#define KESMP_LAYER_H_

#include "kesmp.h"
#include "maxflow.h"

//input:
//rotation graph(rotations,rotation edges)
//output:
//topk closed subset
class KESMP_Layer : virtual public KESMP
{
public:
    KESMP_Layer(bool verbose_case, bool verbose_result) : KESMP(verbose_case, verbose_result){};
    void init_method();

    //3.maxflow bound
    vector<ClosedSubset> find_posets(int k);
    void expand_antichain(Antichain antichain, vector<int> candidates, int pos, int k);

    void package_results(const string &results_file, double runtime, vector<ClosedSubset> kesm_results);
};

#endif