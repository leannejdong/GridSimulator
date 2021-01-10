//
// Created by leanne on 1/9/21.
//

#ifndef GRIDSIMULATOR_PARSER_H
#define GRIDSIMULATOR_PARSER_H
#include <ostream>
#include <vector>
#include <utility>
template <typename T>
std::ostream &operator<<(std::ostream &stream, const std::vector<T> &container)
{
    stream << "[ ";

    for (const T &item : container) {
        stream << item << " ";
    }

    stream << "]";
    return stream;
}

template <typename T>
std::ostream &operator<<(std::ostream &stream, const std::vector<std::pair<T, T>> &container)
{
    for (const std::pair<T, T> &item : container) {
        stream << "[" << item.first << " " << item.second << "]" << " ";
    }

    return stream;
}

std::vector<int> StrToVecInts(const std::string &graph_string);

std::vector<std::pair<int,int>> parseGraph(const std::string &graph_string);

//void test1();
//void test2();
//void test3();
#endif //GRIDSIMULATOR_PARSER_H
