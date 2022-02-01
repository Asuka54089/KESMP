#ifndef TOPK_SM_H_
#define TOPK_SM_H_

#include <iostream>
#include <vector>
#include <set>
#include <bits/stdc++.h>
#include "limits.h"
using namespace std;

typedef set<int> Subset;
struct ClosedSubset
{
	int weight;
	Subset rotations;
	ClosedSubset(int _weight, Subset _rotations) : weight(_weight), rotations(_rotations) {}

	bool operator<(const ClosedSubset &c2) const
	{
		return (weight < c2.weight);
	}
	bool operator>(const ClosedSubset &c2) const
	{
		return (weight > c2.weight);
	}
	bool operator==(const ClosedSubset &c2) const
	{
		return (weight == c2.weight);
	}
};


class TopK_StableMarriages
{
public:

	priority_queue<ClosedSubset, vector<ClosedSubset>, greater<ClosedSubset>> candidate_k_results;


	TopK_StableMarriages();

	bool update(ClosedSubset x, int k);
	int tail(int k);
	void clear();

	vector<ClosedSubset> generate();
};

#endif
