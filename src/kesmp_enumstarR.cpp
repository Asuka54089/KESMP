
#include "../include/kesmp_enumstarR.h"

using namespace std;

void KESMP_Rotation::init_method()
{

    smg.init_pred_and_succ();
    init_topk();

    diff_kS = INT_MIN;

    //is r visited ?(as a start rotation)
    visited = new bool[smg.rotation_num];
    for (int i = 0; i < smg.rotation_num; i++)
    {
        visited[i] = false;
    }
    init_rotation_bounds();
}

vector<ClosedSubset> KESMP_Rotation::find_topk_S(int k)
{
    init_method();

    set<int> null_A;

    // the first result: male optimal stable marriage (S is empty)
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

void KESMP_Rotation::expand_antichain(set<int> A, vector<int> RA, int pos, int k)
{
    for (int i = smg.rotation_num - 1; i >= pos; i--)
    {
        if (antichain_counter > MAX_ANTICHAIN_NUM)
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
        S = smg.antichain_to_closedsubset(A);
        is_updated = update_kesm(S, k);

        if (is_updated)
        {
            update_pruned_rotations(k);
        }

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

void KESMP_Rotation::package_results(const string &results_file, double runtime, vector<ClosedSubset> kesm_results)
{
    map<string, int> counters = {
        {"antichain_counter", antichain_counter},
        //{"update_counter", update_counter},
        //{"recursion_counter", recursion_counter},
        {"#rotation", smg.rotation_num},
        {"first_pruning", pruned_rotation_counter},
        {"final_pruning", pruned_rotations.size()},
    };
    map<string, string> lists = {
        {"rotation_time", to_string(rotation_bound_time)},
    };
    string save_path = results_file + "_m3";
    save_results(save_path, "enum*r", runtime, counters, lists, kesm_results);
}

