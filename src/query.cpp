// query

#include "../include/query.h"

using namespace std;

Query::Query(string instance_dir)
{
    //init result file
    this->instance_dir = instance_dir;
}

// find top-k results
void Query::query(int method, int k)
{
    switch (method)
    {
    case 0:
        enumeration(k);
        break;
    case 1:
        enumstar(k);
        break;
    case 2:
        enumstar_layer(k);
        break;
    case 3:
        enumstar_rotation(k);
        break;
    case 4:
        enumstar_lr(k);
        break;
    case 5:
        enumstar_heuristic(k);
        break;
    case 6:
        enumstar_hlr(k);
        break;
    }
}

void Query::enumeration(int k)
{
    Enum x = Enum(instance_dir);

    start_time = clock();
    results = x.find_topk_closedsubsets(k);
    end_time = clock();

    runtime = (double)(end_time - start_time) / CLOCKS_PER_SEC;
    x.package_results(instance_dir + "_" + to_string(k), runtime, results);
}

void Query::enumstar(int k)
{
    EnumStar x = EnumStar(instance_dir);

    start_time = clock();
    results = x.find_topk_closedsubsets(k);
    end_time = clock();

    runtime = (double)(end_time - start_time) / CLOCKS_PER_SEC;
    x.package_results(instance_dir + "_" + to_string(k), runtime, results);
}

void Query::enumstar_layer(int k)
{
    EnumStarL x = EnumStarL(instance_dir);

    start_time = clock();
    results = x.find_topk_closedsubsets(k);
    end_time = clock();

    runtime = (double)(end_time - start_time) / CLOCKS_PER_SEC;
    x.package_results(instance_dir + "_" + to_string(k), runtime, results);
}

void Query::enumstar_rotation(int k)
{
    EnumStarR x = EnumStarR(instance_dir);
    

    start_time = clock();
    results = x.find_topk_closedsubsets(k);
    end_time = clock();

    runtime = (double)(end_time - start_time) / CLOCKS_PER_SEC;

    x.evaluate(results);
    x.package_results(instance_dir + "_" + to_string(k), runtime, results);
}

void Query::enumstar_lr(int k)
{
    EnumStarLR x = EnumStarLR(instance_dir);
    

    start_time = clock();
    results = x.find_topk_closedsubsets(k);
    end_time = clock();

    runtime = (double)(end_time - start_time) / CLOCKS_PER_SEC;
    x.package_results(instance_dir + "_" + to_string(k), runtime, results);
}

void Query::enumstar_heuristic(int k)
{
    EnumStarH x = EnumStarH(instance_dir);
    

    start_time = clock();
    results = x.find_topk_closedsubsets(k);
    end_time = clock();

    runtime = (double)(end_time - start_time) / CLOCKS_PER_SEC;
    x.package_results(instance_dir + "_" + to_string(k), runtime, results);
    save_poset(instance_dir, x.poset);
}

void Query::enumstar_hlr(int k)
{
    EnumStarHLR x = EnumStarHLR(instance_dir);

    start_time = clock();
    results = x.find_topk_closedsubsets(k);
    end_time = clock();

    runtime = (double)(end_time - start_time) / CLOCKS_PER_SEC;
    x.package_results(instance_dir + "_" + to_string(k), runtime, results);
}
