#include "../include/entities_rotation_poset.h"

using namespace std;

RotationPoset::RotationPoset() {}

void RotationPoset::init(int n, vector<Rotation> &rotations, vector<pair<int, int>> &edges)
{

    this->n = n;
    node_size = rotations.size();
    edge_size = edges.size();

    init_rotations(rotations);
    // this->rotations = rotations;
    init_graph(edges);
}

void RotationPoset::init_rotations(vector<Rotation> &rotations)
{
    Rotation tmp_r;
    int m, w2 = -1;

    for (Rotation r : rotations)
    {
        tmp_r.clear();
        vector<int> next_women;
        r.get_next_women(next_women);
        for (int i = 0; i < r.size; i++)
        {
            m = r.men[i];
            //w1 = r.women[i];
            w2 = next_women[i];
            tmp_r.insert(m, w2);
        }

        tmp_r.depth = r.depth;
        tmp_r.weight = r.weight;
        eliminated_rotations.push_back(tmp_r);
    }
}

void RotationPoset::init_graph(vector<pair<int, int>> &edges)
{
    //init children & parents
    children.resize(node_size);
    parents.resize(node_size);
    for (auto edge : edges)
    {
        int rs = edge.first;
        int rt = edge.second;

        children[rs].insert(rt);
        parents[rt].insert(rs);
    }

    int max_layer = 0;
    for (int r = 0; r < node_size; r++)
    {
        if (parents[r].size() == 0)
        {
            eliminated_rotations[r].depth = 0;
        }
        else
        {
            max_layer = 0;
            for (auto i : parents[r])
            {
                max_layer = (max_layer > eliminated_rotations[i].depth) ? max_layer : eliminated_rotations[i].depth;
            }
            eliminated_rotations[r].depth = max_layer + 1;
        }
    }

    max_depth = eliminated_rotations[node_size - 1].depth;
}

void RotationPoset::init_pred_succ()
{
    pred.resize(node_size);
    succ.resize(node_size);
    for (int i = 0; i < node_size; i++)
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
}

// Marriage RotationPoset::eliminate(Marriage &male_opt, set<int> &S, SmInstance &smi)
// {
//     int m, w = -1;
//     int m_pos, w_pos = -1;

//     Marriage X;
//     vector<int> Rank_men_partner(male_opt.Rank_men_partner);
//     vector<int> Rank_women_partner(male_opt.Rank_women_partner);

//     for (int r : S)
//     {
//         for (int i = 0; i < rotations[r].size; i++)
//         {
//             m = rotations[r].men[i];
//             w = rotations[r].women[rotations[r].get_next_index(i)];
//             m_pos = smi.Rank_w(w, m);
//             w_pos = smi.Rank_m(m, w);
//             Rank_men_partner[m] = w_pos;
//             Rank_women_partner[w] = m_pos;
//         }
//     }
//     X.init(Rank_men_partner, Rank_women_partner);
//     return X;
// }

// Marriage RotationPoset::eliminate(Marriage &M, Rotation &r, SmInstance &smi)
// {
//     int m, w = -1;
//     int m_pos, w_pos = -1;

//     Marriage X;
//     vector<int> Rank_men_partner(M.Rank_men_partner);
//     vector<int> Rank_women_partner(M.Rank_women_partner);

//     for (int i = 0; i < r.size; i++)
//     {
//         m = r.men[i];
//         w = r.women[r.get_next_index(i)];
//         m_pos = smi.Rank_w(w, m);
//         w_pos = smi.Rank_m(m, w);
//         Rank_men_partner[m] = m_pos;
//         Rank_women_partner[w] = w_pos;
//     }

//     X.init(Rank_men_partner, Rank_women_partner);
//     return X;
// }



set<int> RotationPoset::A_to_S(set<int> &A)
{
    set<int> S;
    for (int r : A)
    {
        S.insert(r);
        S.insert(pred[r].begin(), pred[r].end());
    }
    return S;
}

vector<int> RotationPoset::S_to_M(set<int> &subset, vector<int> male_opt_matching){
    vector<int> matching = male_opt_matching;
    int m=-1;
    int w=-1;
    for(int r : subset){
        for(int i=0; i< eliminated_rotations[r].size;i++){
            m = eliminated_rotations[r].men[i];
            w = eliminated_rotations[r].women[i];
            matching[m] = w; 
        }
    }
    return matching;
}

int RotationPoset::calculate_delta_S(set<int> &subset){
    int delta_S = 0;
    int r = -1;
    
    for(int r:subset)
    {
        delta_S = delta_S + eliminated_rotations[r].weight;
    }
    return delta_S;
}
