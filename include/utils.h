#ifndef UTILS_H_
#define UTILS_H_

#include <iostream>
#include <string>
#include <vector>
#include <set>
#include <map>
#include <utility>
#include <algorithm>
#include <fstream>
#include <sstream>
//ubuntu
#include <experimental/filesystem>
//mac
//#include <filesystem>

// #include "../include/rotation.h"
#include "../include/structs.h"
#include "../include/entities_rotation_poset.h"



using namespace std;
//ubuntu
namespace fs = std::experimental::filesystem;
//mac OS
//namespace fs = std::filesystem;


void load_preferences(const string &dir, vector<vector<int>> &male_prefers,vector<vector<int>> &female_prefers);

// void load_matchings(const string &dir, vector<int> &matching1,vector<int> &matching2);
// void load_rotations(const string &rotation_dir, vector<Rotation> &rotations);
// void load_rotation_edges(const string &rotation_dir, set<pair<int, int>> &rotation_edges);

// bool save_results(const string &result_file, string method, double runtime, map<string, int> &counts, map<string, string> &info, vector<ClosedSubset> &best_kSs, SmInstance &smi);
void save_poset(const string &instance_dir, RotationPoset &rotation_poset);


#endif