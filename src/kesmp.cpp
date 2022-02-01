

#include "../include/kesmp.h"

using namespace std;


KESMP::KESMP(bool verbose_case, bool verbose_result)
{
    this->verbose_case = verbose_case;
    this->verbose_result = verbose_result;
}

void KESMP::init_rotation_graph(set<pair<int, int>> rotation_edges, int rotation_num, vector<int> rotation_weights, vector<int> rotation_depths)
{
    
    this->rotation_weights = rotation_weights;
    this->rotation_depths = rotation_depths;

    
    this->rotation_num = rotation_num;
    children.resize(rotation_num);
    
    for (auto iter = rotation_edges.begin(); iter != rotation_edges.end(); ++iter)
    {
        int rs = iter->first;
        int rt = iter->second;

        children[rs].insert(rt);
    }

    if (verbose_case)
    {
        cout << "rotation graph nodes:" << rotation_num << endl;
        cout << "rotation graph edges:" << rotation_edges.size() << endl;
        print_adj(children, "children");
    }
}

/* Enum */
void KESMP::construct_trie()
{
    pred.resize(rotation_num);
    succ.resize(rotation_num);


    for (int i = 0; i < rotation_num; i++)
    {
        for (auto j : children[i])
        {
            pred[j].insert(i);
            succ[i].insert(j);
            for (auto k : pred[i])
            {
                pred[j].insert(k);
                succ[k].insert(j);
            }
        }
    }
    if (verbose_case)
    {
        print_adj(succ, "succ");
        print_adj(pred, "pred");
    }
}

void KESMP::init_method() {}

/* TopK */
void KESMP::init_topk()
{
    //maintain topk results
    kesm_heap.clear();
    tmp_subsets.clear();
    antichain_counter = 0;
    update_counter = 0;
}

/* TopK */
bool KESMP::update_kesm(Subset subset, int k)
{
    antichain_counter++;
    // cout << "count1:" << antichain_counter << endl;
    int weight = calculate_weight(subset);
    ClosedSubset x(weight, subset);
    is_updated = kesm_heap.update(x, k);
    if (verbose_case)
    {
        cout << "Weight:" << weight << endl;
        cout << "isUpdated:" << is_updated << endl;
        cout << endl;
    }
    // maintain topk result

    if (is_updated)
    {
        update_counter++;
    }
    return is_updated;
}

/* TopK */
vector<ClosedSubset> KESMP::find_posets(int k) {}

/* Layer Bound */
void KESMP::init_layer_bound()
{
    layer_bound_flag1 = test_layer_bound();
    if (layer_bound_flag1 == false)
    {
        return;
    }
    //init counter
    //achain_test_counter = 0;
    maxflow_time = 0;

    //init bound/topk
    layer_bound = INT_MAX;

    stop_flag = false;

    //layer
    L_max = rotation_depths.back(); 
    L_current = L_max;              
    L_stop = -1;                    

    //GF = Maxflow(verbose_case);
    layer_bound_flag2 = true;
    GF.init_flow_graph(children, rotation_weights, rotation_depths);
    update_maxflow_bound(-1); //计算完整图的maxflow bound
    if (layer_bound_flag2 == false)
    {
        return;
    }

    cross_flag = true;
}

/* Layer Bound */
bool KESMP::test_layer_bound()
{
    bool positive = false;
    bool negative = false;
    for (int w : rotation_weights)
    {
        if (w > 0)
        {
            positive = true;
            break;
        }
    }
    for (int w : rotation_weights)
    {
        if (w < 0)
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
bool KESMP::update_maxflow_bound(int k)
{
    start_time = clock();

    GF.EdmondsKarp(L_current);
    set<int> x = GF.min_cut();
    if (x.size() == 0)
    {
        layer_bound_flag2 = false;
        return false;
    }
    r_maxflow = *x.rbegin();
    L_maxflow = rotation_depths[r_maxflow] - 1; 

    maxflow_layers.push_back(L_maxflow); 

    subset = antichain_to_closedsubset(x);
    layer_bound = calculate_weight(subset);

    kesm_weight = kesm_heap.tail(k);
    maxflow_bounds.push_back(layer_bound); 
    rule2_kesm_weight.push_back(kesm_weight);        
    if (layer_bound < kesm_weight)
    {
        L_stop = L_current;
        stop_flag = true;
    }

    end_time = clock();
    maxflow_time = maxflow_time + (double)(end_time - start_time) / CLOCKS_PER_SEC;

    if (verbose_case)
    {
        cout << "update layer bound" << endl;
        cout << "current Layer:" << L_current << endl;
        cout << "r_maxflow:" << r_maxflow << endl;
        cout << "L_maxflow:" << L_maxflow << endl;
        cout << "Maximum closed subset:" << endl;
        print_subset(subset);
        cout << "layer_bound:" << layer_bound << endl;
        cout << "kesm:" << kesm_weight << endl;
        cout << "stop_flag:" << stop_flag << endl;
        cout << "L_stop:" << L_stop << endl;
        // cout << "update_counter " << update_counter << endl;
        cout << endl;
    }

    return stop_flag;
}

/* Layer Bound */
bool KESMP::test_cross_antichain(const Antichain &x)
{
    //achain_test_counter++;
    for (int i : x)
    {
        if (rotation_depths[i] > L_stop)
        {
            //cout << "true" << endl;
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
        if (x[i] > 0 && rotation_depths[i] > L_stop)
        {
            //cout << "true" << endl;
            return true;
        }
    }
    return false;
}

/* Subset Bound */
void KESMP::init_subset_bound()
{
    pruning_counter = 0;
    subset_time = 0;
    invaild_bounds.clear();
    pruned_rotations.clear();
}

/* Subset Bound */

void KESMP::update_invaild_bound(int r, int k)
{
    start_time = clock();

    int positiveR_prev = 0;
    int positiveR_succ = 0;
    int i;
    int r_d = rotation_depths[r];

    for (i = 0; i < rotation_weights.size(); i++)
    {
        if (rotation_weights[i] > 0)
        {
            if (rotation_depths[i] <= r_d)
            {
                positiveR_prev = positiveR_prev + rotation_weights[i];
            }
            else
            {
                positiveR_succ = positiveR_succ + rotation_weights[i];
            }
        }
    }

    for (int i : succ[r])
    {
        if (rotation_weights[i] > 0)
        {
            positiveR_succ = positiveR_succ - rotation_weights[i];
        }
    }

    for (int i : pred[r])
    {
        if (rotation_weights[i] < 0)
        {
            positiveR_prev = positiveR_prev + rotation_weights[i];
        }
    }
    int half_bound = positiveR_prev;
    int all_bound = positiveR_prev + positiveR_succ;

    kesm_weight = kesm_heap.tail(k);

    if (kesm_weight > all_bound)
    {
        pruned_rotations.insert(r);
        pruning_counter++;
    }
    else
    {
        SubsetBound x = SubsetBound(r, all_bound);
        invaild_bounds.insert(x);
    }

    end_time = clock();
    subset_time = subset_time + (double)(end_time - start_time) / CLOCKS_PER_SEC;
}

/* Subset Bound */
void KESMP::update_pruned_rotations(int k)
{
    start_time = clock();

    kesm_weight = kesm_heap.tail(k);

    while (!invaild_bounds.empty())
    {
        SubsetBound temp = invaild_bounds.pop();
        if (kesm_weight > temp.bound)
        {
            pruned_rotations.insert(temp.rotation);
        }
        else
        {
            invaild_bounds.insert(temp);
            break;
        }
    }

    end_time = clock();
    subset_time = subset_time + (double)(end_time - start_time) / CLOCKS_PER_SEC;
}


int KESMP::calculate_weight(Subset x)
{
    int weight = 0;
    for (int i : x)
    {
        weight = weight + rotation_weights[i];
    }
    return weight;
}

Subset KESMP::antichain_to_closedsubset(Antichain antichain)
{
    Subset closed_subset;
    for (int r : antichain)
    {
        closed_subset.insert(r);
        closed_subset.insert(pred[r].begin(), pred[r].end());
    }
    return closed_subset;
}


bool KESMP::save_results(const string &results_file, string method, double runtime, map<string, int> counters, map<string, string> lists, vector<ClosedSubset> kesm_results)
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
    for (ClosedSubset i : kesm_results)
    {
        output_file << i.weight << endl;
        for (int j : i.rotations)
        {
            output_file << j << " ";
        }
        output_file << endl;
    }
    output_file.close();
}


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

void KESMP::package_results(const string &results_file, double runtime, vector<ClosedSubset> results){};

void KESMP::print_adj(vector<set<int>> adj, string type)
{
    cout << "-----" << type << "-----" << endl;
    for (int i = 0; i < adj.size(); i++)
    {
        cout << i << endl;
        for (auto itr = adj[i].begin(); itr != adj[i].end(); itr++)
        {
            cout << *itr << " ";
        }
        cout << endl;
        cout << endl;
    }
}

void KESMP::print_results(vector<ClosedSubset> results)
{
    cout << "antichain_counter: " << antichain_counter << endl;
    cout << "update_counter: " << update_counter << endl;
    for (int i = 0; i < results.size(); i++)
    {
        cout << i << endl;
        cout << "weight: " << results[i].weight << endl;
        print_subset(results[i].rotations);

        cout << endl;
    }
}

void KESMP::print_antichain(Antichain antichain)
{
    cout << "Antichain: ";
    for (auto itr = antichain.begin(); itr != antichain.end(); itr++)
    {
        cout << *itr << " ";
    }
    cout << endl;
}

void KESMP::print_subset(Subset subset)
{
    cout << "Subset: ";
    for (auto itr = subset.begin(); itr != subset.end(); itr++)
    {
        cout << *itr << " ";
    }
    cout << endl;
}

void KESMP::print_AQ_item(AntichainQueueItem AQ_item)
{
    cout << "AQ_Antichain: ";
    for (auto n : AQ_item.antichain)
    {
        cout << n << " ";
    }

    cout << "AQ_Candidates: ";
    for (auto n : AQ_item.candidates)
    {
        cout << n << " ";
    }
    cout << endl;
}
