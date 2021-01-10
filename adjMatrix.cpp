//
// Created by leanne on 1/10/21.
//

#include "adjMatrix.h"
#include <iostream>

void addEdge(adjMatrix &adj, int u, int v)
{
    adj(u, v) += 1;
    adj(v, u) += 1;
}

void displayMatrix(adjMatrix const& inc)
{
    for(int i = 0; i < inc.size(); i++)
    {
        for(int j = 0; j < inc.size(); j++)
        {
            std::cout << inc(i,j) << "  ";
        }
        std::cout << std::endl;
    }
}
