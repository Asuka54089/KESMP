#include "../include/sm_operation.h"

using namespace std;

SmOperation::SmOperation(){}


void SmOperation::init(vector<vector<int>> &male_prefers, vector<vector<int>> &female_prefers,vector<int> &matching,vector<Rotation> &rotations){
    init_positions(male_prefers, female_prefers);
    //init_rotations(rotations);
    int_male_opt_matching(matching);
    init_elim_rotations(rotations);

}

void SmOperation::init_positions(vector<vector<int>> &male_prefers, vector<vector<int>> &female_prefers)
{
    int size = male_prefers.size();
    for(int i=0; i<size;i++){
        vector<int> tmp;
        for(int j=0; j<size;j++){
            tmp.push_back(-1);
        }   
        male_positions.push_back(tmp);
        female_positions.push_back(tmp);
    }

    for(int i=0; i<size;i++){
        for(int j=0; j<size;j++){
            male_positions[i][male_prefers[i][j]] = j;
            female_positions[i][female_prefers[i][j]] = j;
        }
    }
}

// void SmOperation::init_rotations(vector<Rotation> &rotations){
//     this->rotations = rotations;
// }

void SmOperation::init_elim_rotations(vector<Rotation> &rotations){
    Rotation elim_r;
    int m =-1;
    int w = -1;
    for(Rotation r : rotations){
        elim_r.clear();
        vector<int> next_females;
        r.get_next_females(next_females);
        for(int i=0; i<r.size();i++){
            m = r.Rm[i];
            w = next_females[i];
            elim_r.insert(m,w);
            
        }
        elim_r.weight = r.weight;
        elim_r.depth = r.depth;
        eliminated_rotations.push_back(elim_r);
    }
}

void SmOperation::int_male_opt_matching(vector<int> &matching){
    this->male_opt_matching = matching;
    this->male_esm_score = calculate_esm_score(matching);
}


vector<int> SmOperation::closedsubset_to_matching(set<int> &subset){
    vector<int> matching = male_opt_matching;
    int m=-1;
    int w=-1;
    for(int r : subset){
        for(int i=0; i<eliminated_rotations[r].size();i++){
            m = eliminated_rotations[r].Rm[i];
            w = eliminated_rotations[r].Rw[i];
            matching[m] = w; 
        }
    }
    return matching;
}

int SmOperation::calculate_esm_score(vector<int> &matching){
    int w_score = 0;
    int m_score = 0;
    int w = 0;
    for(int m=0;m<matching.size();m++)
    {
        w = matching[m];
        // the female position that male can get
        m_score = m_score + male_positions[m][w];
        w_score = w_score + female_positions[w][m];
    }
    return m_score + w_score;
}

int SmOperation::calculate_diff_S(set<int> &subset){
    int diff_S = 0;
    int r = -1;
    
    for(int r:subset)
    {
        diff_S = diff_S + eliminated_rotations[r].weight;
    }
    return diff_S;
}
