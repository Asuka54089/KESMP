#include "../include/entities_rotation.h"

using namespace std;

Rotation::Rotation()
{
    clear();
}

void Rotation::insert(int m, int w)
{
    men.push_back(m);
    women.push_back(w);
    size++;
}

void Rotation::reverse_pairs()
{
    reverse(men.begin(), men.end());
    reverse(women.begin(), women.end());
}

void Rotation::calculate_weight(SmInstance &smi){
    int score1 = 0;
    int score2 = 0;
    int m,w = -1;
    for(int i=0;i<size;i++){
        m = men[i];
        w = women[i];
        score1 = score1 + smi.Rank_w(w,m)+smi.Rank_m(m,w);
    }
    vector<int> next_women;
    get_next_women(next_women);

    for(int i=0;i<size;i++){
        m = men[i];
        w = next_women[i];
        score2 = score2 + smi.Rank_w(w,m)+smi.Rank_m(m,w);
    }
    this->weight = score1 - score2;
}

void Rotation::get_next_women(vector<int> &next_women)
{
    next_women.resize(women.size());
    for (int i = 0; i < women.size(); i++)
    {
        next_women[i] = women[get_next_index(i)];
    }
}

int Rotation::get_next_index(int index)
{
    if (index == size - 1)
        return 0;
    return index + 1;
}

int Rotation::get_prev_index(int index)
{
    if (index == 0)
        return size - 1;
    return index - 1;
}

void Rotation::clear()
{
    men.clear();
    women.clear();
    size = 0;
    depth = -1;
}

// void Rotation::print_R()
// {
//     cout << "Rotation" << endl;
//     cout << "size: " << this->size() << endl;
//     cout << "pairs" << endl;
//     for (int i = 0; i < this->size(); i++)
//     {
//         cout << Rm[i] << " " << Rw[i] << endl;
//     }
//     cout << "depth: " << depth << endl;
//     cout << "weight: " << weight << endl;
//     cout << endl;
// }
