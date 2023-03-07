// #include "../include/utils.h"
#include "../include/query.h"
#include "../include/lib/Format.h"

// #include <iostream>
using namespace std;



int main(int argc, char *argv[])
{
    if (argc != 4)
        {
            printf("Wrong arguments");
            return 0;
        }

        string key = argv[1];

        int method = 0;
        sscanf(argv[2], "%d", &method);

        int k = 1;
        sscanf(argv[3], "%d", &k);


        string dir_format = "./instance/{}/{}";
            
        string instance_dir = util::Format(dir_format, key, key);

        Query x = Query(instance_dir,instance_dir,instance_dir+"_m",instance_dir);
  

        x.query(method,k);
  

}

// int main(int argc, char *argv[])
// {
//     // Dataset Key (foler name)
//     // [Input Files] 
//     // 1. preference lists    key
//     // 2. rotation graph (node)  key_r
//     // 3. rotation graph (edge)  key_e
//     // 4. male-optimal/female-optimal matching  key_m
//     // 5. instance info (n, type, rotation graph, hard instance params) 

//     // string key = "uniform";
//     string key = "n8";
    
//     string preference_file = "./instance/"+key;
//     string rotation_file = "./instance/"+ key;
//     string matching_file = "./instance/"+ key +"_m";
//     //string info_file = "./instance/"+key+"_i";
    
//     // Result File
//     string result_file = "./instance/"+key;

//     // Query Params
//     // 1. method
//     // [0]: ENUM(S)
//     // [1]: ENUM*
//     // [2]: ENUM*-L
//     // [3]: ENUM*-R
//     // [4]: ENUM*-LR
//     // [5]: ENUM*-h
//     // [6]: ENUM*-h-LR
//     // 2. k
//     int method = 0;
//     int k = 5;

    
//     Query x = Query(preference_file,rotation_file,matching_file,result_file);
  

//     x.query(method,k);

// }
