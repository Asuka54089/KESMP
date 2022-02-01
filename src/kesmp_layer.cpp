
#include "../include/kesmp_layer.h"

using namespace std;

void KESMP_Layer::init_method()
{

    construct_trie();
    //maintain topk results
    init_topk();

    kesm_weight = INT_MIN;

    visited = new bool[rotation_num];
    for (int i = 0; i < rotation_num; i++)
    {
        visited[i] = false;
    }

    init_layer_bound();

    recursion_counter = 0;
}

vector<ClosedSubset> KESMP_Layer::find_posets(int k)
{
    init_method();
    if ((!layer_bound_flag1) || (!layer_bound_flag2))
    {
        vector<ClosedSubset> null;
        return null;
    }

    Antichain null_antichain;

    update_kesm(null_antichain, k);
    if (verbose_case)
    {
        cout << antichain_counter << endl;
        print_antichain(null_antichain);
    }

    vector<int> candidates;
    for (int i = 0; i < rotation_num; i++)
    {
        candidates.push_back(i);
    }

    expand_antichain(null_antichain, candidates, 0, k);

    vector<ClosedSubset> kesm_results = kesm_heap.generate();

    if (verbose_result)
    {
        cout << "method: layer" << endl;
        cout << "maxflow_time" << maxflow_time << endl;
        for (int l : maxflow_layers)
        {
            cout << l << " ";
        }
        cout << endl;
        cout << "L_max " << L_max << endl;
        cout << "L_stop " << L_stop << endl;
        cout << "L_current " << L_current << endl;
        cout << "layer_bound:" << layer_bound << endl;
        cout << "kesm_weight:" << kesm_weight << endl;
        //cout << "achain_test_counter:" << achain_test_counter << endl;

        print_results(kesm_results);
    }
    return kesm_results;
}

void KESMP_Layer::expand_antichain(Antichain antichain, vector<int> candidates, int pos, int k)
{
    // if (antichain_counter > 1000000)
    // {
    //     return;
    // }
    for (int i = rotation_num - 1; i >= pos; i--)
    {
        // if (antichain_counter > 1000000)
        // {
        //     return;
        // }
        recursion_counter++;
        int r = candidates[i];
        if (r < 0)
        {
            continue;
        }
    
        if (visited[r] == false)
        {
            visited[r] = true;
            L_current = rotation_depths[r];
            // cout << "current rotation " << r << endl;
            // cout << "current layer " << L_current << endl;
            if (stop_flag == false)
            {
                if (L_current == L_maxflow)
                {
                    update_maxflow_bound(k);
                }
            }
        }

        antichain.insert(r);

        cross_flag = true;
        if (stop_flag)
        {
            cross_flag = test_cross_antichain(antichain);
        }


        if (cross_flag)
        {
            subset = antichain_to_closedsubset(antichain);
            update_kesm(subset, k);
            if (verbose_case)
            {
                cout << antichain_counter << endl;
                print_antichain(antichain);
            }
        }

        vector<int> tmp_candidates;
        vaild_counter = 0;
        for (int j = i + 1; j < candidates.size(); j++)
        {
            if (candidates[j] > 0)
            {
                if (succ[r].find(candidates[j]) != succ[r].end())
                {
                    candidates[j] = -candidates[j];
                    tmp_candidates.push_back(j);
                }
            }
            if (candidates[j] > 0)
            {
                vaild_counter = vaild_counter + 1;
            }
        }

        if (stop_flag)
        {
            if (cross_flag == false)
            {
                cross_flag = test_cross_candidate(candidates, i + 1, candidates.size() - 1);
            }
        }

        if (vaild_counter > 0 && cross_flag == true)
        {
            expand_antichain(antichain, candidates, i + 1, k);
        }

        for (int n : tmp_candidates)
        {
            candidates[n] = abs(candidates[n]);
        }
        antichain.erase(r);
    }
}

void KESMP_Layer::package_results(const string &results_file, double runtime, vector<ClosedSubset> kesm_results)
{
    string save_path = results_file + "_m2";
    map<string, int> counters = {};
    map<string, string> lists = {};
    if (!layer_bound_flag1 || !layer_bound_flag2)
    {
        counters = {
            {"layer_bound_flag1", layer_bound_flag1},
            {"layer_bound_flag2", layer_bound_flag2},
        };
        lists = {};
        save_results(save_path, "layer", runtime, counters, lists, kesm_results);
    }
    else
    {
        counters = {
            {"antichain_counter", antichain_counter},
            {"update_counter", update_counter},
            {"recursion_counter", recursion_counter},
            {"L_max", L_max},
            {"L_stop", L_stop},
            {"layer_bound", layer_bound},
            {"kesm_weight", kesm_weight},
            //{"achain_test_counter", achain_test_counter},
        };

        string maxflow_layers_str = convert_list(maxflow_layers);
        string maxflow_bounds_str = convert_list(maxflow_bounds);
        string rule2_kesm_weight_str = convert_list(rule2_kesm_weight);
        lists = {
            {"maxflow_time", to_string(maxflow_time)},
            {"maxflow_layers", maxflow_layers_str},
            {"maxflow_bounds", maxflow_bounds_str},
            {"rule2_kesm_weight", rule2_kesm_weight_str},
        };

        save_results(save_path, "layer", runtime, counters, lists, kesm_results);
    }
}