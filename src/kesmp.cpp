

#include "../include/kesmp.h"

using namespace std;


KESMP::KESMP(){}

void KESMP::init(vector<vector<int>> &male_prefers, vector<vector<int>> &female_prefers,vector<int> &matching,vector<Rotation> &rotations,set<pair<int, int>> &rotation_edges){
    antichain_counter=0;
    smo.init(male_prefers, female_prefers,matching,rotations);
    smg.init(rotation_edges,rotations.size(),rotations.back().depth);
}

void KESMP::init_method() {}

/* TopK */
void KESMP::init_topk()
{
    // Maintain candidate top-k results
    TKSM_object.clear();
    tmp_S.clear();
    antichain_counter = 0;
    update_counter = 0;
}

/* TopK */
bool KESMP::update_kesm(set<int> &S, int k)
{
    antichain_counter++;

    //print_rotation_set(S, to_string(S.size()));
    int diff_S = smo.calculate_diff_S(S);
    //cout<< antichain_counter <<endl;
    ClosedSubset x(diff_S, S);
    is_updated = TKSM_object.update(x, k);

    if (is_updated)
    {
        update_counter++;
    }
    return is_updated;
}

/* TopK */
vector<ClosedSubset> KESMP::find_topk_S(int k) {}

/* Layer Bound */
void KESMP::init_layer_bounds()
{
    layer_bound_time = 0;


    lb_flag_rotation = test_flag_rotation();
    if (lb_flag_rotation == false)
    {
        return;
    }


    //init bound
    layer_bound = INT_MAX;

    //layer
    L_max = smg.max_depth; //the last rotation
    L_current = L_max;              //shrink from the max layer of G_R
    L_stop = -1;                    //init, if we don't have any candidate top-k results, the graph should shrink to the first layer. (layer 0)
    lb_flag_stop = false;  // init, we can't prune now.

    //apply mincut on the whole G_R to generate max subset
    lb_flag_mincutset = true;

    vector<int> rotation_weights;
    vector<int> rotation_depths;
    for(int i=0;i<smg.rotation_num;i++){
        rotation_weights.push_back(smo.eliminated_rotations[i].weight);
        rotation_depths.push_back(smo.eliminated_rotations[i].depth);
    }
    GF.init_flow_graph(smg.children, rotation_weights, rotation_depths);
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
    for (int i=0;i<smg.rotation_num;i++)
    {
        if (smo.eliminated_rotations[i].weight > 0)
        {
            positive = true;
            break;
        }
    }
    for (int i=0;i<smg.rotation_num;i++)
    {
        if (smo.eliminated_rotations[i].weight < 0)
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
        if (smo.eliminated_rotations[i].depth > L_stop)
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
        if (x[i] > 0 && smo.eliminated_rotations[i].depth > L_stop)
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
    L_maxflow = smo.eliminated_rotations[r_maxflow].depth - 1;


    mincut_layers.push_back(L_maxflow); 

    S = smg.antichain_to_closedsubset(X);
    layer_bound = smo.calculate_diff_S(S);

    diff_kS = TKSM_object.tail(k);
    mincut_bounds.push_back(layer_bound); 
    mincut_diff_kS.push_back(diff_kS);        
    if (layer_bound < diff_kS)
    {
        L_stop = L_current;
        lb_flag_stop = true;
    }

    end_time = clock();
    layer_bound_time = layer_bound_time + (double)(end_time - start_time) / CLOCKS_PER_SEC;

    bool x = lb_flag_stop;
    return x;
}



/* Rotation Bound */
void KESMP::init_rotation_bounds()
{
    pruned_rotation_counter = 0;
    rotation_bound_time = 0;
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
    int r_d = smo.eliminated_rotations[r].depth;

    for (i = 0; i < smg.rotation_num; i++)
    {
        if (smo.eliminated_rotations[i].weight > 0)
        {
            if (smo.eliminated_rotations[i].weight <= r_d)
            {
                positiveR_prev = positiveR_prev + smo.eliminated_rotations[i].weight;
            }
            else
            {
                positiveR_succ = positiveR_succ + smo.eliminated_rotations[i].weight;
            }
        }
    }

    for (int i : smg.succ[r])
    {
        if (smo.eliminated_rotations[i].weight > 0)
        {
            positiveR_succ = positiveR_succ - smo.eliminated_rotations[i].weight;
        }
    }

    for (int i : smg.pred[r])
    {
        if (smo.eliminated_rotations[i].weight < 0)
        {
            positiveR_prev = positiveR_prev + smo.eliminated_rotations[i].weight;
        }
    }

    int r_bound = positiveR_prev + positiveR_succ;

    diff_kS = TKSM_object.tail(k);

    if (diff_kS > r_bound)
    {
        pruned_rotations.insert(r);
        pruned_rotation_counter++;
    }
    else
    {
        RotationBound rb = RotationBound(r, r_bound);
        rotation_bounds.insert(rb);
    }

    end_time = clock();
    rotation_bound_time = rotation_bound_time + (double)(end_time - start_time) / CLOCKS_PER_SEC;
}

/* Rotation Bound */
void KESMP::update_pruned_rotations(int k)
{
    start_time = clock();

    diff_kS = TKSM_object.tail(k);

    while (!rotation_bounds.empty())
    {
        RotationBound temp = rotation_bounds.pop();
        if (diff_kS > temp.bound)
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
    rotation_bound_time = rotation_bound_time + (double)(end_time - start_time) / CLOCKS_PER_SEC;
}



bool KESMP::save_results(const string &results_file, string method, double runtime, map<string, int> &counters, map<string, string> &lists, vector<ClosedSubset> &best_kSs)
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
    for (int i = best_kSs.size()-1; i>=0; i--)
    {
        ClosedSubset Si = best_kSs[i];
        output_file << smo.male_esm_score-Si.diff_S << endl;
        for (int j : Si.rotations)
        {
            output_file << j << " ";
        }
        output_file << endl;
    }
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
void KESMP::package_results(const string &results_file, double runtime, vector<ClosedSubset> &best_kSs){};


