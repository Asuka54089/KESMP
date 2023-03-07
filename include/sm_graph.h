#ifndef SM_GRAPH_H_
#define SM_GRAPH_H_

// #include <iostream>
// #include <vector>
// #include "../lib/stdc++.h"

#include "rotation.h"
#include "structs.h"
using namespace std;

// Rotation Graph Class
// variables of rotation graph
// opeations on rotation graph
class SmGraph
{
public:
    
    SmGraph();
    
    int rotation_num;
    int max_depth;

    //children & parents
    vector<set<int>> children;
    vector<set<int>> parents;
    //init rotation graph
    void init(set<pair<int, int>> &rotation_edges, int rotation_num,int max_depth);

    //predecessor & succdecessor
    vector<set<int>> pred;
    vector<set<int>> succ;
    //init predecessor & succdecessor
    void init_pred_and_succ();
    


    // antichain --> closed subset (A-->S,by BFS)
    set<int> antichain_to_closedsubset(set<int> antichain);

};


#endif
