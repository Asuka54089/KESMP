#ifndef ENTITIES_ROTATION_POSET_H_
#define ENTITIES_ROTATION_POSET_H_

#include <set>
#include "../include/entities_rotation.h"
#include "../include/sm_instance.h"

using namespace std;

class RotationPoset
{
public:
    RotationPoset();

    void init(int n, vector<Rotation> &rotations, vector<pair<int, int>> &edges);

    int n, node_size, edge_size;
    int max_depth;

    // vector<Rotation> rotations; 
    vector<Rotation> eliminated_rotations; //ordered pair lists after elimination

    //children & parents
    vector<set<int>> children;
    vector<set<int>> parents;

    //predecessor & succdecessor
    vector<set<int>> pred;
    vector<set<int>> succ;
    //init predecessor & succdecessor

    void init_rotations(vector<Rotation> &rotations);
    void init_graph(vector<pair<int, int>> &edges);
    void init_pred_succ();

    //Elimiantion
    // vector<int> closedsubset_to_matching(vector<int> &matching, set<int> &subset);
    // Marriage eliminate(Marriage &male_opt, set<int> &S, SmInstance &smi);
    // Marriage eliminate(Marriage &M, Rotation &r, SmInstance &smi);

    //Graph Operations
    // antichain --> closed subset (A-->S,by BFS)
    set<int> A_to_S(set<int> &A);
    vector<int> S_to_M(set<int> &subset, vector<int> male_opt_matching);
    int calculate_delta_S(set<int> &S);


};

#endif
