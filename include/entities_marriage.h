#ifndef ENTITIES_MARRIAGE_H_
#define ENTITIES_MARRIAGE_H_

//#include "../include/structs.h"

#include <iostream>
#include <vector>
#include <string>
using namespace std;

class Marriage
{
public:
	int n;

	vector<int> Rank_men_partner;   // Pm(w) list
	vector<int> Rank_women_partner; // Pw(m) list

	// equality cost(s)

	int egalitarian_cost;

	Marriage();
	void init(vector<int> &Rank_men_partner, vector<int> &Rank_women_partner);

	// boolean hasBetterCost(Marriage other);
	void calculate_egalitarian_cost();
	void clear();

	bool is_null();
	bool is_equal(Marriage &M1);
};

#endif