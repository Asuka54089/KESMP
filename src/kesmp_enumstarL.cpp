
#include "../include/kesmp_enumstarL.h"

using namespace std;

void KESMP_Layer::init_method()
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

    init_layer_bounds();

}

vector<ClosedSubset> KESMP_Layer::find_topk_S(int k)
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

void KESMP_Layer::expand_antichain(set<int> A, vector<int> RA, int pos, int k)
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
        //may calculate layer bound for a unvisited r
        if (visited[r] == false)
        {
            visited[r] = true;
            L_current = smo.eliminated_rotations[r].depth;

            if (lb_flag_stop == false)
            {
                if (L_current == L_maxflow)
                {
                    update_layer_bound(k);
                }
            }
        }

         A.insert(r);

        // test cross flag
        lb_flag_cross = true;
        if (lb_flag_stop)
        {
            lb_flag_cross = test_flag_A_iscross(A);
        }

        if (lb_flag_cross)
        {
            //update topk results
            S = smg.antichain_to_closedsubset(A);
            update_kesm(S, k);
        }

        vector<int> tmp_RA;
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

        if (lb_flag_stop)
        {
            if (lb_flag_cross == false)
            {
                lb_flag_cross = test_cross_candidate(RA, i + 1, RA.size() - 1);
            }
        }

        if (vaild_counter > 0 && lb_flag_cross == true)
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

void KESMP_Layer::package_results(const string &results_file, double runtime, vector<ClosedSubset> kesm_results)
{
    string save_path = results_file + "_m2";
    map<string, int> counters = {};
    map<string, string> lists = {};
    if (!lb_flag_rotation || !lb_flag_mincutset)
    {
        counters = {
            {"lb_flag_rotation", lb_flag_rotation},
            {"lb_flag_mincutset", lb_flag_mincutset},
        };
        lists = {};
        save_results(save_path, "enum*layer", runtime, counters, lists, kesm_results);
    }
    else
    {
        counters = {
            {"antichain_counter", antichain_counter},
            //{"update_counter", update_counter},
            //{"recursion_counter", recursion_counter},
            {"L_max", L_max},
            {"L_stop", L_stop},
            {"layer_bound", layer_bound},
            {"diff_kS", diff_kS},
            //{"achain_test_counter", achain_test_counter},
        };

        string mincut_layers_str = convert_list(mincut_layers);
        string mincut_bounds_str = convert_list(mincut_bounds);
        string mincut_diff_kS_str = convert_list(mincut_diff_kS);
        lists = {
            {"layer_bound_time", to_string(layer_bound_time)},
            {"mincut_layers", mincut_layers_str},
            {"mincut_bounds", mincut_bounds_str},
            {"mincut_diff_kS", mincut_diff_kS_str},
        };

        save_results(save_path, "enum*l", runtime, counters, lists, kesm_results);
    }
}