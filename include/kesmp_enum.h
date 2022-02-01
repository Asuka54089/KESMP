#ifndef KESMP_ENUM_H_
#define KESMP_ENUM_H_

#include "kesmp.h"

class KESMP_Enum : public KESMP
{
public:
    //counter
    int enum_counter;

    bool overflow;


    vector<set<int>> parents;
    
    void init_method();

    KESMP_Enum(bool verbose_case, bool verbose_result) : KESMP(verbose_case, verbose_result){};
    vector<ClosedSubset> find_posets(int k);

    //enumerate all + reverse_bfs
    bool isClosed(Subset subset);
    bool reverse_bfs(int v, Subset subset, bool isclosed[]);

    void package_results(const string &results_file, double runtime, vector<ClosedSubset> kesm_results);
};

#endif
