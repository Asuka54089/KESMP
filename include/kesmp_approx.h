#ifndef KESMP_APPROX_H_
#define KESMP_APPROX_H_

#include "kesmp.h"

class KESMP_Approx : public KESMP
{
public:
    int candidate_size; 
    
    void init_method();
    void init_subset();

    KESMP_Approx(bool verbose_case, bool verbose_result) : KESMP(verbose_case, verbose_result){};

    //5.Rotation+
    vector<ClosedSubset> find_posets_hpr(int k);
    vector<ClosedSubset> find_posets_hprs(int k);

    void package_results(const string &results_file, double runtime, vector<ClosedSubset> kesm_results, int method);
};

#endif
