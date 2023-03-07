#ifndef STRUCTS_H_
#define STRUCTS_H_

#include <iostream>
#include <vector>
#include <set>
#include <deque>

using namespace std;

// struct AntichainQueueItem
// {
//     set<int> antichain;
//     deque<int> candidates;
// };


struct ClosedSubset
{
	int diff_S;// Delta S
	set<int> rotations; 
	ClosedSubset(int _diff_S, set<int> _rotations) : diff_S(_diff_S), rotations(_rotations) {}

	bool operator<(const ClosedSubset &s2) const
	{
		return (diff_S < s2.diff_S);
	}
	bool operator>(const ClosedSubset &s2) const
	{
		return (diff_S > s2.diff_S);
	}
	bool operator==(const ClosedSubset &s2) const
	{
		return (diff_S == s2.diff_S);
	}
};

#endif