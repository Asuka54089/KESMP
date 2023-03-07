#include "../include/rotation.h"

using namespace std;

Rotation::Rotation()
{
    depth = -1;
    weight = 0;
}

int Rotation::size()
{
    return Rw.size();
}

void Rotation::insert(int m, int w)
{
    Rm.push_back(m);
    Rw.push_back(w);
}


void Rotation::get_next_females(vector<int> &next_Rw)
{
    next_Rw.resize(Rw.size());
    for (int i = 0; i < Rw.size() - 1; i++)
    {
        next_Rw[i] = Rw[i + 1];
    }
    next_Rw[Rw.size() - 1] = Rw[0];
}

void Rotation::clear()
{
    Rm.clear();
    Rw.clear();
    depth = -1;
    weight = 0;
}

void Rotation::print_R()
{
    cout << "Rotation" << endl;
    cout << "size: " << this->size() << endl;
    cout << "pairs" << endl;
    for (int i = 0; i < this->size(); i++)
    {
        cout << Rm[i] << " " << Rw[i] << endl;
    }
    cout << "depth: " << depth << endl;
    cout << "weight: " << weight << endl;
    cout << endl;
}

