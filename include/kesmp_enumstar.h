#ifndef KESMP_ENUMSTAR_H_
#define KESMP_ENUMSTAR_H_

#include "kesmp.h"


class KESMP_Enumstar : public KESMP
{
public:
    void init_method();

    KESMP_Enumstar() : KESMP(){};

    // Method 1: ENUM*
    vector<ClosedSubset> find_topk_S(int k);
    
    void expand_antichain(set<int> A, vector<int> RA, int pos, int k);
    
    //vector<ClosedSubset> find_topK_S_queue(int k);
    
    void package_results(const string &results_file,double runtime,vector<ClosedSubset> kesm_results);
};

#endif