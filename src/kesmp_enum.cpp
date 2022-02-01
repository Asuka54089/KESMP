#include "../include/kesmp_enum.h"

using namespace std;

//1.brute
void KESMP_Enum::init_method()
{
    init_topk();

    overflow = false;


    parents.resize(rotation_num);
    for (int i = 0; i < rotation_num; i++)
    {
        for (auto itr2 = children[i].begin(); itr2 != children[i].end(); itr2++)
            parents[*itr2].insert(i);
    }
}

vector<ClosedSubset> KESMP_Enum::find_posets(int k)
{
    init_method();

    vector<ClosedSubset> kesm_results;
    int power_set_size = pow(2, rotation_num);

    if (power_set_size < 0)
    {
        //cout << "overflow" << endl;
        overflow = true;
        return kesm_results;
    }

    for (enum_counter = 0; enum_counter < power_set_size; enum_counter++)
    {
        Subset subset;
        for (int j = 0; j < rotation_num; j++)
        {
            /* Check if jth bit in the counter is set
            If set then print jth element from set */
            if (enum_counter & (1 << j))
                subset.insert(j);
        }

     
        bool flag_isclosed = false;
        if (subset.size() == 0)
        {
            flag_isclosed = true;
        }
        else
        {
            flag_isclosed = isClosed(subset);
        }

        if (verbose_case)
        {
            cout << "enum_counter " << enum_counter << endl;
            print_subset(subset);
            cout << "Closed: " << flag_isclosed << endl;
        }

  
        if (flag_isclosed)
        {
            update_kesm(subset, k);
        }
    }

    kesm_results = kesm_heap.generate();

    if (verbose_result)
    {
        cout << "method: enum" << endl;
        cout << "enum_counter " << enum_counter << endl;
        cout << "antichain_counter " << antichain_counter << endl;
        print_results(kesm_results);
    }

    return kesm_results;
}

bool KESMP_Enum::isClosed(Subset subset)
{

    bool isclosed[rotation_num];
    memset(isclosed, 0, sizeof(isclosed));
    for (int v : subset)
    {
        if (reverse_bfs(v, subset, isclosed) == false)
        {
            return false;
        }
    }
    return true;
}

bool KESMP_Enum::reverse_bfs(int v, set<int> subset, bool isclosed[])
{
    // Create a queue for BFS
    list<int> queue;

    queue.push_back(v);

    // 'i' will be used to get all adjacent
    // vertices of a vertex
    list<int>::iterator i;

    while (!queue.empty())
    {
        int x = queue.front();
        //cout << x << " ";
        queue.pop_front();

        for (auto i = parents[x].begin(); i != parents[x].end(); ++i)
        {
        
            if (isclosed[*i] == true)
            {
                continue;
            }
           
            if (subset.find(*i) != subset.end())
            {
                queue.push_back(*i);
            }
            else
            {
                return false;
            }
        }
    }
    isclosed[v] = true;
    return true;
}

void KESMP_Enum::package_results(const string &results_file, double runtime, vector<ClosedSubset> kesm_results)
{
    string save_path = results_file + "_m0";
    map<string, int> counters;
    if (overflow)
    {
        counters = {{"overflow", true}};
    }
    else
    {
        counters = {{"enum_counter", enum_counter}, {"anchain_counter", antichain_counter}, {"update_counter", update_counter}};
    }
    map<string, string> lists = {};
    //method 0: enum
    save_results(save_path, "enum", runtime, counters, lists, kesm_results);
}
