#ifndef SM_OPERATION_H_
#define SM_OPERATION_H_

#include <iostream>
#include <vector>
#include <set>
#include <algorithm>
#include <map>
// mac OS
//#include "../lib/stdc++.h"
// ubuntu
//#include <bits/stdc++.h>

#include "rotation.h"
#include "structs.h"
using namespace std;

// Stable Marraige Operation Class
// variables of stable marriage instance
// operations on matchings and closed subsets
class SmOperation
{
public:
    
    SmOperation();

    // init sm instance
    void init(vector<vector<int>> &male_prefers, vector<vector<int>> &female_prefers,vector<int> &matching,vector<Rotation> &rotations);
    

    void init_positions(vector<vector<int>> &male_prefers, vector<vector<int>> &female_prefers);
    vector<vector<int>> male_positions,female_positions;
    
    void int_male_opt_matching(vector<int> &matching);
    vector<int> male_opt_matching;
    int male_esm_score;
    
    // store rotation pairs after elimination transformation (for directly opt-sm/S-->sm)
    void init_elim_rotations(vector<Rotation> &rotations);
    vector<Rotation> eliminated_rotations; 
    
    // closed subset --> stable marriage (opt-sm/S-->sm, by rotation elimination)
    vector<int> closedsubset_to_matching(set<int> &subset);

    /* Calculation */ 
    // sum by positons
    int calculate_esm_score(vector<int> &matching); 
    // sum by rotation weights
    int calculate_diff_S(set<int> &subset); 

};

#endif
