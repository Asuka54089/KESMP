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

#include "utils.h"
#include "structs.h"
#include "topk_sm.h"
#include "sm_operation.h"
#include "sm_graph.h"
#include "bound_mincut.h"
#include "bound_rotation.h"


// KESMP (basic class)
// init top-k results & update
// init layar bound & functions
// init rotation bound & functions
// save results
class KESMP
{
public:
    // Static Variables
    const int MAX_ANTICHAIN_NUM = 10000000;

    /* Input a SM instance */
    KESMP();
    // Stable Marraige Operation Object
    SmOperation smo;
    // Rotation Graph Object
    SmGraph smg;
    // Init smg and smo
    void init(vector<vector<int>> &male_prefers, vector<vector<int>> &female_prefers,vector<int> &matching,vector<Rotation> &rotations,set<pair<int, int>> &rotation_edges);
   
    /* Top-K */
    // Top-K Candidate Results
    TopK_StableMarriages TKSM_object;
    vector<ClosedSubset> tmp_S;
    // update the candidate top-k results
    bool update_kesm(set<int> &S, int k);
    int diff_kS; // the weight of the candidate k-th closed subset
    
    // Top-K Methods
    virtual vector<ClosedSubset> find_topk_S(int k);
    
    /* Counters */ 
    int antichain_counter; //# enumerated antichains(# update TKSM)
    int update_counter; //# replace a closed subset in TKSM

    /* Init */ 
    // init for different methods
    void init_method();
    // init for topk results
    void init_topk();
    // init for bounds
    void init_layer_bounds();
    void init_rotation_bounds();


    // Variables
    // for Top-K Candidate Results
    bool is_updated;
    // for Top-K Methods
    set<int> S;
    int vaild_counter;


    /* Bound */
    bool *visited;              //is r visited ?(as a start rotation)
    clock_t start_time, end_time; // time

    /* Layer Bound */
    double layer_bound_time;
    vector<int> mincut_layers; //layers when applying mincut
    vector<int> mincut_bounds; //bounds when applying mincut
    vector<int> mincut_diff_kS; // the diff_kS when applying mincut
    
    //flags
    bool lb_flag_rotation; //mincut works with positive rotations + negative rotations.
    bool lb_flag_mincutset;// layer bound can't continue to work if max subset is empty. 
    bool lb_flag_stop; // stop enumeration
    bool lb_flag_cross; // candidate rotation + antichain have rotatioms in the two sides on stop layer
    //check these flags
    bool test_flag_rotation();
    bool test_flag_A_iscross(const set<int> &x);
    bool test_cross_candidate(const vector<int> &x, int start, int end);
    
    // flow network
    Mincut GF = Mincut(false); //index of layer is from 0
    int layer_bound;
    
    // layers
    int L_current; // the layer of the start rotation
    int L_max;     // max layer of G_R
    int L_maxflow; // the next layer that we can apply mincut
    int r_maxflow; // the rotation in the outlayer of the current closed subset, if this r is visited, we can apply mincut to G[r.L-1].
    
    int L_stop;    // the stop layer

    // update layer bound
    bool update_layer_bound(int k);
   
   

    /* Rotation Bound */
    // counters
    int pruned_rotation_counter;
    double rotation_bound_time;

    // for promising rotations
    set<int> pruned_rotations;
    RotationBounds rotation_bounds;
    
    void update_pruned_rotations(int k);
    void calculate_rotation_bound(int r, int k);


    /* save results */
    bool save_results(const string &results_file, string method, double time, map<string, int> &counters, map<string, string> &lists, vector<ClosedSubset> &best_kSs);
    string convert_list(const vector<int> info);
    void package_results(const string &results_file, double runtime, vector<ClosedSubset> &best_kSs);

};

#endif