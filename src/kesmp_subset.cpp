
#include "../include/kesmp_subset.h"

using namespace std;

void KESMP_Subset::init_method()
{
    //maintain topk results
    construct_trie();
    init_topk();

    kesm_weight = INT_MIN;

  
    visited = new bool[rotation_num];
    for (int i = 0; i < rotation_num; i++)
    {
        visited[i] = false;
    }
    init_subset_bound();
    recursion_counter = 0;
}

vector<ClosedSubset> KESMP_Subset::find_posets(int k)
{
    init_method();

    //deque<int> cand;
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
        cout << "method: subset" << endl;
        cout << "rotation " << rotation_num << endl;
        cout << "first pruning rotation " << pruning_counter << endl;
        cout << "final pruning rotation " << pruned_rotations.size() << endl;

        print_results(kesm_results);
    }
    return kesm_results;
}

void KESMP_Subset::expand_antichain(Antichain antichain, vector<int> candidates, int pos, int k)
{
    for (int i = rotation_num - 1; i >= pos; i--)
    {
        recursion_counter++;
        int r = candidates[i];
        if (r < 0)
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
            update_invaild_bound(r, k);
        }

        antichain.insert(r);
   
        subset = antichain_to_closedsubset(antichain);
        is_updated = update_kesm(subset, k);
        if (verbose_case)
        {
            cout << antichain_counter << endl;
            print_antichain(antichain);
        }

        if (is_updated)
        {
            update_pruned_rotations(k);
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

        if (vaild_counter > 0)
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

void KESMP_Subset::package_results(const string &results_file, double runtime, vector<ClosedSubset> kesm_results)
{
    map<string, int> counters = {
        {"antichain_counter", antichain_counter},
        {"update_counter", update_counter},
        {"recursion_counter", recursion_counter},
        {"rotation", rotation_num},
        {"first_pruning", pruning_counter},
        {"final_pruning", pruned_rotations.size()},
    };
    map<string, string> lists = {
        {"subset_time", to_string(subset_time)},
    };
    string save_path = results_file + "_m3";
    save_results(save_path, "subset", runtime, counters, lists, kesm_results);
}

