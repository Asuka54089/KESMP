#ifndef UTILS_H_
#define UTILS_H_

#include <iostream>
#include <string>
#include <vector>
#include <set>
#include <utility>
#include <algorithm>
#include <fstream>
#include <sstream>
//ubuntu
#include <experimental/filesystem>
//mac
//#include <filesystem>

#include "../include/rotation.h"
#include "../include/structs.h"
// #include "../include/sm_operation.h"

using namespace std;
//ubuntu
namespace fs = std::experimental::filesystem;
//mac OS
//namespace fs = std::filesystem;


void load_preferences(const string &dir, vector<vector<int>> &male_prefers,vector<vector<int>> &female_prefers);

void load_matchings(const string &dir, vector<int> &matching1,vector<int> &matching2);

void load_rotations(const string &rotation_dir, vector<Rotation> &rotations);
void load_rotation_edges(const string &rotation_dir, set<pair<int, int>> &rotation_edges);

/* Print */
void print_adj(vector<set<int>> &adj, string type);
void print_matching(vector<int> &matching, string title);
void print_rotation_set(set<int> &rotation_set, string title);


#endif