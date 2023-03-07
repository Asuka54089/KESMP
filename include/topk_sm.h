#ifndef TOPK_SM_H_
#define TOPK_SM_H_

#include <iostream>
#include <vector>
#include <set>
#include <queue>
//#include <bits/stdc++.h>
#include "limits.h"

#include "structs.h"
using namespace std;


// Top-k ESM Class
// a priority queue to maintain candidate top-k best closed subsets
class TopK_StableMarriages
{
public:

	// candidate top-k closed subsets
	priority_queue<ClosedSubset, vector<ClosedSubset>, greater<ClosedSubset>> candidate_kSs;

	TopK_StableMarriages();
	
	// update top-k results
	bool update(ClosedSubset x, int k);
	// return the k-th best result
	int tail(int k);
	void clear();

	// return final top-k results
	vector<ClosedSubset> generate();
};

#endif