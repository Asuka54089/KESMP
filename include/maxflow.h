#ifndef MAXFLOW_H_
#define MAXFLOW_H_

#include <iostream>
#include <string.h>
#include <vector>
#include <utility>
#include <set>
#include <queue>
#include "limits.h"

//input
//rotation weight
//rotation edges

//output
//maxflow
//rotaiton ids

class Maxflow
{
public:
    bool verbose;
    
    int pINF = INT_MAX;
    int nINF = INT_MIN;

    int rotation_num;     
    int **flow_graph;     
    int **residual_graph; 
    bool *rotation_mask;      

   
    std::vector<int> rotation_depths;

    std::vector<int> rotation_weights;

    Maxflow(bool verbose);

   
    void init_flow_graph(std::vector<std::set<int>> children, std::vector<int> rotation_weights, std::vector<int> rotation_depths);


    void shrink_flow_graph(int outlayer);
    
    void construct_residual_graph();

   
    bool bfs(int s, int t, int *parent);
    int EdmondsKarp(int outlayer);

    
    void dfs(int **graph, const int s, bool *visited);
    std::set<int> min_cut();

private:
    void print_residual_graph();
    void print_path(int *parent, int size);
};

#endif
