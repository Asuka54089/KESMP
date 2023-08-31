#include "../include/sm_graph.h"

using namespace std;

SmGraph::SmGraph(){}

void SmGraph::init(set<pair<int, int>> &rotation_edges, int rotation_num,int max_depth){
    // init rotation graph
    this->rotation_num = rotation_num;
    this->max_depth = max_depth;
    
    //init children & parents
    children.resize(rotation_num);
    parents.resize(rotation_num);
    for (auto iter = rotation_edges.begin(); iter != rotation_edges.end(); ++iter)
    {
        int rs = iter->first;
        int rt = iter->second;

        children[rs].insert(rt);
        parents[rt].insert(rs);
    }

}

void SmGraph::init_pred_and_succ(){
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
}



set<int> SmGraph::antichain_to_closedsubset(set<int> antichain)
{
    set<int> closed_subset;
    for (int r : antichain)
    {
        closed_subset.insert(r);
        closed_subset.insert(pred[r].begin(), pred[r].end());
    }
    return closed_subset;
}



