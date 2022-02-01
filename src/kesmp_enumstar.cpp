
#include "../include/kesmp_enumstar.h"

using namespace std;

void KESMP_Enumstar::init_method()
{
    construct_trie();
    init_topk();
    recursion_counter = 0;
}


vector<ClosedSubset> KESMP_Enumstar::find_posets(int k)
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
        cout << "method: enumstar" << endl;
        print_results(kesm_results);
    }
    return kesm_results;
}

void KESMP_Enumstar::expand_antichain(Antichain antichain, vector<int> candidates, int pos, int k)
{
    // if (antichain_counter > 1000000)
    // {
    //     // cout << "count3:" << antichain_counter << endl;
    //     return;
    // }
    for (int i = rotation_num - 1; i >= pos; i--)
    {
        // cout << "count2:" << antichain_counter << endl;
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
        antichain.insert(r);

      
        subset = antichain_to_closedsubset(antichain);
        update_kesm(subset, k);
        if (verbose_case)
        {
            cout << antichain_counter << endl;
            print_antichain(antichain);
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


vector<ClosedSubset> KESMP_Enumstar::find_posets_queue(int k)
{
    init_method();

 
    deque<int> cand;
    Antichain null_achain;
    for (int i = 0; i < rotation_num; i++)
    {
        cand.push_back(i);
    }

    AntichainQueueItem AQ_item0;
    queue<AntichainQueueItem> AQ;
    AQ_item0.antichain = null_achain;
    AQ_item0.candidates = cand;
    AQ.push(AQ_item0);

    update_kesm(null_achain, k);
    if (verbose_case)
    {
        cout << antichain_counter << endl;
        print_antichain(null_achain);
    }

    while (!AQ.empty())
    {
        AntichainQueueItem AQ_item = AQ.front();
        AQ.pop();

        if (verbose_case)
        {
            cout << "[Pop AQ]:" << endl;
            print_AQ_item(AQ_item0);
        }
        //print_antichain(tmp_ac.antichain);
        while (!AQ_item.candidates.empty())
        {
            int r = AQ_item.candidates.front();
            AQ_item.candidates.pop_front();

        
            Antichain new_achain;
            new_achain.insert(AQ_item.antichain.begin(), AQ_item.antichain.end());
            new_achain.insert(r);

          
            Subset subset = antichain_to_closedsubset(new_achain);
            update_kesm(subset, k);
            if (verbose_case)
            {
                cout << antichain_counter << endl;
                print_antichain(new_achain);
            }

          
            deque<int> new_cand;
            for (auto n : AQ_item.candidates)
            {
                // if (pred[r].find(n) == pred[r].end() && succ[r].find(n) == succ[r].end())
                if (succ[r].find(n) == succ[r].end())
                {
                    new_cand.push_back(n);
                }
            }

            if (new_cand.size() > 0)
            {
                AntichainQueueItem new_AQ_item;
                new_AQ_item.antichain = new_achain;
                new_AQ_item.candidates = new_cand;

                AQ.push(new_AQ_item);
            }
        }
    }

    vector<ClosedSubset> results = kesm_heap.generate();

    if (verbose_result)
    {
        cout << "method: enum" << endl;
        print_results(results);
    }
    return results;
}

void KESMP_Enumstar::package_results(const string &results_file, double runtime, vector<ClosedSubset> kesm_results)
{
    map<string, int> counters = {
        {"antichain_counter", antichain_counter},
        {"update_counter", update_counter},
        {"recursion_counter", recursion_counter},
    };
    map<string, string> lists = {};
    string save_path = results_file + "_m1";
    save_results(save_path, "enum*", runtime, counters, lists, kesm_results);

    // if (method == 1)
    // {
    //     map<string, int> Counters = {
    //         {"anchain_counter", antichain_counter},
    //         {"update_counter", update_counter},
    //         {"recursion_counter", recursion_counter},
    //     };
    //     map<string, string> Lists = {};
    //     string save_path = filepath + "_m1r";
    //     save_results(save_path, "enum-recursion", time, Counters, Lists, results);
    // }
    // else
    // {
    //     map<string, int> Counters = {
    //         {"anchain_counter", antichain_counter},
    //         {"update_counter", update_counter},
    //         {"recursion_counter", recursion_counter},
    //     };
    //     map<string, string> Lists = {};
    //     string save_path = filepath + "_m1q";
    //     save_results(save_path, "enum-queue", time, Counters, Lists, results);
    // }
}
