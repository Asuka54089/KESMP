
#include "../include/kesmp_enumstar.h"

using namespace std;

void KESMP_Enumstar::init_method()
{
    smg.init_pred_and_succ();
    init_topk();
}

// ENUM*
vector<ClosedSubset> KESMP_Enumstar::find_topk_S(int k)
{
    init_method();

    set<int> null_A;

    // the first result: the male optimal stable marriage (S is empty)
    update_kesm(null_A, k);
    
    // candidate rotations for expanding antichain A
    vector<int> RA;

    for (int i = 0; i < smg.rotation_num; i++)
    {
        RA.push_back(i);
    }

    expand_antichain(null_A, RA, 0, k);

    vector<ClosedSubset> best_kSs = TKSM_object.generate();

    return best_kSs;
}

void KESMP_Enumstar::expand_antichain(set<int> A, vector<int> RA, int pos, int k)
{

    for (int i = smg.rotation_num - 1; i >= pos; i--)
    {
        if (antichain_counter > MAX_ANTICHAIN_NUM)
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
        S = smg.antichain_to_closedsubset(A);
        update_kesm(S, k);

        vector<int> tmp_RA; //local variable (can't be the class memeber)
        vaild_counter = 0;
        for (int j = i + 1; j < RA.size(); j++)
        {
            if (RA[j] > 0)
            {
                if (smg.succ[r].find(RA[j]) != smg.succ[r].end())
                {
                    RA[j] = -RA[j];
                    tmp_RA.push_back(j);
                }
            }
            if (RA[j] > 0)
            {
                vaild_counter = vaild_counter + 1;
            }
        }

        if (vaild_counter > 0)
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



void KESMP_Enumstar::package_results(const string &results_file, double runtime, vector<ClosedSubset> kesm_results)
{
    map<string, int> counters = {
        {"antichain_counter", antichain_counter},
        //{"update_counter", update_counter},
    };
    map<string, string> lists = {};
    string save_path = results_file + "_m1";
    save_results(save_path, "enum*", runtime, counters, lists, kesm_results);

}
