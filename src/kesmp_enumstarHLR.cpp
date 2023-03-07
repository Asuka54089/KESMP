
#include "../include/kesmp_enumstarHLR.h"

using namespace std;

void KESMP_HLR::init_method()
{
    candidate_size = 0;

    smg.init_pred_and_succ();
    init_topk();

    diff_kS = INT_MIN;


    visited = new bool[smg.rotation_num];
    for (int i = 0; i < smg.rotation_num; i++)
    {
        visited[i] = false;
    }
    init_layer_bounds();
    init_rotation_bounds();

}

vector<ClosedSubset> KESMP_HLR::find_topk_S(int k)
{
    init_method();
    if ((!lb_flag_rotation) || (!lb_flag_mincutset))
    {
        vector<ClosedSubset> null;
        return null;
    }

    set<int> null_A;

    // candidate rotations for expanding antichain A
    vector<int> RA;

    for (int i = 0; i < smg.rotation_num; i++)
    {
        RA.push_back(i);
        if (smo.eliminated_rotations[i].weight >= 0)
        {  
            candidate_size++;
        }
    }

    expand_antichain(null_A, RA, 0, k);

    vector<ClosedSubset> best_kSs = TKSM_object.generate();

    
    return best_kSs;
}

void KESMP_HLR::expand_antichain(set<int> A, vector<int> RA, int pos, int k)
{
    for (int i = smg.rotation_num - 1; i >= pos; i--)
    {
        int r = RA[i];
        if (r < 0 || smo.eliminated_rotations[r].weight<0)
        {
            continue;
        }

        if (pruned_rotations.find(r) != pruned_rotations.end())
        {
            continue;
        }

        
        if (visited[r] == false)
        {
            visited[r] = true;

            //layer bound
            L_current = smo.eliminated_rotations[r].depth;
            if (lb_flag_stop == false)
            {
                if (L_current == L_maxflow)
                {
                    update_layer_bound(k);
                }
            }

            //subset bound
            calculate_rotation_bound(r, k);
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
            is_updated = update_kesm(S, k);
            if (is_updated)
            {
                update_pruned_rotations(k);
            }

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

void KESMP_HLR::package_results(const string &results_file, double runtime, vector<ClosedSubset> kesm_results)
{
    string save_path = results_file + "_m6";
    map<string, int> counters = {};
    map<string, string> lists = {};
    if (!lb_flag_rotation | !lb_flag_mincutset)
    {
        counters = {
            {"lb_flag_rotation", lb_flag_rotation},
            {"lb_flag_mincutset", lb_flag_mincutset},
        };
        lists = {};
        save_results(save_path, "hlr", runtime, counters, lists, kesm_results);
    }
    else
    {
        counters = {
            {"antichain_counter", antichain_counter},
            //{"update_counter", update_counter},
            // {"recursion_counter", recursion_counter},
            {"rotation", smg.rotation_num},
            {"first_pruning", pruned_rotation_counter},
            {"final_pruning", pruned_rotations.size()},
            {"L_max", L_max},
            {"L_stop", L_stop},
            {"layer_bound", layer_bound},
            {"diff_kS", diff_kS},
            {"candidate_size", candidate_size}
            };
        string mincut_layers_str = convert_list(mincut_layers);
        string mincut_bounds_str = convert_list(mincut_bounds);
        string mincut_diff_kS_str = convert_list(mincut_diff_kS);
        lists = {
            {"rotation_time", to_string(rotation_bound_time)},
            {"layer_bound_time", to_string(layer_bound_time)},
            {"mincut_layers", mincut_layers_str},
            {"mincut_bounds", mincut_bounds_str},
            {"mincut_diff_kS", mincut_diff_kS_str},
        };

        save_results(save_path, "enum*hlr", runtime, counters, lists, kesm_results);
    }
}