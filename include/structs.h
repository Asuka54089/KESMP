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
	int deltaS;// Delta S
	set<int> rotations; 
	ClosedSubset(int _deltaS, set<int> _rotations) : deltaS(_deltaS), rotations(_rotations) {}

	bool operator<(const ClosedSubset &s2) const
	{
		return (deltaS < s2.deltaS);
	}
	bool operator>(const ClosedSubset &s2) const
	{
		return (deltaS > s2.deltaS);
	}
	bool operator==(const ClosedSubset &s2) const
	{
		return (deltaS == s2.deltaS);
	}
};

#endif