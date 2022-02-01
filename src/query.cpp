// 封装 topk query

#include "../include/query.h"

using namespace std;

Query::Query(string rotation_file, string results_file)
{
    load_rotations(rotation_file, rotation_weights, rotation_depths, rotation_edges);
    this->results_file = results_file;
}

void Query::query_enum(int k, bool verbose_case, bool verbose_result)
{
    KESMP_Enum x = KESMP_Enum(verbose_case, verbose_result);
    x.init_rotation_graph(rotation_edges, rotation_weights.size(), rotation_weights, rotation_depths);

    start_time = clock();
    results = x.find_posets(k);
    end_time = clock();

    runtime = (double)(end_time - start_time) / CLOCKS_PER_SEC;

    x.package_results(results_file, runtime, results);
}

void Query::query_enumstar(int k, bool verbose_case, bool verbose_result)
{
    KESMP_Enumstar x = KESMP_Enumstar(verbose_case, verbose_result);
    x.init_rotation_graph(rotation_edges, rotation_weights.size(), rotation_weights, rotation_depths);

 
    start_time = clock();
    results = x.find_posets(k);
    end_time = clock();

    runtime = (double)(end_time - start_time) / CLOCKS_PER_SEC;
    x.package_results(results_file, runtime, results);

    
}

void Query::query_layer(int k, bool verbose_case, bool verbose_result)
{

    KESMP_Layer x = KESMP_Layer(verbose_case, verbose_result);
    x.init_rotation_graph(rotation_edges, rotation_weights.size(), rotation_weights, rotation_depths);

    start_time = clock();
    results = x.find_posets(k);
    end_time = clock();

    runtime = (double)(end_time - start_time) / CLOCKS_PER_SEC;
    x.package_results(results_file, runtime, results);
}

void Query::query_subset(int k, bool verbose_case, bool verbose_result)
{

    KESMP_Subset x = KESMP_Subset(verbose_case, verbose_result);
    x.init_rotation_graph(rotation_edges, rotation_weights.size(), rotation_weights, rotation_depths);

    start_time = clock();
    results = x.find_posets(k);
    end_time = clock();

    runtime = (double)(end_time - start_time) / CLOCKS_PER_SEC;
    x.package_results(results_file, runtime, results);
}

void Query::query_hybrid(int k, bool verbose_case, bool verbose_result)
{
    KESMP_Hybrid x = KESMP_Hybrid(verbose_case, verbose_result);
    x.init_rotation_graph(rotation_edges, rotation_weights.size(), rotation_weights, rotation_depths);

    start_time = clock();
    results = x.find_posets(k);
    end_time = clock();

    runtime = (double)(end_time - start_time) / CLOCKS_PER_SEC;
    x.package_results(results_file, runtime, results);
}

void Query::query_hpr(int k, bool verbose_case, bool verbose_result)
{
    KESMP_Approx x = KESMP_Approx(verbose_case, verbose_result);
    x.init_rotation_graph(rotation_edges, rotation_weights.size(), rotation_weights, rotation_depths);

    start_time = clock();
    results = x.find_posets_hpr(k);
    end_time = clock();

    runtime = (double)(end_time - start_time) / CLOCKS_PER_SEC;
    x.package_results(results_file, runtime, results, 5);
}

void Query::query_hprs(int k, bool verbose_case, bool verbose_result)
{
    KESMP_Approx x = KESMP_Approx(verbose_case, verbose_result);
    x.init_rotation_graph(rotation_edges, rotation_weights.size(), rotation_weights, rotation_depths);

    start_time = clock();
    results = x.find_posets_hprs(k);
    end_time = clock();

    runtime = (double)(end_time - start_time) / CLOCKS_PER_SEC;
    x.package_results(results_file, runtime, results, 6);
}

void Query::query(int k, int method, string rotation_file, string result_file)
{
    switch (method)
    {
    case 0:
        query_enum(k, false, false);
        break;
    case 1:
        query_enumstar(k, false, false);
        break;
    case 2:
        query_layer(k, false, false);
        break;
    case 3:
        query_subset(k, false, false);
        break;
    case 4:
        query_hybrid(k, false, false);
        break;
    case 5:
        query_hpr(k, false, false);
        break;
    case 6:
        query_hprs(k, false, false);
        break;
    }
}