#ifndef KESMP_ENUMSTAR_H_
#define KESMP_ENUMSTAR_H_

#include "kesmp.h"


class KESMP_Enumstar : public KESMP
{
public:
    void init_method();

    KESMP_Enumstar(bool verbose_case, bool verbose_result) : KESMP(verbose_case, verbose_result){};

    vector<ClosedSubset> find_posets(int k);
    void expand_antichain(Antichain antichain, vector<int> candidates, int pos, int k);
    
    vector<ClosedSubset> find_posets_queue(int k);
    
    void package_results(const string &results_file,double runtime,vector<ClosedSubset> kesm_results);
};

#endif