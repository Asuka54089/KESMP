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

        Query x = Query(instance_dir);
  

        x.query(method,k);
  

}

// int main(int argc, char *argv[])
// {


//     // string key = "uniform";
//     string key = "testn";
//     // string key = "n8";

//     string dir_format = "./instance/{}/{}";
            
//     string instance_dir = util::Format(dir_format, key, key);
    
    

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
//     int method = 1;
//     int k = 5;

    
   

//     Query x = Query(instance_dir);

//     x.query(method,k);


// }
