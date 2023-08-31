#include "../include/sm_instance.h"

using namespace std;

SmInstance::SmInstance() {}

void SmInstance::init(vector<vector<int>> &Pref_men, vector<vector<int>> &Pref_women)
{
    n = Pref_men.size();
    this->Pref_men = Pref_men;
    this->Pref_women = Pref_women;
    invert_index();
}

void SmInstance::invert_index()
{
    for (int i = 0; i < n; i++)
    {
        vector<int> tmp(n, -1);
        Rank_men.push_back(tmp);
        Rank_women.push_back(tmp);
    }

    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n; j++)
        {
            Rank_men[i][Pref_men[i][j]] = j;
            Rank_women[i][Pref_women[i][j]] = j;
        }
    }
}

void SmInstance::gs_male_opt()
{
    vector<int> next_proposal(n, 0);
    vector<int> Rank_men_partner(n, -1);
    vector<int> Rank_women_partner(n, -1);

    int free_m, free_m_pos, w, w_pos, m, m_pos;

    stack<int> single_men;
    for (int m = 0; m < n; m++)
    {
        single_men.push(m);
    }

    while (!single_men.empty())
    {
        free_m = single_men.top();
        single_men.pop();

        w_pos = next_proposal[free_m];
        w = Pref_m(free_m, w_pos);
        m_pos = Rank_women_partner[w];
        free_m_pos = Rank_w(w, free_m);
        next_proposal[free_m]++;

        if (m_pos == -1)
        {
            Rank_men_partner[free_m] = w_pos;
            Rank_women_partner[w] = free_m_pos;
        }
        else
        {
            if (free_m_pos < m_pos)
            {
                m = Pref_w(w, m_pos);
                Rank_men_partner[free_m] = w_pos;
                Rank_women_partner[w] = free_m_pos;

                Rank_men_partner[m] = -1;
                single_men.push(m);
            }
            else
            {
                single_men.push(free_m);
            }
        }
    }

    male_opt.init(Rank_men_partner, Rank_women_partner);

    male_opt_matching = marriage_to_matching(male_opt);

    // for (int i = 0; i < n; i++)
    // {
    //     cout << male_opt.Rank_women_partner[i] << " ";
    // }
    // cout << endl;

    // for (int i = 0; i < n; i++)
    // {
    //     cout << get_w_by_pos(i, male_opt.Rank_men_partner[i]) << " ";
    // }
    // cout << endl;

    // for (int i = 0; i < n; i++)
    // {
    //     cout << Pref_w(i, male_opt.Rank_women_partner[i]) << " ";
    // }
    // cout << endl;
}

void SmInstance::gs_female_opt()
{
    vector<int> next_proposal(n, 0);
    vector<int> Rank_men_partner(n, -1);
    vector<int> Rank_women_partner(n, -1);

    int free_w, free_w_pos, w, w_pos, m, m_pos;

    stack<int> single_women;
    for (int w = 0; w < n; w++)
    {
        single_women.push(w);
    }

    while (!single_women.empty())
    {
        free_w = single_women.top();
        single_women.pop();

        m_pos = next_proposal[free_w];
        m = Pref_w(free_w, m_pos);
        w_pos = Rank_men_partner[m];
        free_w_pos = Rank_m(m, free_w);
        next_proposal[free_w]++;

        if (w_pos == -1)
        {
            Rank_women_partner[free_w] = m_pos;
            Rank_men_partner[m] = free_w_pos;
        }
        else
        {
            if (free_w_pos < w_pos)
            {
                w = Pref_m(m, w_pos);
                Rank_women_partner[free_w] = m_pos;
                Rank_men_partner[m] = free_w_pos;

                Rank_women_partner[w] = -1;
                single_women.push(w);
            }
            else
            {
                single_women.push(free_w);
            }
        }
    }

    female_opt.init(Rank_men_partner, Rank_women_partner);
    female_opt_matching = marriage_to_matching(female_opt);

    // for (int i = 0; i < n; i++)
    // {
    //     cout << female_opt.Rank_men_partner[i] << " ";
    // }
    // cout << endl;

    // for (int i = 0; i < n; i++)
    // {
    //     cout << female_opt.Rank_women_partner[i] << " ";
    // }
    // cout << endl;

    // for (int i = 0; i < n; i++)
    // {
    //     cout << get_w_by_pos(i, female_opt.Rank_men_partner[i]) << " ";
    // }
    // cout << endl;

    // for (int i = 0; i < n; i++)
    // {
    //     cout << Pref_w(i, female_opt.Rank_women_partner[i]) << " ";
    // }
    // cout << endl;
}

int SmInstance::Rank_w(int w, int m)
{
    return Rank_women[w][m];
}
int SmInstance::Rank_m(int m, int w)
{
    return Rank_men[m][w];
}
int SmInstance::Pref_m(int m, int i)
{
    return Pref_men[m][i];
}
int SmInstance::Pref_w(int w, int i)
{
    return Pref_women[w][i];
}

bool SmInstance::is_stable(vector<int> matching)
{
    int m1, m2, w1, w2 = 0;

    for (int i = 0; i < n; i++)
    {
        m1 = i;
        w1 = matching[i];
        for (int j = 0; j < n; j++)
        {
            m2 = j;
            w2 = matching[j];
            // if ((Rank_w(w1, m1) > Rank_w(w1, m2)) && (Rank_m(m2, w2) > Rank_m(m2, w1)))
            // {
            //     string pair_format = "(m{0},w{1}) (m{2},w{3}) m{4}<-->w{5}";
            //     string pair_string = util::Format(pair_format, m1, w1, m2, w2, m2, w1);
            //     cout << "blocking pair: " << pair_string << endl;
            //     return false;
            // }
            if ((Rank_m(m1, w1) > Rank_m(m1, w2)) && (Rank_w(w2, m2) > Rank_w(w2, m1)))
            {
                string pair_format = "(m{0},w{1}) (m{2},w{3}) m{4}<-->w{5}";
                string pair_string = util::Format(pair_format, m1, w1, m2, w2, m1, w2);
                cout << "blocking pair: " << pair_string << endl;
                return false;
            }
        }
    }
    return true;
}

int SmInstance::count_blocking_pairs(vector<int> matching)
{
    vector<int> matching2(n);
    int m, w = -1;
    for (int i = 0; i < n; i++)
    {
        m = i;
        w = matching[i];
        matching2[w] = m;
    }

    // Check for bpairs from the point of view of men
    int cnt = 0;
    int mIndex_of_i, mIndex_of_j;
    for (int i = 0; i < n; i++)
    {
        mIndex_of_i = Rank_m(i, matching[i]);
        for (int j = 0; j < n; j++)
        {
            mIndex_of_j = Rank_w(j, matching2[j]);
            if (j != matching[i] && Rank_w(j, i) < mIndex_of_j && Rank_m(i, j) < mIndex_of_i)
                cnt++;
        }
    }
    // Compare against the point of view of women
    int cnt2 = 0;
    int mIndex_of_i2, mIndex_of_j2;
    for (int i = 0; i < n; i++)
    {
        mIndex_of_i2 = Rank_w(i, matching2[i]);
        for (int j = 0; j < n; j++)
        {
            mIndex_of_j2 = Rank_m(j, matching[j]);
            if (j != matching2[i] && Rank_m(j, i) < mIndex_of_j2 && Rank_w(i, j) < mIndex_of_i2)
                cnt2++;
        }
    }
    if (cnt != cnt2)
    {
        cout << "Error when computing blocking pairs!" << endl;
    }
    cout << "blocking count: " << cnt << "," << cnt2 << endl;
    return cnt;
}

vector<int> SmInstance::marriage_to_matching(Marriage &M)
{
    vector<int> matching;

    for (int m = 0; m < n; m++)
    {
        matching.push_back(Pref_m(m, M.Rank_men_partner[m]));
    }
    return matching;
}
