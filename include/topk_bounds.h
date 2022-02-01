#ifndef TOPK_BOUNDS_H_
#define TOPK_BOUNDS_H_

#include <iostream>
#include <vector>
#include <set>
#include <bits/stdc++.h>
#include "limits.h"
using namespace std;

struct SubsetBound
{
	int rotation;
	int bound;
	SubsetBound(int _rotation, int _bound) : rotation(_rotation), bound(_bound) {}

	bool operator<(const SubsetBound &c2) const
	{
		return (bound < c2.bound);
	}
	bool operator>(const SubsetBound &c2) const
	{
		return (bound > c2.bound);
	}
	bool operator==(const SubsetBound &c2) const
	{
		return (bound == c2.bound);
	}
};


class TopK_Bounds
{
public:
	priority_queue<SubsetBound, vector<SubsetBound>, greater<SubsetBound>> invaild_bounds;


	TopK_Bounds();

	void insert(SubsetBound x);
	bool empty();
	SubsetBound pop();
	void clear();
};

#endif
