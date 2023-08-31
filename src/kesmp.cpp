

#include "../include/kesmp.h"

using namespace std;

KESMP::KESMP(string instance_dir)
{
    this->instance_dir = instance_dir;
}

void KESMP::init()
{
    sm_counts = 0;

    vector<vector<int>> men_prefs, Pref_women;
    load_preferences(instance_dir, men_prefs, Pref_women);

    smi.init(men_prefs, Pref_women);
    smi.gs_male_opt();
    smi.gs_female_opt();

    SmStructure sms;
    vector<Rotation> rotations = sms.find_rotations(smi);
    vector<pair<int, int>> edges = sms.construct_graph(rotations, smi);

    poset.init(smi.n, rotations, edges);
    cout << "construct poset" << endl;
    cout << "#r: " << poset.node_size << endl;
    cout << "#e: " << poset.edge_size << endl;
    // save_poset(instance_dir, poset);
}

void KESMP::init_method() {}

/* TopK */
void KESMP::init_topk()
{
    // Maintain candidate top-k results
    topk_object.clear();
    tmp_S.clear();
    sm_counts = 0;
    update_counts = 0;
}

/* TopK */
bool KESMP::update_kesm(set<int> &S, int k)
{
    sm_counts++;

    // if (evaluate(S) == false)
    // {
    //     cout << sm_counts << ":"
    //          << "not stable" << endl;
    // }

    //print_rotation_set(S, to_string(S.size()));
    int deltaS = poset.calculate_delta_S(S);
    //cout<< sm_counts <<endl;
    ClosedSubset x(deltaS, S);
    is_updated = topk_object.update(x, k);

    if (is_updated)
    {
        update_counts++;
    }
    return is_updated;
}

/* TopK */
vector<ClosedSubset> KESMP::find_topk_closedsubsets(int k) {}

/* Layer Bound */
void KESMP::init_layer_bounds()
{
    lb_time = 0;

    lb_flag_rotation = test_flag_rotation();
    if (lb_flag_rotation == false)
    {
        return;
    }

    //init bound
    layer_bound = INT_MAX;

    //layer
    L_max = poset.max_depth; //the last rotation
    L_current = L_max;       //shrink from the max layer of G_R
    L_stop = -1;             //init, if we don't have any candidate top-k results, the graph should shrink to the first layer. (layer 0)
    lb_flag_stop = false;    // init, we can't prune now.

    //apply mincut on the whole G_R to generate max subset
    lb_flag_mincutset = true;

    vector<int> rotation_weights;
    vector<int> rotation_depths;
    for (int i = 0; i < poset.node_size; i++)
    {
        rotation_weights.push_back(poset.eliminated_rotations[i].weight);
        rotation_depths.push_back(poset.eliminated_rotations[i].depth);
    }
    GF.init_flow_graph(poset.children, rotation_weights, rotation_depths);
    update_layer_bound(-1); //calculate the maximized closed subset for G_R
    if (lb_flag_mincutset == false)
    {
        return;
    }

    lb_flag_cross = true;
}

/* Layer Bound */
bool KESMP::test_flag_rotation()
{
    bool positive = false;
    bool negative = false;
    for (int i = 0; i < poset.node_size; i++)
    {
        if (poset.eliminated_rotations[i].weight > 0)
        {
            positive = true;
            break;
        }
    }
    for (int i = 0; i < poset.node_size; i++)
    {
        if (poset.eliminated_rotations[i].weight < 0)
        {
            negative = true;
            break;
        }
    }
    if (positive && negative)
    {
        return true;
    }
    return false;
}

/* Layer Bound */
bool KESMP::test_flag_A_iscross(const set<int> &x)
{
    for (int i : x)
    {
        if (poset.eliminated_rotations[i].depth > L_stop)
        {
            return true;
        }
    }
    return false;
}

/* Layer Bound */
bool KESMP::test_cross_candidate(const vector<int> &x, int start, int end)
{
    for (int i = end; i >= start; i--)
    {
        if (x[i] > 0 && poset.eliminated_rotations[i].depth > L_stop)
        {
            return true;
        }
    }
    return false;
}

/* Layer Bound */
bool KESMP::update_layer_bound(int k)
{
    start_time = clock();

    GF.EdmondsKarp(L_current);
    set<int> X = GF.min_cut();
    if (X.size() == 0)
    {
        lb_flag_mincutset = false;
        return false;
    }
    r_maxflow = *X.rbegin();
    // update the next layer we can apply mincut
    L_maxflow = poset.eliminated_rotations[r_maxflow].depth - 1;

    lb_layers.push_back(L_maxflow);

    S = poset.A_to_S(X);
    layer_bound = poset.calculate_delta_S(S);

    kth_deltaS = topk_object.tail(k);
    lb_bounds.push_back(layer_bound);
    lb_k_deltaS.push_back(kth_deltaS);
    if (layer_bound < kth_deltaS)
    {
        L_stop = L_current;
        lb_flag_stop = true;
    }

    end_time = clock();
    lb_time = lb_time + (double)(end_time - start_time) / CLOCKS_PER_SEC;

    bool x = lb_flag_stop;
    return x;
}

/* Rotation Bound */
void KESMP::init_rotation_bounds()
{
    pruned_rotation_counts = 0;
    rb_time = 0;
    rotation_bounds.clear();
    pruned_rotations.clear();
}

/* Rotation Bound */
//1.for successors: positive rotations - positive rotations in succ[r]
//2.for predecesors: negative rotations in prev[r]
void KESMP::calculate_rotation_bound(int r, int k)
{
    start_time = clock();

    int positiveR_prev = 0;
    int positiveR_succ = 0;
    int i;
    int r_d = poset.eliminated_rotations[r].depth;

    for (i = 0; i < poset.node_size; i++)
    {
        if (poset.eliminated_rotations[i].weight > 0)
        {
            if (poset.eliminated_rotations[i].weight <= r_d)
            {
                positiveR_prev = positiveR_prev + poset.eliminated_rotations[i].weight;
            }
            else
            {
                positiveR_succ = positiveR_succ + poset.eliminated_rotations[i].weight;
            }
        }
    }

    for (int i : poset.succ[r])
    {
        if (poset.eliminated_rotations[i].weight > 0)
        {
            positiveR_succ = positiveR_succ - poset.eliminated_rotations[i].weight;
        }
    }

    for (int i : poset.pred[r])
    {
        if (poset.eliminated_rotations[i].weight < 0)
        {
            positiveR_prev = positiveR_prev + poset.eliminated_rotations[i].weight;
        }
    }

    int r_bound = positiveR_prev + positiveR_succ;

    kth_deltaS = topk_object.tail(k);

    if (kth_deltaS > r_bound)
    {
        pruned_rotations.insert(r);
        pruned_rotation_counts++;
    }
    else
    {
        RotationBound rb = RotationBound(r, r_bound);
        rotation_bounds.insert(rb);
    }

    end_time = clock();
    rb_time = rb_time + (double)(end_time - start_time) / CLOCKS_PER_SEC;
}

/* Rotation Bound */
void KESMP::update_pruned_rotations(int k)
{
    start_time = clock();

    kth_deltaS = topk_object.tail(k);

    while (!rotation_bounds.empty())
    {
        RotationBound temp = rotation_bounds.pop();
        if (kth_deltaS > temp.bound)
        {
            pruned_rotations.insert(temp.rotation);
        }
        else
        {
            rotation_bounds.insert(temp);
            break;
        }
    }

    end_time = clock();
    rb_time = rb_time + (double)(end_time - start_time) / CLOCKS_PER_SEC;
}

bool KESMP::save_results(const string &results_file, string method, double runtime, map<string, int> &counters, map<string, string> &lists, vector<ClosedSubset> &topk_closed_subsets)
{

    ofstream output_file;
    output_file.open(results_file);

    //rotation(weight,depth,pairs)
    output_file << "method"
                << " " << method << endl;
    output_file << "time"
                << " " << runtime << endl;

    for (auto &t : counters)
    {
        output_file << t.first << " " << t.second << endl;
        //cout << t.first << t.second << endl;
    }

    for (auto &t : lists)
    {
        output_file << t.first << " " << t.second << endl;
        //cout << t.first << t.second << endl;
    }
    output_file << endl;

    // for (ClosedSubset Si : best_kSs)
    for (int i = topk_closed_subsets.size() - 1; i >= 0; i--)
    {
        ClosedSubset Si = topk_closed_subsets[i];
        output_file << smi.male_opt.egalitarian_cost - Si.deltaS << endl;
        for (int j : Si.rotations)
        {
            output_file << j << " ";
        }
        output_file << endl;
    }
    output_file.close();

    ofstream output_file2;
    output_file2.open(instance_dir + "_m");
    int m, w = 0;

    output_file2 << smi.male_opt.egalitarian_cost << endl;
    for (m = 0; m < smi.n; m++)
    {
        output_file2 << smi.male_opt_matching[m] << " ";
    }
    output_file2 << endl;
    output_file2 << smi.female_opt.egalitarian_cost << endl;
    for (m = 0; m < smi.n; m++)
    {
        output_file2 << smi.female_opt_matching[m] << " ";
    }
    output_file2 << endl;
    output_file.close();


}

/* save */
string KESMP::convert_list(const vector<int> info)
{
    string info_str;
    for (int i : info)
    {
        info_str = info_str + to_string(i) + " ";
    }
    info_str.pop_back();
    return info_str;
}

/* save */
void KESMP::package_results(double runtime, vector<ClosedSubset> &best_kSs){};

bool KESMP::evaluate(vector<ClosedSubset> &results)
{
    for (auto &t : results)
    {
        if (evaluate(t.rotations) == false)
        {
            return false;
        }
    }
    return true;
}

bool KESMP::evaluate(set<int> &S)
{
    vector<int> tmp = poset.S_to_M(S, smi.male_opt_matching);
    // smi.count_blocking_pairs(tmp);
    return smi.is_stable(tmp);
}