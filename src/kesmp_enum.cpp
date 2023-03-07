
#include "../include/kesmp_enum.h"

using namespace std;

void KESMP_Enum::init_method()
{
    init_topk();
}

// ENUM*
vector<ClosedSubset> KESMP_Enum::find_topk_S(int k)
{
    init_method();
    vector<int> D;
    deque<int> Rexposed;
    set<int> S;

    init_Rexposed(D,Rexposed);

    set<int> null_A;
    // the first result: male optimal stable marriage (S is empty)
    update_kesm(null_A, k);
   
    enumerate_stable_marriages(D, Rexposed, null_A, k);

    vector<ClosedSubset> best_kSs = TKSM_object.generate();

    return best_kSs;
}


void KESMP_Enum::init_Rexposed(vector<int> &D, deque<int> &Rexposed){
    int in_degree = 0;
    for(int i=0;i<smg.rotation_num;i++){
        in_degree = smg.parents[i].size();
        D.push_back(in_degree);
        if(in_degree==0){
            Rexposed.push_back(i);
        }
    }
}

// The Stable Marriage Problem: Structure and Algorithms (P125)
// Efficent Enumeration of All Stable Matchings 
void KESMP_Enum::enumerate_stable_marriages(vector<int> &D, deque<int> &Rexposed, set<int> &S, int k)
{
    if (antichain_counter > MAX_ANTICHAIN_NUM)
    {
        return;
    }
    if (!Rexposed.empty())
    {
        int r = Rexposed.front();
        //cout<<"r-: "<<r<<endl;
        Rexposed.pop_front();
        S.insert(r);
        update_kesm(S, k);
        for (int next_r : smg.children[r])
        {
            D[next_r] = D[next_r] - 1;
            if (D[next_r] == 0)
            {
                Rexposed.push_back(next_r);
            }
        }
        enumerate_stable_marriages(D, Rexposed, S, k);
        for (int next_r : smg.children[r])
        {
            D[next_r] = D[next_r] + 1;
            if (D[next_r] == 1)
            {
                Rexposed.pop_back();
            }
        }

        S.erase(r);
        //cout<<"r+: "<<r<<endl;
        enumerate_stable_marriages(D,Rexposed, S,k);
        Rexposed.push_front(r);
    }
}

void KESMP_Enum::package_results(const string &results_file, double runtime, vector<ClosedSubset> kesm_results)
{   
    map<string, int> counters = {
        {"antichain_counter", antichain_counter},
        //{"update_counter", update_counter},
    };
    map<string, string> lists = {};
    string save_path = results_file + "_m0";
    save_results(save_path, "enum", runtime, counters, lists, kesm_results);

}
