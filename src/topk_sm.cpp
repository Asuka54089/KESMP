#include "../include/topk_sm.h"

using namespace std;


TopK_StableMarriages::TopK_StableMarriages(){}


bool TopK_StableMarriages::update(ClosedSubset S, int k)
{
    // if the number of candidate best results is smaller than k 
    if (candidate_kSs.size() < k)
    {
        candidate_kSs.push(S);
        return true;
    }

    // if we already have k candidate best results
    // compare the top element
    ClosedSubset kS = candidate_kSs.top();
    if (S > kS)
    {
        candidate_kSs.pop();
        candidate_kSs.push(S);
        return true;
    }
    return false;
}


int TopK_StableMarriages::tail(int k)
{
    if (candidate_kSs.size() == k)
    {
        ClosedSubset kS = candidate_kSs.top();
        return kS.diff_S;
    }
    else{
        return INT_MIN;
    }

}


vector<ClosedSubset> TopK_StableMarriages::generate()
{
    // best top-k closed subsets
    vector<ClosedSubset> best_kSs;
    while (!candidate_kSs.empty())
    {
        ClosedSubset S = candidate_kSs.top();
        best_kSs.push_back(S);
        candidate_kSs.pop();
    }
    return best_kSs;
}

void TopK_StableMarriages::clear()
{
    while (!candidate_kSs.empty())
        candidate_kSs.pop();
}
