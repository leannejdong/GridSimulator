//
// Created by leanne on 1/9/21.
//
#include "../include/parser.h"
#include <sstream>
#include <fstream>
#include <cassert>
#include <iostream>
using std::vector;
using std::pair;
using std::string;

vector<int> StrToVecInts(const string &graph_string)
{
    vector<int> vecInts;
    std::istringstream input_stream(graph_string);
    int number;
    while(input_stream >> number) {
        vecInts.push_back(number);
    }
    //printVec(vecInts);
    return vecInts;
}

vector<pair<int,int>> parseGraph(const string &graph_string)
{
    //std::istringstream split(graph_string);
    std::ifstream split(graph_string);
    string each;
    vector<pair<int, int>> V;

    while (std::getline(split, each)) {
        vector<int> vec1 = StrToVecInts(each);
        std::getline(split, each);
        vector<int> vec2 = StrToVecInts(each);

        for (int x : vec2) {
            V.push_back(std::make_pair(vec1[0] - 1, x - 1));
        }
    }

    return V;
}


