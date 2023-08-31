
#include "../include/alg_enumstar.h"

using namespace std;

void EnumStar::init_method()
{
    poset.init_pred_succ();
    init_topk();
}

// ENUM*
vector<ClosedSubset> EnumStar::find_topk_closedsubsets(int k)
{
    init();
    
    init_method();

    set<int> null_A;

    // the first result: the male optimal stable marriage (S is empty)
    update_kesm(null_A, k);
    
    // candidate rotations for expanding antichain A
    vector<int> RA;

    for (int i = 0; i < poset.node_size; i++)
    {
        RA.push_back(i);
    }

    expand_antichain(null_A, RA, 0, k);

    vector<ClosedSubset> topk_closed_subsets = topk_object.generate();

    return topk_closed_subsets;
}

void EnumStar::expand_antichain(set<int> A, vector<int> RA, int pos, int k)
{

    for (int i = poset.node_size - 1; i >= pos; i--)
    {
        if (sm_counts > MAX_ANTICHAIN_NUM)
        {
            return;
        }
      
        int r = RA[i];
        if (r < 0)
        {
            continue;
        }
        A.insert(r);

        //update topk results
        S = poset.A_to_S(A);
        update_kesm(S, k);

        vector<int> tmp_RA; //local variable (can't be the class memeber)
        vaild_counts = 0;
        for (int j = i + 1; j < RA.size(); j++)
        {
            if (RA[j] > 0)
            {
                if (poset.succ[r].find(RA[j]) != poset.succ[r].end())
                {
                    RA[j] = -RA[j];
                    tmp_RA.push_back(j);
                }
            }
            if (RA[j] > 0)
            {
                vaild_counts = vaild_counts + 1;
            }
        }

        if (vaild_counts > 0)
        {
            expand_antichain(A, RA, i + 1, k);
        }

        for (int n : tmp_RA)
        {
            RA[n] = abs(RA[n]);
        }
        A.erase(r);
    }
}



void EnumStar::package_results(const string &results_file, double runtime, vector<ClosedSubset> topk_closed_subsets)
{
    map<string, int> counters = {
        {"sm_counts", sm_counts},
        {"update_counts", update_counts},
    };
    map<string, string> lists = {};
    string save_path = results_file + "_m1";
    save_results(save_path, "enumstar", runtime, counters, lists, topk_closed_subsets);

}
