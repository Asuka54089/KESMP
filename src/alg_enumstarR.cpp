
#include "../include/alg_enumstarR.h"

using namespace std;

void EnumStarR::init_method()
{
    poset.init_pred_succ();
    init_topk();

    kth_deltaS = INT_MIN;

    //is r visited ?(as a start rotation)
    visited = new bool[poset.node_size];
    for (int i = 0; i < poset.node_size; i++)
    {
        visited[i] = false;
    }
    init_rotation_bounds();
}

vector<ClosedSubset> EnumStarR::find_topk_closedsubsets(int k)
{
    init();
    init_method();

    set<int> null_A;

    // the first result: male optimal stable marriage (S is empty)
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

void EnumStarR::expand_antichain(set<int> A, vector<int> RA, int pos, int k)
{
    for (int i = poset.node_size - 1; i >= pos; i--)
    {
        if (sm_counts > MAX_ANTICHAIN_NUM)
        {
            return;
        }
        
        int r =  RA[i];
        if (r < 0)
        {
            continue;
        }

        if (pruned_rotations.find(r) != pruned_rotations.end())
        {
            continue;
        }

        //calculate rotation bound
        if (visited[r] == false)
        {
            visited[r] = true;
            calculate_rotation_bound(r, k);
        }

        A.insert(r);
        //update topk results
        S = poset.A_to_S(A);
        is_updated = update_kesm(S, k);

        if (is_updated)
        {
            update_pruned_rotations(k);
        }

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

void EnumStarR::package_results(const string &results_file, double runtime, vector<ClosedSubset> topk_closed_subsets)
{
    map<string, int> counters = {
        {"sm_counts", sm_counts},
        {"update_counts", update_counts},
        //{"recursion_counter", recursion_counter},
        {"R", poset.node_size},
        {"rb_first_pruning", pruned_rotation_counts},
        {"rb_final_pruning", pruned_rotations.size()},
    };
    map<string, string> lists = {
        {"rb_time", to_string(rb_time)},
    };
    string save_path = results_file + "_m3";
    save_results(save_path, "enumstarr", runtime, counters, lists, topk_closed_subsets);
}

