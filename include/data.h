#ifndef DATA_H_
#define DATA_H_

#include <iostream>
#include <string>
#include <vector>
#include <set>
#include <algorithm>
#include <fstream>
#include <sstream>
#include <experimental/filesystem>

using namespace std;
namespace fs = std::experimental::filesystem;

bool load_file_names(const string &path, vector<string> &file_names);

// load rotation info
void load_rotations(const string &path, vector<int> &rotation_weights, vector<int> &rotation_depths, set<pair<int, int>> &rotation_edges);

#endif