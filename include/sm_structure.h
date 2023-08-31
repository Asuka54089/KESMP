#ifndef SM_STRUCTURE_H_
#define SM_STRUCTURE_H_

#include <iostream>
#include <vector>
#include <stack>
#include <utility>

// mac OS
//#include "../lib/stdc++.h"
// ubuntu
//#include <bits/stdc++.h>


#include "../include/sm_instance.h"
#include "../include/entities_marriage.h"
#include "../include/entities_rotation.h"
#include "../include/entities_rotation_poset.h"
using namespace std;

class SmStructure
{
public:
    SmStructure();

    vector<Rotation> find_rotations(SmInstance &smi);
    vector<pair<int, int>> construct_graph(vector<Rotation> &rotations, SmInstance &smi); //return edges

    int next(Marriage &M, int m, SmInstance &smi);
    void eliminate(Marriage &M, Rotation &r, SmInstance &smi);

    void link_rotations(vector<pair<int, int>> &edges, vector<Rotation> &rotations, SmInstance &smi);
    void sparsify_graph(vector<pair<int, int>> &edges, vector<Rotation> &rotations);
    void check_transitivity(int rotatio_size, vector<vector<int>> &graph);
    void dfs_util(int u, vector<bool> &visited, vector<vector<int>> &graph);

    void print_graph(vector<vector<int>> &graph);
};


#endif
