// query

#include "../include/query.h"

using namespace std;

Query::Query(string preference_file, string rotation_file, string matching_file, string results_file)
{
    // load sm instance
    load_preferences(preference_file, male_prefers, female_prefers);
    load_matchings(matching_file, male_opt_matching, female_opt_matching);
    load_rotations(rotation_file + "_r", rotations);
    load_rotation_edges(rotation_file + "_e", rotation_edges);

    //init result file
    this->results_file = results_file;
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
    KESMP_Enum x = KESMP_Enum();
    x.init(male_prefers, female_prefers, male_opt_matching, rotations, rotation_edges);

    start_time = clock();
    results = x.find_topk_S(k);
    end_time = clock();

    runtime = (double)(end_time - start_time) / CLOCKS_PER_SEC;
    x.package_results(results_file + "_" + to_string(k), runtime, results);
}

void Query::enumstar(int k)
{
    KESMP_Enumstar x = KESMP_Enumstar();
    x.init(male_prefers, female_prefers, male_opt_matching, rotations, rotation_edges);

    start_time = clock();
    results = x.find_topk_S(k);
    end_time = clock();

    runtime = (double)(end_time - start_time) / CLOCKS_PER_SEC;
    x.package_results(results_file + "_" + to_string(k), runtime, results);
}

void Query::enumstar_layer(int k)
{
    KESMP_Layer x = KESMP_Layer();
    x.init(male_prefers, female_prefers, male_opt_matching, rotations, rotation_edges);

    start_time = clock();
    results = x.find_topk_S(k);
    end_time = clock();

    runtime = (double)(end_time - start_time) / CLOCKS_PER_SEC;
    x.package_results(results_file + "_" + to_string(k), runtime, results);
}

void Query::enumstar_rotation(int k)
{
    KESMP_Rotation x = KESMP_Rotation();
    x.init(male_prefers, female_prefers, male_opt_matching, rotations, rotation_edges);

    start_time = clock();
    results = x.find_topk_S(k);
    end_time = clock();

    runtime = (double)(end_time - start_time) / CLOCKS_PER_SEC;
    x.package_results(results_file + "_" + to_string(k), runtime, results);
}

void Query::enumstar_lr(int k)
{
    KESMP_LR x = KESMP_LR();
    x.init(male_prefers, female_prefers, male_opt_matching, rotations, rotation_edges);

    start_time = clock();
    results = x.find_topk_S(k);
    end_time = clock();

    runtime = (double)(end_time - start_time) / CLOCKS_PER_SEC;
    x.package_results(results_file + "_" + to_string(k), runtime, results);
}

void Query::enumstar_heuristic(int k)
{
    KESMP_Heuristic x = KESMP_Heuristic();
    x.init(male_prefers, female_prefers, male_opt_matching, rotations, rotation_edges);

    start_time = clock();
    results = x.find_topk_S(k);
    end_time = clock();

    runtime = (double)(end_time - start_time) / CLOCKS_PER_SEC;
    x.package_results(results_file + "_" + to_string(k), runtime, results);
}

void Query::enumstar_hlr(int k)
{
    KESMP_HLR x = KESMP_HLR();
    x.init(male_prefers, female_prefers, male_opt_matching, rotations, rotation_edges);

    start_time = clock();
    results = x.find_topk_S(k);
    end_time = clock();

    runtime = (double)(end_time - start_time) / CLOCKS_PER_SEC;
    x.package_results(results_file + "_" + to_string(k), runtime, results);
}
