/*!
 * Implementation of existing operations.
 * More could be added...
 */
#ifndef GRIDSIMULATOR_TEST_H
#define GRIDSIMULATOR_TEST_H
#include "../include/parser.h"
#include "../include/Matrix.h"
#include "../include/graph.h"
#include <vector>
#include <cassert>
#include <ranges>
#include <algorithm>
#include <fstream>


using std::string;
using std::pair;
using std::vector;
using namespace std::ranges;
void test1()
{
    string graph_string =
            "1\n"     // node number
            "2 3\n"   // list of nodes connected to node 1
            "2\n"     // node number
            "3 4 5\n" // list of nodes connected to node 2
            "4\n"     // node number
            "5\n";    // list of nodes connected to node 4

    vector<pair<int,int>> result = parseGraph(graph_string);

    vector<pair<int,int>> expected_result = {
            {1,2},
            {1,3},
            {2,3},
            {2,4},
            {2,5},
            {4,5}
    };

    // assert(result == expected_result);
}

void test2()
{
    string graph_string =
            "1\n"     // node number
            "2 3 4\n"   // list of nodes connected to node 1
            "2\n"     // node number
            "3 4 5\n" // list of nodes connected to node 2
            "4\n"     // node number
            "5\n";    // list of nodes connected to node 4

    vector<pair<int,int>> result = parseGraph(graph_string);

    vector<pair<int,int>> expected_result = {
            {0,1},
            {0,2},
            {0,3},
            {1,2},
            {1,3},
            {1,4},
            {3,4}
    };

    assert(result == expected_result);
}


void test3()
{
//    string graph_string =
//            "1\n"     // node number
//            "2 3 4\n"   // list of nodes connected to node 1
//            "2\n"     // node number
//            "3 4 5\n" // list of nodes connected to node 2
//            "4\n"     // node number
//            "5\n"
//            "5\n"     // node number
//            "6 7\n"
//    ;

    vector<pair<int,int>> result = parseGraph("model.txt");
    std::cout << result << "\n";

//    vector<pair<int,int>> expected_result = {
//            {0,1},
//            {0,2},
//            {0,3},
//            {1,2},
//            {1,3},
//            {1,4},
//            {3,4},
//            {4,5},
//            {4,6}
//    };

    vector<pair<int,int>> expected_result = {
            {0,1},
            {5, 1},
            {5, 2},
            {5, 3},
            {6, 3},
            {6, 4},
            {7, 1},
            {7, 2},
            {7, 4}
    };

    assert(result == expected_result);

    vector<int> v;

    for (auto p : result) {
        v.push_back(p.first);
        v.push_back(p.second);
    }
    sort(v);
    auto last = std::unique(begin(v), end(v));
    v.erase(last, v.end());
    std::cerr << v.size() << "\n";

    Graph g(v.size());
    for (auto &p : result)
    {
        g.addEdge(p.first, p.second);
    }
    g.printMat();

    vector<int> cycles;
    g.Gotlieb123(std::back_inserter(cycles));
    std::ofstream of("cycles.data");
    std::cout << "Print Cycles " << "\n";
    print_cycles(std::begin(cycles), std::end(cycles), std::cout);
    print_cycles(std::begin(cycles), std::end(cycles), of);

//    Matrix adj(8);
//    std::cerr << adj.size() << "\n";
//    for (auto &v : result)
//    {
//        // std::cerr << v.first << " " << v.second << "\n";
//        addEdge(adj, v.first, v.second);
//    }
//    displayMatrix(adj);
//
//    Matrix m(3);
//    std::cerr << "Setting m(1,2) to 5\n";
//    m(1,2) = 5;
//    const Matrix &m2 = m;
//    //matrix m2 = m;
//    std::cerr << "m2(1,2) = " << m2(1,2) << "\n";
}
#endif //GRIDSIMULATOR_TEST_H


