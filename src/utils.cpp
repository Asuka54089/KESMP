// Load Files

#include "../include/utils.h"
using namespace std;



void load_preferences(const string &dir, vector<vector<int>> &male_prefers,vector<vector<int>> &female_prefers){
    fstream preference_file;
    //cout<< fs::current_path()<<endl;
    preference_file.open(dir, ios::in);

    if (preference_file.is_open())
    {
        // cout<< "Open Prefernce File:" << dir<<endl;
        string line = "";
        int line_id = 1;
        int size = 0;

        while (getline(preference_file, line))
        {
            if (line_id==1){
                stringstream input(line);
                input >> size;
            }
            if (line_id >=2 && line_id <size+2)
            {
                istringstream ss(line);
                vector<int> tmp;
                string w;
                while(getline(ss, w, ' ')) {
                    tmp.push_back(stoi(w));
                }
                male_prefers.push_back(tmp);
            }
            if (line_id >= size+3 && line_id <=size*2+2){
                istringstream ss(line);
                vector<int> tmp;
                string m;
                while(getline(ss, m, ' ')) {
                    tmp.push_back(stoi(m));
                }
                female_prefers.push_back(tmp);
            }
            line_id++;
        }
    }

}


// void load_matchings(const string &dir, vector<int> &male_matching,vector<int> &female_matching)
// {
//     fstream matching_file;
//     // cout<< fs::current_path()<<endl;
//     matching_file.open(dir, ios::in);

//     if (matching_file.is_open())
//     {
//         string line = "";
//         int line_id = 1;
//         int male_score = 0;
//         int female_score = 0;
//         string tmp;

//         while (getline(matching_file, line))
//         {
            
//             if (line_id == 1)
//             {
//                 stringstream input(line);
//                 input >> male_score;
//             }
//             if (line_id == 2){
//                 istringstream ss(line);
                
//                 while(getline(ss, tmp, ' ')) {
//                     male_matching.push_back(stoi(tmp));
//                 }
//             }
//             if (line_id ==3)
//             {
//                 stringstream input(line);
//                 input >> female_score;
//             }
//             if (line_id ==4)
//             {
//                 istringstream ss(line);
//                 while(getline(ss, tmp, ' ')) {
//                     female_matching.push_back(stoi(tmp));
//                 }
//             }
//             line_id++;
//         }
//     }

// }


// void load_rotations(const string &rotation_dir, vector<Rotation> &rotations)
// {
//     fstream rotation_file;
//     //cout << file_path + "_r" << endl;
//     rotation_file.open(rotation_dir, ios::in);

//     if (rotation_file.is_open())
//     {
//         string line = "";
//         int line_id = 1;
//         int rotation_num = 0;
//         int instance_size = 0;

//         int rotation_size = 0;
//         int weight = 0, depth = 0;
//         int m = -1, w = -1;
//         set<pair<int, int>> tmp;
//         Rotation r;

//         int edge_num = 0;

//         while (getline(rotation_file, line))
//         {
//             //cout << "line:" << line_id << endl;
//             //size
//             if (line_id == 1)
//             {
//                 //cout << "line1" << endl;
//                 stringstream input(line);
//                 input >> instance_size;
//                 input >> rotation_num;
//             }
//             if (rotation_num == 0)
//                 break;
//             if (line_id > 1)
//             {
//                 stringstream input(line);
//                 //cout << line << endl;
//                 if (rotation_size == 0)
//                 {
//                     input >> rotation_size;
//                     input >> weight;
//                     input >> depth;
//                     r.depth = depth;
//                     r.weight = weight;
//                 }
//                 else
//                 {
//                     input >> m;
//                     input >> w;
//                     r.insert(m, w);
//                     rotation_size--;
//                     if (rotation_size == 0)
//                     {
//                         rotations.push_back(r);
//                         r.clear();
//                     }
//                 }
//             }
//             line_id++;
//         }
//     }
// }


// void load_rotation_edges(const string &rotation_dir, set<pair<int, int>> &rotation_edges)
// {
//     fstream rotation_edge_file;
//     rotation_edge_file.open(rotation_dir, ios::in);

//     if (rotation_edge_file.is_open())
//     {
//         string line = "";
//         int line_id = 1;
//         int r1 = -1, r2 = -1;
//         while (getline(rotation_edge_file, line))
//         {
//             if (line_id > 1)
//             {
//                 stringstream input(line);
//                 input >> r1;
//                 input >> r2;
//                 rotation_edges.insert(pair<int, int>(r1, r2));
//             }
//             line_id++;
//         }
//     }
// }



// /* Print */
// void print_adj(vector<set<int>> &adj,string title)
// {
//     cout<< title<<endl;
//     for (int i = 0; i < adj.size(); i++)
//     {
//         cout << i << endl;
//         for (auto itr = adj[i].begin(); itr != adj[i].end(); itr++)
//         {
//             cout << *itr << " ";
//         }
//         cout << endl;
//         cout << endl;
//     }
// }

// void print_matching(vector<int> &matching, string title)
// {
//     cout<< title<<endl;
//     cout << "n: " << matching.size() << endl;
//     // cout << "score: " << matching[matching.size()-1] << endl;
//     for (int i = 0; i < matching.size(); i++)
//     {
//         cout << i+1 << " - " <<  matching[i]+1 << endl;
//     }
// }

// void print_rotation_set(set<int> &rotation_set, string title)
// {
//     cout<< title<<endl;
//     cout<<"[";
//     for (auto itr = rotation_set.begin(); itr != rotation_set.end(); itr++)
//     {
//         cout << *itr << " ";
//     }
//     cout <<"]"<< endl;
// }


bool save_results(const string &result_file, string method, double runtime, map<string, int> &counts, map<string, string> &info, vector<ClosedSubset> &best_kSs, SmInstance &smi)
{

    ofstream output_file;
    output_file.open(result_file);

    //rotation(weight,depth,pairs)
    output_file << "method"
                << "\t" << method << endl;
    output_file << "time"
                << "\t" << runtime << endl;

    for (auto &t : counts)
    {
        output_file << t.first << "\t" << t.second << endl;
        //cout << t.first << t.second << endl;
    }

    for (auto &t : info)
    {
        output_file << t.first << "\t" << t.second << endl;
        //cout << t.first << t.second << endl;
    }
    

     for (int i = best_kSs.size() - 1; i >= 0; i--)
    {
        ClosedSubset Si = best_kSs[i];
        output_file << smi.male_opt.egalitarian_cost - Si.deltaS << endl;
        for (int j : Si.rotations)
        {
            output_file << j << " ";
        }
        output_file << endl;
    }
    output_file.close();

}

void save_poset(const string &instance_dir, RotationPoset &rotation_poset)
{
    ofstream output_file1;
    output_file1.open(instance_dir + "_r");

    output_file1 << rotation_poset.n << " " << rotation_poset.node_size << endl;
    // for (auto &r : rotation_poset.rotations)
    for (auto &r : rotation_poset.eliminated_rotations)
    {
        output_file1 << r.size << " " << r.weight << " " << r.depth << endl;
        for (int i = 0; i < r.size; i++)
        {
            output_file1 << r.men[i] << " " << r.women[i] << endl;
        }
    }

    ofstream output_file2;
    output_file2.open(instance_dir + "_e");
    output_file2 << rotation_poset.edge_size << endl;
    for (int r = 0; r < rotation_poset.node_size; r++)
    {
        for (auto rx : rotation_poset.children[r])
        {
            output_file2 << r << " " << rx << endl;
        }
    }
}