#include "../include/bound_rotation.h"

using namespace std;

//初始化topk结果
RotationBounds::RotationBounds(){}

void RotationBounds::insert(RotationBound rb)
{
    dormant_bounds.push(rb);
}

bool RotationBounds::empty()
{
    return dormant_bounds.empty();
}

RotationBound RotationBounds::pop()
{
    RotationBound rb = dormant_bounds.top();
    dormant_bounds.pop();
    return rb;
}

void RotationBounds::clear()
{
    while (!dormant_bounds.empty())
        dormant_bounds.pop();
}