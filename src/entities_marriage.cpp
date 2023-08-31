#include "../include/entities_marriage.h"

using namespace std;

Marriage::Marriage()
{
    n = -1;
}

void Marriage::init(vector<int> &Rank_men_partner, vector<int> &Rank_women_partner)
{
    clear();

    n = Rank_men_partner.size();

    this->Rank_men_partner = Rank_men_partner;
    this->Rank_women_partner = Rank_women_partner;

    calculate_egalitarian_cost();
}

void Marriage::calculate_egalitarian_cost()
{
    int men_cost = 0;
    int women_cost = 0;

    for (int i = 0; i < n; i++)
    {
        men_cost = men_cost + Rank_men_partner[i];
        women_cost = women_cost + Rank_women_partner[i];
    }

    egalitarian_cost = men_cost + women_cost;
}



void Marriage::clear()
{
    n = -1;
    Rank_men_partner.clear();
    Rank_women_partner.clear();

    egalitarian_cost = -1;
   
}

bool Marriage::is_null()
{
    return (n < 0) ? true : false;
}

bool Marriage::is_equal(Marriage &M1)
{
    if (n == -1 || M1.is_null())
    {
        return false;
    }
    for (int i = 0; i < n; i++)
    {
        if (Rank_men_partner[i] != M1.Rank_men_partner[i])
            return false;
        if (Rank_women_partner[i] != M1.Rank_women_partner[i])
            return false;
    }
    return true;
}