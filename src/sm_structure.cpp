#include "../include/sm_structure.h"

using namespace std;

SmStructure::SmStructure() {}

vector<Rotation> SmStructure::find_rotations(SmInstance &smi)
{
    int m, m1;
    vector<Rotation> rotations;
    vector<int> final_proposal(smi.n);
    for (int m = 0; m < smi.n; m++)
    {
        final_proposal[m] = smi.female_opt.Rank_men_partner[m];
    }
    stack<int> stack;
    Marriage M = smi.male_opt;
    vector<bool> in_stack(smi.n, false);

    int i = 0;
    while (i < smi.n)
    {
        if (stack.empty())
        {
            while (M.Rank_men_partner[i] == final_proposal[i])
            {
                i = i + 1;
                if (i == smi.n)
                    break;
            }
            if (i < smi.n)
            {
                stack.push(i);
                in_stack[i] = true;
            }
        }
        else
        {
            m = stack.top();
            m = next(M, m, smi);
            while (!in_stack[m])
            {
                stack.push(m);
                in_stack[m] = true;
                // cout<<"find next m" << m <<endl;
                m = next(M, m, smi);
                // cout<<"new m" << m <<endl;
                // if(m==smi.n){
                //     cout<<"test"<<endl;
                // }
            }
            m1 = stack.top();
            stack.pop();
            in_stack[m1] = false;
            Rotation tmp_r;
            tmp_r.insert(m1, smi.Pref_m(m1, M.Rank_men_partner[m1]));
            while (m != m1)
            {
                m1 = stack.top();
                stack.pop();
                in_stack[m1] = false;
                tmp_r.insert(m1, smi.Pref_m(m1, M.Rank_men_partner[m1]));
            }

            tmp_r.reverse_pairs();
            tmp_r.calculate_weight(smi);
            rotations.push_back(tmp_r);
            // cout << "eliminate r" << rotations.size() << endl;
            eliminate(M, tmp_r, smi);

            //Print next M
            // for (int i = 0; i < smi.n; i++)
            // {
            //     cout << "(m" << i << ","
            //          << "w" << smi.Pref_m(i, M.Rank_men_partner[i]) << "), ";
            // }
            // cout << endl;
        }
    }

    return rotations;
}

int SmStructure::next(Marriage &M, int m, SmInstance &smi)
{
    int w;
    int kappa = M.Rank_men_partner[m];
    kappa++;
    while (true)
    {
        if (kappa == smi.n)
            return smi.n;
        w = smi.Pref_m(m, kappa);

        if (smi.Rank_w(w, m) < M.Rank_women_partner[w])
            break;
        kappa++;
    }
    // cout << "[m" << m << ",w" << w << "]" << endl;
    return smi.Pref_w(w, M.Rank_women_partner[w]);
}

void SmStructure::eliminate(Marriage &M, Rotation &r, SmInstance &smi)
{
    int m, next_w;

    vector<int> next_women;
    r.get_next_women(next_women);

    for (int i = 0; i < r.size; i++)
    {
        m = r.men[i];
        next_w = next_women[i];

        M.Rank_men_partner[m] = smi.Rank_m(m, next_w);
        M.Rank_women_partner[next_w] = smi.Rank_w(next_w, m);
    }
}

vector<pair<int, int>> SmStructure::construct_graph(vector<Rotation> &rotations, SmInstance &smi)
{
    vector<pair<int, int>> edges;
    link_rotations(edges, rotations, smi);
    sparsify_graph(edges, rotations);
    return edges;
}

void SmStructure::link_rotations(vector<pair<int, int>> &edges, vector<Rotation> &rotations, SmInstance &smi)
{

    int m1, w1, w1_pos, m2;
    int m, index, latest_type1_label;

    // Initialize
    vector<vector<int>> labels;
    vector<vector<int>> label_types;
    for (int i = 0; i < smi.n; i++)
    {
        vector<int> row(smi.n, -1);
        labels.push_back(row);
        label_types.push_back(row);
    }

    // Labels

    // for (Rotation r : rotations)
    for(int r_id = 0;r_id<rotations.size();r_id++)
    {
        Rotation r = rotations[r_id];
        for (int i = 0; i < r.size; i++)
        {
            m1 = r.men[i];
            w1 = r.women[i];
            w1_pos = smi.Rank_m(m1, w1);
            labels[m1][w1_pos] = r_id;
            label_types[m1][w1_pos] = 1;

            // m2 = r.men[r.get_next_index(i)];
            m2 = r.men[r.get_prev_index(i)];

            for (int j = smi.Rank_w(w1, m1) - 1; j > smi.Rank_w(w1, m2); j--)
            {
                m = smi.Pref_w(w1, j);
                index = smi.Rank_m(m, w1);
                labels[m][index] = r_id;
                label_types[m][index] = 2;
            }
        }
        // r_id++;
    }

    // Edges
    for (int i = 0; i < smi.n; i++)
    {
        latest_type1_label = -1;

        for (int j = smi.male_opt.Rank_men_partner[i]; j <= smi.female_opt.Rank_men_partner[i]; j++)
        {
            if (label_types[i][j] == 1)
            {
                if (latest_type1_label != -1)
                    edges.push_back(pair<int, int>(latest_type1_label, labels[i][j]));
                latest_type1_label = labels[i][j];
            }
            else if (label_types[i][j] == 2)
                edges.push_back(pair<int, int>(labels[i][j], latest_type1_label));
        }
    }

    // Sort the values in vector
    sort(edges.begin(), edges.end());
    // Remove duplicate values from vector
    edges.erase(unique(edges.begin(), edges.end()), edges.end());
}

void SmStructure::sparsify_graph(vector<pair<int, int>> &edges, vector<Rotation> &rotations)
{

    vector<vector<int>> graph;
    for (int i = 0; i < rotations.size(); i++)
    {
        vector<int> row(rotations.size(), 0);
        graph.push_back(row);
    }

    for (auto edge : edges)
    {
        if (edge.first != edge.second)
        {
            graph[edge.first][edge.second] = 1;
        }
    }

    check_transitivity(rotations.size(), graph);

    vector<pair<int, int>> reduced_edges;
    int rs, rt;
    for (int i = 0; i < edges.size(); i++)
    {
        rs = edges[i].first;
        rt = edges[i].second;
        if (graph[rs][rt] > 1 || rs==rt)
        {
            //removed edge
        }
        else
        {
            reduced_edges.push_back(pair<int, int>(rs, rt));
        }
    }

    edges = reduced_edges;
}

void SmStructure::check_transitivity(int node_size, vector<vector<int>> &graph)
{
    // create a set to store all visited vertices
    vector<bool> visited(node_size, false);
    //call the recursive helper function to print DFS traversal starting from all
    //vertices one by one
    for (int i = 0; i < node_size; i++)
    {
        if (visited[i] == false)
        {
            // cout << "dfs: " << i << endl;
            dfs_util(i, visited, graph);
        }
    }
}

void SmStructure::dfs_util(int u, vector<bool> &visited, vector<vector<int>> &graph)
{
    //DFS
    //Mark the current node as visited and print it
    visited[u] = true;

    //recur for all the vertices adjacent to this vertex
    bool vflag, jflag = false;
    // for (int v = 0; v < graph.size(); v++)
    for (int v = graph.size() - 1; v >= 0; v--)
    {

        vflag = graph[u][v];
        if (vflag == 1 && v != u)
        {
            if (visited[v] == false)
            {
                // cout << "dfs= " << v << endl;
                dfs_util(v, visited, graph);
            }
            for (int j = 0; j < graph.size(); j++)
            {
                graph[u][j] = graph[u][j] + graph[v][j];
            }
            // cout << "add " << v << " to " << u << endl;
        }
    }
}

void SmStructure::print_graph(vector<vector<int>> &graph)
{
    for (int i = 0; i < graph.size(); i++)
    {
        cout << i << ":" << endl;
        for (int j = 0; j < graph.size(); j++)
        {
            cout << j << "," << graph[i][j];
        }
        cout << endl;
    }
}