

#include "../include/topk_sm.h"

using namespace std;


TopK_StableMarriages::TopK_StableMarriages()
{
}

bool TopK_StableMarriages::update(ClosedSubset x, int k)
{
    if (candidate_k_results.size() < k)
    {
        candidate_k_results.push(x);
        return true;
    }

    ClosedSubset kx = candidate_k_results.top();
    if (x > kx)
    {
        candidate_k_results.pop();
        candidate_k_results.push(x);
        return true;
    }
    return false;
}

int TopK_StableMarriages::tail(int k)
{
    if (candidate_k_results.size() == k)
    {
        ClosedSubset kx = candidate_k_results.top();
        return kx.weight;
    }
    else{
        return INT_MIN;
    }

}

vector<ClosedSubset> TopK_StableMarriages::generate()
{
    vector<ClosedSubset> best_k_results;
    while (!candidate_k_results.empty())
    {
        ClosedSubset x = candidate_k_results.top();
        best_k_results.push_back(x);
        candidate_k_results.pop();
    }
    return best_k_results;
}

void TopK_StableMarriages::clear()
{
    while (!candidate_k_results.empty())
        candidate_k_results.pop();
}
