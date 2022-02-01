

#include "../include/data.h"

bool load_file_names(const string &path, vector<string> &file_names)
{

    if (!fs::exists(path))
    {
        return false;
    }

    for (const auto &entry : fs::directory_iterator(path))
        file_names.push_back(entry.path());


    if (file_names.size() == 0)
    {
        return false;
    }
    return true;
}

void load_rotations(const string &file_path, vector<int> &rotation_weights, vector<int> &rotation_depths, set<pair<int, int>> &rotation_edges)
{
    fstream rotation_file;
    //cout << file_path + "_r" << endl;
    rotation_file.open(file_path + "_r", ios::in);

    if (rotation_file.is_open())
    {
        string line = "";
        int line_id = 1;
        int rotation_num = 0;
        int instance_size = 0;

        int rotation_size = 0;
        int weight = 0, depth = 0;
        int m = -1, w = -1;
        set<pair<int, int>> tmp;

        int edge_num = 0;

        while (getline(rotation_file, line))
        {
            //cout << "line:" << line_id << endl;
            //size
            if (line_id == 1)
            {
                //cout << "line1" << endl;
                stringstream input(line);
                input >> instance_size;
                input >> rotation_num;
            }
            if (rotation_num == 0)
                break;
            if (line_id > 1)
            {
                stringstream input(line);
                //cout << line << endl;
                if (rotation_size == 0)
                {
                    input >> rotation_size;
                    input >> weight;
                    input >> depth;

                    rotation_weights.push_back(weight);
                    rotation_depths.push_back(depth);
                }
                else
                {
                    rotation_size--;
                }
            }
            line_id++;
        }
    }

    fstream rotation_edge_file;
    rotation_edge_file.open(file_path + "_e", ios::in);

    if (rotation_edge_file.is_open())
    {
        string line = "";
        int line_id = 1;
        int r1 = -1, r2 = -1;
        while (getline(rotation_edge_file, line))
        {
            if (line_id > 1)
            {
                stringstream input(line);
                input >> r1;
                input >> r2;
                rotation_edges.insert(pair<int, int>(r1, r2));
            }
            line_id++;
        }
    }
}