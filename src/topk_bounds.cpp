

#include "../include/topk_bounds.h"

using namespace std;


TopK_Bounds::TopK_Bounds()
{
}

void TopK_Bounds::insert(SubsetBound x)
{
    invaild_bounds.push(x);
}

bool TopK_Bounds::empty()
{
    return invaild_bounds.empty();
}

SubsetBound TopK_Bounds::pop()
{
    SubsetBound x = invaild_bounds.top();
    invaild_bounds.pop();
    return x;
}

void TopK_Bounds::clear()
{
    while (!invaild_bounds.empty())
        invaild_bounds.pop();
}