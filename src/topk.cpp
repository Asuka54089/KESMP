#include "../include/topk.h"

using namespace std;


TopK_StableMarriages::TopK_StableMarriages(){}


bool TopK_StableMarriages::update(ClosedSubset S, int k)
{
    // if the number of candidate best results is smaller than k 
    if (topk_heap.size() < k)
    {
        topk_heap.push(S);
        return true;
    }

    // if we already have k candidate best results
    // compare the top element
    ClosedSubset kS = topk_heap.top();
    if (S > kS)
    {
        topk_heap.pop();
        topk_heap.push(S);
        return true;
    }
    return false;
}


int TopK_StableMarriages::tail(int k)
{
    if (topk_heap.size() == k)
    {
        ClosedSubset kS = topk_heap.top();
        return kS.deltaS;
    }
    else{
        return INT_MIN;
    }

}


vector<ClosedSubset> TopK_StableMarriages::generate()
{
    // best top-k closed subsets
    vector<ClosedSubset> topk_closed_subsets;
    while (!topk_heap.empty())
    {
        ClosedSubset S = topk_heap.top();
        topk_closed_subsets.push_back(S);
        topk_heap.pop();
    }
    return topk_closed_subsets;
}

void TopK_StableMarriages::clear()
{
    while (!topk_heap.empty())
        topk_heap.pop();
}
