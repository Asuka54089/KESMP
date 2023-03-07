// Mincut Class

//[Input]
//rotation weights
//rotation edges

//[Output]
//the maximum-weighted closed subset

// layer bounds are calculated in kesmp.h


#ifndef BOUND_MINCUT_H_
#define BOUND_MINCUT_H_

#include <iostream>
#include <string.h>
#include <vector>
#include <utility>
#include <set>
#include <queue>
#include "limits.h"


class Mincut
{
public:
    bool verbose;
    
    int pINF = INT_MAX;
    int nINF = INT_MIN;

    int rotation_num;
    int **flow_graph;     //It is static(G[L]).
    int **residual_graph; //It will be updated by masks to generate the layer subgraphs(G[L],G[L-1],...,G[1]), then we apply Mincut algorithm on it.）
    bool *rotation_mask;    

    // for residual graph construction
    std::vector<int> rotation_depths;
    //for mincut generation
    std::vector<int> rotation_weights;

    Mincut(bool verbose);

    //rotation graph --> flow graph
    //init residual_graph (by flow graph)
    //depths is used in residual_graph construction
    void init_flow_graph(std::vector<std::set<int>> children, std::vector<int> rotation_weights, std::vector<int> rotation_depths);

    // Give the outlayer, it updates mask for a layer subgraph
    // Contain rotations in outlayer
    // layer index is from layer 0
    void shrink_flow_graph(int outlayer);
    // construct the residual_graph for a layer subgraph by the mask
    void construct_residual_graph();

    // EdmondsKarp
    bool bfs(int s, int t, int *parent);
    int EdmondsKarp(int outlayer);

    //min-cut --> maximum closed subset
    void dfs(int **graph, const int s, bool *visited);
    std::set<int> min_cut();

private:
    void print_residual_graph();
    void print_path(int *parent, int size);
};

#endif