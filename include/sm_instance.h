#ifndef SM_INSTANCE_H_
#define SM_INSTANCE_H_

#include <iostream>
#include <vector>
#include <set>
#include <stack>

// #include <algorithm>
// #include <map>
// mac OS
//#include "../lib/stdc++.h"
// ubuntu
//#include <bits/stdc++.h>

#include "../include/lib/Format.h"

#include "../include/entities_marriage.h"

using namespace std;

class SmInstance
{
public:
    SmInstance();
    void init(vector<vector<int>> &Pref_men, vector<vector<int>> &Pref_women);

    int n;

    vector<vector<int>> Pref_men, Pref_women; // (m1, 1st) -> w2
    vector<vector<int>> Rank_men, Rank_women; // (m1, w2) -> 1st
    void invert_index();

    int Rank_w(int w, int m);
    int Rank_m(int m, int w);
    int Pref_m(int m, int i);
    int Pref_w(int w, int i);

    void gs_male_opt();
    void gs_female_opt();
    Marriage male_opt;
    Marriage female_opt;

    vector<int> male_opt_matching;
    vector<int> female_opt_matching;

    bool is_stable(vector<int> matching);
    int count_blocking_pairs(vector<int> matching);

    vector<int> marriage_to_matching(Marriage &M);
};

#endif
