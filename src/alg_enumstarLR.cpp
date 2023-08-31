
#include "../include/alg_enumstarLR.h"

using namespace std;

void EnumStarLR::init_method()
{
    poset.init_pred_succ();
    init_topk();

    kth_deltaS = INT_MIN;

    visited = new bool[poset.node_size];
    for (int i = 0; i < poset.node_size; i++)
    {
        visited[i] = false;
    }
    init_layer_bounds();
    init_rotation_bounds();
}

vector<ClosedSubset> EnumStarLR::find_topk_closedsubsets(int k)
{
    init();
    init_method();
    if ((!lb_flag_rotation) || (!lb_flag_mincutset))
    {
        // layer bound doesn't work
        vector<ClosedSubset> null;
        return null;
    }

    set<int> null_A;

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

void EnumStarLR::expand_antichain(set<int> A, vector<int> RA, int pos, int k)
{
    if (sm_counts > MAX_ANTICHAIN_NUM)
    {
        return;
    }

    for (int i = poset.node_size - 1; i >= pos; i--)
    {
        int r = RA[i];
        if (r < 0)
        {
            continue;
        }

        if (pruned_rotations.find(r) != pruned_rotations.end())
        {
            continue;
        }

        //calculate maxflow bound for a unvisited r
        if (visited[r] == false)
        {
            //cout<<"visited[r]:"<<r<<endl;
            visited[r] = true;

            //layer bound
            L_current = poset.eliminated_rotations[r].depth;
            if (lb_flag_stop == false)
            {
                if (L_current == L_maxflow)
                {
                    //cout << "update layer bound: "<< L_current<<endl;
                    update_layer_bound(k);
                }
            }

            //rotation bound
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
            S = poset.A_to_S(A);
            is_updated = update_kesm(S, k);
            if (is_updated)
            {
                update_pruned_rotations(k);
            }
        }

        vector<int> tmp_RA;
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

        if (lb_flag_stop)
        {
            if (lb_flag_cross == false)
            {
                lb_flag_cross = test_cross_candidate(RA, i + 1, RA.size() - 1);
            }
        }

        if (vaild_counts > 0 && lb_flag_cross == true)
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

void EnumStarLR::package_results(const string &results_file, double runtime, vector<ClosedSubset> topk_closed_subsets)
{
    string save_path = results_file + "_m4";
    map<string, int> counters = {};
    map<string, string> lists = {};
    if (!lb_flag_rotation | !lb_flag_mincutset)
    {
        counters = {
            {"lb_flag_rotation", lb_flag_rotation},
            {"lb_flag_mincutset", lb_flag_mincutset},
        };
        lists = {};
        save_results(save_path, "enumstarlr", runtime, counters, lists, topk_closed_subsets);
    }
    else
    {
        counters = {
            {"sm_counts", sm_counts},
            {"update_counts", update_counts},
            // {"recursion_counter", recursion_counter},
            {"R", poset.node_size},
            {"rb_first_pruning", pruned_rotation_counts},
            {"rb_final_pruning", pruned_rotations.size()},
            {"lb_layer_max", L_max},
            {"lb_layer_stop", L_stop},
            {"lb", layer_bound},
            {"kth_deltaS", kth_deltaS},
            //{"achain_test_counter", achain_test_counter}
        };
        string lb_layers_str = convert_list(lb_layers);
        string lb_bounds_str = convert_list(lb_bounds);
        string lb_k_deltaS_str = convert_list(lb_k_deltaS);
        lists = {
            {"rb_time", to_string(rb_time)},
            {"lb_time", to_string(lb_time)},
            {"lb_layers", lb_layers_str},
            {"lb_bounds", lb_bounds_str},
            {"lb_kth_deltaS", lb_k_deltaS_str},
        };

        save_results(save_path, "enumstarlr", runtime, counters, lists, topk_closed_subsets);
    }
}