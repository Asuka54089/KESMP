
#include "../include/alg_enumstarL.h"

using namespace std;

void EnumStarL::init_method()
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

    // mincut time
    clock_t start_time2, end_time2;
    start_time2 = clock();
    init_layer_bounds();
    end_time2 = clock();
    mincut_time = (double)(end_time - start_time) / CLOCKS_PER_SEC;

}

vector<ClosedSubset> EnumStarL::find_topk_closedsubsets(int k)
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

void EnumStarL::expand_antichain(set<int> A, vector<int> RA, int pos, int k)
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
        //may calculate layer bound for a unvisited r
        if (visited[r] == false)
        {
            visited[r] = true;
            L_current = poset.eliminated_rotations[r].depth;

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
            S = poset.A_to_S(A);
            update_kesm(S, k);
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

void EnumStarL::package_results(const string &results_file, double runtime, vector<ClosedSubset> topk_closed_subsets)
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
        save_results(save_path, "enumstarl", runtime, counters, lists, topk_closed_subsets);
    }
    else
    {
        counters = {
            {"sm_counts", sm_counts},
            {"update_counts", update_counts},
            //{"recursion_counter", recursion_counter},
            {"lb_layer_max", L_max},
            {"lb_layer_stop", L_stop},
            {"lb", layer_bound},
            {"kth_deltaS", kth_deltaS},
            //{"achain_test_counter", achain_test_counter},
        };

        string lb_layers_str = convert_list(lb_layers);
        string lb_bounds_str = convert_list(lb_bounds);
        string lb_k_deltaS_str = convert_list(lb_k_deltaS);
        lists = {
            {"lb_time", to_string(lb_time)},
            {"mincut_time", to_string(mincut_time)},
            {"lb_layers", lb_layers_str},
            {"lb_bounds", lb_bounds_str},
            {"lb_kth_deltaS", lb_k_deltaS_str},
        };

        save_results(save_path, "enumstarl", runtime, counters, lists, topk_closed_subsets);
    }
}