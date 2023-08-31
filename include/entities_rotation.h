#ifndef ENTITIES_ROTATION_H_
#define ENTITIES_ROTATION_H_

#include <iostream>
#include <vector>
#include <algorithm>
#include "../include/sm_instance.h"
using namespace std;

class Rotation
{
public:
	vector<int> men;
	vector<int> women;

	int depth;
	int weight;
	int size;

	Rotation();

	void insert(int m, int w);
	void reverse_pairs();
	void calculate_weight(SmInstance &smi);

	int get_next_index(int index);
	int get_prev_index(int index);

	// get women list after rotation elimination
	void get_next_women(vector<int> &next_women);

	void clear();
};

#endif