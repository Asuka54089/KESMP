
#include "../include/kesmp_approx.h"

using namespace std;

void KESMP_Approx::init_method()
{
    construct_trie();
    //maintain topk results
    init_topk();

    candidate_size = 0;
}

void KESMP_Approx::init_subset()
{
    kesm_weight = INT_MIN;
 
    visited = new bool[rotation_num];
    for (int i = 0; i < rotation_num; i++)
    {
        visited[i] = false;
    }
    init_subset_bound();
}

vector<ClosedSubset> KESMP_Approx::find_posets_hpr(int k)
{
    init_method();

    deque<int> cand;
    Antichain null_achain;
    for (int i = 0; i < rotation_num; i++)
    {
        if (rotation_weights[i] >= 0)
        {
            cand.push_back(i);
        }
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
            if (antichain_counter > 1000000)
            {
                break;
            }

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
        cout << "method: hpr" << endl;
        cout << "rotation " << rotation_num << endl;
        cout << "candidate rotation " << cand.size() << endl;
        print_results(results);
    }
    candidate_size = cand.size();
    return results;
}

vector<ClosedSubset> KESMP_Approx::find_posets_hprs(int k)
{
    init_method();
    init_subset();


    deque<int> cand;
    Antichain null_achain;
    for (int i = 0; i < rotation_num; i++)
    {
        if (rotation_weights[i] >= 0)
        {
            cand.push_back(i);
        }
        //cand.push_back(i);
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

            if (pruned_rotations.find(r) != pruned_rotations.end())
            {
                continue;
            }

            if (visited[r] == false)
            {
                visited[r] = true;
                update_invaild_bound(r, k);
            }

       
            Antichain new_achain;
            new_achain.insert(AQ_item.antichain.begin(), AQ_item.antichain.end());
            new_achain.insert(r);

            
            subset = antichain_to_closedsubset(new_achain);
            is_updated = update_kesm(subset, k);
            if (verbose_case)
            {
                cout << antichain_counter << endl;
                print_antichain(new_achain);
            }

            if (is_updated)
            {
                update_pruned_rotations(k);
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
        cout << "method: hprs" << endl;
        cout << "rotation " << rotation_num << endl;
        cout << "candidate rotation " << cand.size() << endl;
        cout << "first pruning rotation " << pruning_counter << endl;
        cout << "final pruning rotation " << pruned_rotations.size() << endl;
        print_results(results);
    }

    candidate_size = cand.size();
    return results;
}

void KESMP_Approx::package_results(const string &results_file, double runtime, vector<ClosedSubset> kesm_results, int method)
{
    if (method == 5)
    {
        map<string, int> counters = {{"antichain_counter", antichain_counter}, {"update_counter", update_counter}, {"rotation", rotation_num}, {"candidate_rotation", candidate_size}};
        map<string, string> lists = {};
        string save_path = results_file + "_m5";
        save_results(save_path, "hpr", runtime, counters, lists, kesm_results);
    }
    if (method == 6)
    {
        map<string, int> counters = {{"antichain_counter", antichain_counter}, {"update_counter", update_counter}, {"rotation", rotation_num}, {"candidate_rotation", candidate_size}, {"first_pruning", pruning_counter}, {"final_pruning", pruned_rotations.size()}};
        map<string, string> lists = {};
        string save_path = results_file + "_m6";
        save_results(save_path, "hprs", runtime, counters, lists, kesm_results);
    }
}