#include "../include/query.h"
#include "../include/data.h"

using namespace std;

// Arguments
// 1: k
// 2: methods [brute,enum,layer,subset,hybrid,h1,h2]
// 3: rotation file
// 4: result file

int main(int argc, char *argv[])
{
    if (argc != 5)
    {
        printf("Wrong arguments");
        return 0;
    }

    int k = 0;
    sscanf(argv[1], "%d", &k);

    int method = 0;
    sscanf(argv[2], "%d", &method);

    string rotation_file = argv[3];
    string result_file = argv[4];

    // string rotation_file = "../../datasets/uniform/rotation/500/u_500_2"; 
    // string result_file = "../../results/uniform/u_500_2"; 
    // int method = 6;
    // int k=3;

    Query x = Query(rotation_file, result_file);
    x.query(k,method,rotation_file,result_file); 
}
