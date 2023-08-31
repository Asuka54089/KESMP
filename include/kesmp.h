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
#include "topk.h"
#include "sm_instance.h"
#include "sm_structure.h"
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
    KESMP(string instance_dir);
    string instance_dir;

    SmInstance smi;
    RotationPoset poset;
    // Init instance and poset
    void init();

    /* Top-K */
    // Top-K Candidate Results
    TopK_StableMarriages topk_object;
    vector<ClosedSubset> tmp_S;
    // update the candidate top-k results
    bool update_kesm(set<int> &S, int k);
    int kth_deltaS; // the weight of the candidate k-th closed subset (current bound)

    // Top-K Methods
    virtual vector<ClosedSubset> find_topk_closedsubsets(int k);

    /* Counters */
    int sm_counts; //# enumerated antichains(# update TKSM)
    int update_counts;    //# replace a closed subset in TKSM

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
    int vaild_counts;

    /* Bound */
    bool *visited;                //is r visited ?(as a start rotation)
    clock_t start_time, end_time; // time

    /* Layer Bound */
    double lb_time;
    vector<int> lb_layers;  //layers when applying mincut
    vector<int> lb_bounds;  //bounds when applying mincut
    vector<int> lb_k_deltaS; // the deltaS when applying mincut

    //flags
    bool lb_flag_rotation;  //mincut works with positive rotations + negative rotations.
    bool lb_flag_mincutset; // layer bound can't continue to work if max subset is empty.
    bool lb_flag_stop;      // stop enumeration
    bool lb_flag_cross;     // candidate rotation + antichain have rotatioms in the two sides on stop layer
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

    int L_stop; // the stop layer

    // update layer bound
    bool update_layer_bound(int k);

    /* Rotation Bound */
    // counts
    int pruned_rotation_counts;
    double rb_time;

    // for promising rotations
    set<int> pruned_rotations;
    RotationBounds rotation_bounds;

    void update_pruned_rotations(int k);
    void calculate_rotation_bound(int r, int k);

    bool evaluate(vector<ClosedSubset> &results);
    bool evaluate(set<int> &S);

    /* save results */
    bool save_results(const string &results_file, string method, double time, map<string, int> &counts, map<string, string> &lists, vector<ClosedSubset> &topk_closed_subsets);
    string convert_list(const vector<int> info);
    void package_results(double runtime, vector<ClosedSubset> &topk_closed_subsets);
};

#endif
