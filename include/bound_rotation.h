// Rotation Bound Class
// a priority queue to maintain the best rotation bound

#ifndef BOUND_ROTATION_H_
#define BOUND_ROTATION_H_

#include <iostream>
#include <vector>
#include <set>
#include <queue>
//#include <bits/stdc++.h>
#include "limits.h"
using namespace std;

struct RotationBound
{
	int rotation;
	int bound;
	RotationBound(int _rotation, int _bound) : rotation(_rotation), bound(_bound) {}

	bool operator<(const RotationBound &rb2) const
	{
		return (bound < rb2.bound);
	}
	bool operator>(const RotationBound &rb2) const
	{
		return (bound > rb2.bound);
	}
	bool operator==(const RotationBound &rb2) const
	{
		return (bound == rb2.bound);
	}
};


class RotationBounds
{
public:
	priority_queue<RotationBound, vector<RotationBound>, greater<RotationBound>> dormant_bounds;


	RotationBounds();

	void insert(RotationBound x);
	bool empty();
	RotationBound pop();
	void clear();
};

#endif