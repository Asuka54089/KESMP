#ifndef KESMP_H_
#define KESMP_H_

#include <iostream>
#include <fstream>
#include <string.h>
#include <vector>
#include <map>
#include <set>
#include <queue>
#include <deque>
#include <math.h>
#include <numeric>
#include <time.h>

#include "topk_sm.h"
#include "maxflow.h"
#include "topk_bounds.h"

//using namespace std;


typedef set<int> Antichain;

struct AntichainQueueItem
{
    Antichain antichain;
    deque<int> candidates;
};

class KESMP
{
public:
    
    vector<set<int>> children;
    
    int rotation_num;
    vector<int> rotation_weights;
    vector<int> rotation_depths;

    KESMP(bool verbose_case, bool verbose_result);

    void init_rotation_graph(set<pair<int, int>> rotation_edges, int rotation_num, vector<int> rotation_weights, vector<int> rotation_depths);

    /* TopK */
    //maintain topk results
    TopK_StableMarriages kesm_heap;
    vector<ClosedSubset> tmp_subsets;

    bool update_kesm(Subset subset, int k);
 
    virtual vector<ClosedSubset> find_posets(int k);

    int antichain_counter; 
    int update_counter; 

  
    void init_method();

    void init_topk();

    void init_layer_bound();
    void init_subset_bound();

    /* Enum */

    vector<set<int>> pred;
    vector<set<int>> succ;
    
    void construct_trie();
    int vaild_counter; 
    Subset subset;     
    bool is_updated;    

    int recursion_counter; 

    /* Bound */
    int kesm_weight;                   
    bool *visited;              
    clock_t start_time, end_time; 
    /* Layer Bound */
  
    //int achain_test_counter;
    double maxflow_time;
    vector<int> maxflow_layers;
    vector<int> maxflow_bounds;
    vector<int> rule2_kesm_weight;
 
    bool layer_bound_flag1;
    bool layer_bound_flag2;
    Maxflow GF = Maxflow(false); 
    int layer_bound;
    int L_current; 
    int L_max;    
    int r_maxflow; 
    int L_maxflow; 
    int L_stop;    
    bool stop_flag;
    bool cross_flag;
    
    bool test_layer_bound();
    bool update_maxflow_bound(int k);
    bool test_cross_antichain(const Antichain &x);
    bool test_cross_candidate(const vector<int> &x, int start, int end);

    /* Subset Bound */
  
    int pruning_counter;
    double subset_time;

    set<int> pruned_rotations;
    TopK_Bounds invaild_bounds;
 
    void update_pruned_rotations(int k);
    void update_invaild_bound(int r, int k);

 

    int calculate_weight(Subset x);
  
    Subset antichain_to_closedsubset(Antichain antichain);


    bool save_results(const string &results_file, string method, double time, map<string, int> counters, map<string, string> lists, vector<ClosedSubset> kesm_results);
    string convert_list(const vector<int> info);
    void package_results(const string &results_file, double runtime, vector<ClosedSubset> kesm_results);


    bool verbose_case;
    bool verbose_result;
    void print_results(vector<ClosedSubset> results);
    void print_adj(vector<set<int>> adj, string type);
    void print_antichain(Antichain antichain);
    void print_subset(Subset subset);
    void print_AQ_item(AntichainQueueItem AQ_item);
};

#endif
