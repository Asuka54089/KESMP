// maxflow工具类, 计算layer bound
// 只算maxflow, 跟stable marriage分开

#include "../include/maxflow.h"

using namespace std;

Maxflow::Maxflow(bool verbose)
{
    this->verbose = verbose;
}

void Maxflow::init_flow_graph(vector<set<int>> children, vector<int> rotation_weights, vector<int> rotation_depths)
{
    
    this->rotation_num = rotation_weights.size();


    flow_graph = new int *[rotation_num + 2];
    for (int i = 0; i < rotation_num + 2; i++)
    {
        flow_graph[i] = new int[rotation_num + 2];
        for (int j = 0; j < rotation_num + 2; j++)
        {
        
            flow_graph[i][j] = 0;
        }
    }
 
    for (int i = 0; i < rotation_num; i++)
    {
        for (int j : children[i])
        {
          
            flow_graph[i][j] = pINF;
           
            //flowG[j][i] = 0;
        }
    }

    // RG_size      s
    // RG_size+1    t
    for (int i = 0; i < rotation_num; i++)
    {
        if (rotation_weights[i] > 0)
        {
            // r+ ->t
            flow_graph[i][rotation_num + 1] = rotation_weights[i];
         
            //flowG[RG_size + 1][i] = 0;
        }
        if (rotation_weights[i] < 0)
        {
            // s ->r-
            flow_graph[rotation_num][i] = -rotation_weights[i];
        
            //flowG[i][RG_size] = 0;
        }
    }

    
    residual_graph = new int *[rotation_num + 2];
    for (int i = 0; i < rotation_num + 2; i++)
    {
        residual_graph[i] = new int[rotation_num + 2];
    }
    rotation_mask = new bool[rotation_num];

    this->rotation_depths = rotation_depths;
    this->rotation_weights = rotation_weights;
}

void Maxflow::shrink_flow_graph(int outlayer)
{
    //memset(mask, 0, sizeof(mask));
    for (int i = 0; i < rotation_num; i++)
    {
        rotation_mask[i] = false;
    }

    for (int i = 0; i < rotation_depths.size(); i++)
    {
        if (rotation_depths[i] <= outlayer)
        {
            rotation_mask[i] = true;
        }
        else
        {
            break;
        }
    }

    if (verbose)
    {
        cout << "subgraph layer:" << outlayer << endl;
        cout << "subgraph mask:";
        for (int i = 0; i < rotation_num; i++)
        {
            cout << rotation_mask[i] << " ";
        }
        cout << endl;
    }
}

void Maxflow::construct_residual_graph()
{

    int i, j;
   
    for (i = 0; i < rotation_num + 2; i++)
    {
        for (j = 0; j < rotation_num + 2; j++)
        {
            residual_graph[i][j] = 0;
        }
    }
    //i-j edge
    for (i = 0; i < rotation_num; i++)
    {
        for (j = 0; j < rotation_num; j++)
        {
            if (rotation_mask[i] == true && rotation_mask[j] == true)
            {
                residual_graph[i][j] = flow_graph[i][j];
            }
        }
    }
    //(s,r-) / (r+,t)
    for (i = 0; i < rotation_num; i++)
    {
        if (rotation_mask[i] == true)
        {
            residual_graph[rotation_num][i] = flow_graph[rotation_num][i];
            residual_graph[i][rotation_num + 1] = flow_graph[i][rotation_num + 1];
        }
    }

    if (verbose)
    {
        cout << "init residual graph" << endl;
        print_residual_graph();
    }
}

bool Maxflow::bfs(int s, int t, int *parent)
{
    // Create a visited array and mark all vertices as not
    // visited
    bool visited[rotation_num + 2];
    memset(visited, 0, sizeof(visited));

    // Create a queue, enqueue source vertex and mark source
    // vertex as visited
    queue<int> q;
    q.push(s);
    visited[s] = true;
    parent[s] = -1;

    // Standard BFS Loop
    while (!q.empty())
    {
        int u = q.front();
        q.pop();

        for (int v = 0; v < rotation_num + 2; v++)
        {
            if (visited[v] == false && residual_graph[u][v] > 0)
            {
                // If we find a connection to the sink node,
                // then there is no point in BFS anymore We
                // just have to set its parent and can return
                // true
                if (v == t)
                {
                    parent[v] = u;
                    return true;
                }
                q.push(v);
                parent[v] = u;
                visited[v] = true;
            }
        }
    }

    //print_path(parent, RG_size + 2);
    // We didn't reach sink in BFS starting from source, so
    // return false
    return false;
}

// Returns the maximum flow from s to t in the given graph
int Maxflow::EdmondsKarp(int outlayer)
{
    int u, v;

    // Create a residual graph and fill the residual graph
    // with given capacities in the original graph as
    // residual capacities in residual graph

    // Residual graph where residualG[i][j]
    // indicates residual capacity of edge
    // from i to j (if there is an edge. If rGraph[i][j] is 0, then there is not)

    shrink_flow_graph(outlayer);
    construct_residual_graph();

    int s = rotation_num;
    int t = rotation_num + 1;

    int parent[rotation_num + 2];
    // This array is filled by BFS and to
    // store path

    int max_flow = 0; // There is no flow initially

    // Augment the flow while tere is path from source to
    // sink

    while (bfs(s, t, parent))
    {
        // Find minimum residual capacity of the edges along
        // the path filled by BFS. Or we can say find the
        // maximum flow through the path found.
        int path_flow = pINF;
        for (v = t; v != s; v = parent[v])
        {
            u = parent[v];
            path_flow = min(path_flow, residual_graph[u][v]);
        }

        // update residual capacities of the edges and
        // reverse edges along the path
        for (v = t; v != s; v = parent[v])
        {
            u = parent[v];
            residual_graph[u][v] -= path_flow;
            residual_graph[v][u] += path_flow;
        }

        // Add path flow to overall flow
        max_flow += path_flow;
    }

    // Return the overall flow

    if (verbose)
    {
        cout << "no augmenting path" << endl;
        print_residual_graph();
    }

    return max_flow;
}

void Maxflow::dfs(int **graph, const int s, bool *visited)
{
    visited[s] = true;
    //cout << "visited " << s << endl;
    for (int i = 0; i < rotation_num + 2; i++)
        if (graph[s][i] && !visited[i])
        {
            //cout << rGraph[s][i] << "," << rGraph[i][s] << endl;
            //cout << s << "," << i << endl;
            dfs(graph, i, visited);
        }
}

set<int> Maxflow::min_cut()
{
    set<int> max_subset;

    for (int i = 0; i < rotation_num; i++)
    {
        if (rotation_weights[i] > 0 && rotation_mask[i] == true)
        {
            max_subset.insert(i);
        }
    }

    bool visited[rotation_num + 2];
    memset(visited, 0, sizeof(visited));

    dfs(residual_graph, rotation_num, visited);

    // Print all edges that are from a reachable vertex to
    // non-reachable vertex in the original graph
    for (int i = 0; i < rotation_num + 2; i++)
        for (int j = 0; j < rotation_num + 2; j++)
        {
            if (visited[i] && !visited[j] && flow_graph[i][j])
            {
                if (i == rotation_num && rotation_mask[j] == true)
                {
                    //cout << i << " - " << j << endl;
                    max_subset.insert(j);
                }
                if (j == rotation_num + 1 && rotation_mask[i] == true)
                {
                    //cout << i << " - " << j << endl;
                    max_subset.erase(i);
                }
            }
        }
    return max_subset;
}

void Maxflow::print_residual_graph()
{
    cout << "+++++++++++++++++++++++" << endl;
    for (int i = 0; i < rotation_num + 2; i++)
    {
        for (int j = 0; j < rotation_num + 2; j++)
        {
            if (residual_graph[i][j] > 0)
                cout << i << "\t" << j << "\t" << residual_graph[i][j] << endl;
        }
    }
    cout << endl;
}

void Maxflow::print_path(int *parent, int size)
{
    for (int i = size; i != size; i = parent[i])
    {
        cout << i << "<--" << parent[i] << "<--";
    }
    cout << endl;
}
