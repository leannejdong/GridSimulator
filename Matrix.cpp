//
// Created by leanne on 1/10/21.
//

#include "Matrix.h"
#include <iostream>

void addEdge(Matrix &adj, int u, int v)
{
    adj(u, v) += 1;
    adj(v, u) += 1;
}

void displayMatrix(Matrix const& adj)
{
    for(int i = 0; i < adj.size(); i++)
    {
        for(int j = 0; j < adj.size(); j++)
        {
            std::cout << adj(i,j) << "  ";
        }
        std::cout << std::endl;
    }
}
