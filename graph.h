//
// Created by leanne on 1/11/21.
//

#ifndef GRIDSIMULATOR_GRAPH_H
#define GRIDSIMULATOR_GRAPH_H

#include <cassert>
#include <stack>
#include <algorithm>
#include <iostream>
class Graph {
    int r;
    std::vector<std::vector<int>> adjMatrix;
    std::vector<std::vector<int>> treeAdjMat;

    int countDifference(auto &treeAdjMat_i, auto &adjMatrix_i)
    {
        int n_differences = 0;
        for(int j=0; j<treeAdjMat_i.size(); j++)
            if(treeAdjMat_i[j]!=adjMatrix_i[j])
                ++n_differences;
        return n_differences;
    }

public:
    // Initialize the matrix to zero
    Graph(int r) : r(r), adjMatrix(r, std::vector<int>(r, 0)),
                   treeAdjMat(r, std::vector<int>(r)) {}
    void addEdge(int i, int j)
    {
        assert(i >= 0 && i < r && j >= 0 && j < r);
        ++adjMatrix[i][j] ;
        ++adjMatrix[j][i] ;
    }

    int size() const { return r; };

    template<class OutputIterator>
    void DFSCheckCycle(std::vector<std::vector<int>> &adjMat, size_t u, size_t par, std::vector<bool> &visited,
                       std::vector<int> &parents, size_t source, OutputIterator foundCycle)
    {
        if (visited[u]) {  // if u is visited
            if (u == source) {
                // mark u as seen by setting its parents
                parents[u] = par;
                while (true) {
                    // cycle found,
                    *foundCycle++ = u;
                    u = parents[u];
                    if (u == source) {
                        *foundCycle++ = u;  // foundCycle.push_back(u);
                        break;
                    }
                }
            }
            return;
        }
        visited[u] = true;
        parents[u] = par;
        for (size_t v = 0; v < adjMat.size(); ++v) {
            if (adjMat[u][v] == 1 && int(v) != parents[u]) {
                DFSCheckCycle(adjMat, v, u, visited, parents, source, foundCycle);
            }
        }
    }

    // Block1: Partition the set of n nodes w.r.t. chosen edges to form a set of disjoint trees with
    // adjacency matrix B. For if row i of adjMatrix, set treeAdjMatrix[i][j] = 1 if
    // adjMatrix[i][j] is the 1st superdiagonal element of ith row of adjMatrix
    template<typename OutputIterator>
    OutputIterator Gotlieb123(OutputIterator cycles)
    {
        const auto r = adjMatrix.size();
        // Initialize adjacency matrix for spanning tree
        treeAdjMat = std::vector<std::vector<int>> (r, std::vector<int>(r, 0));
        for (size_t i = 0; i < r; ++i) {
            for (size_t j = i; j < r; ++j) {
                if (adjMatrix[i][j] == 1) {
                    treeAdjMat[i][j] = 1;
                    treeAdjMat[j][i] = 1;
                    break;
                }
            }
        }

        // BLOCK 2: Find all connected components
        /* Example: C = [ [ 1 1 1 0 0 1 ],
         *                [ 0 0 0 1 0 0 ],
         *                [ 0 0 0 0 1 0 ]]
         * It is like walking through a maze. Similar to DFS,
         * we need to know which way to go.
         * When we hit a dead end, we take the first thing off
         * the stack and go back there and try */

        std::vector<std::vector<int>> connComponents;
        std::vector<bool> visited(r, false);
        for (size_t u = 0; u < r; ++u)
        {
            if (visited[u])
                continue;
            std::vector<int> component(r, 0);
            std::stack<int> s;
            s.push(u);
            while (!s.empty())
            {
                int v = s.top();
                visited[v] = true;
                component[v] = 1;
                s.pop();
                for (size_t w = 0; w < r; ++w)
                {
                    if (treeAdjMat[v][w] && !visited[w])
                    {
                        s.push(w); // put one branch and try onto stack
                    }
                }
            }
            connComponents.push_back(component);
        }
        // Now focus on finding cycle base
        /* Block 3: Here the connected components are amalgamated by
        adding appropriate edges to the adjacency matrix B (treeAdjMat)
        Example: edge(2, 7) and (3, 6) are added back to B
        */
        //NOTE:
        //  some utility functions for connected components - jeh
        auto connComponentOrder = [](auto &c){ return std::count(std::begin(c),std::end(c),true); };
        auto toComponent =
                [&](auto n)->auto &{ //NOTE: return auto &, since we get a copy by default - jeh
                    return *std::find_if(std::begin(connComponents),std::end(connComponents),[&](auto &c){ return c[n]; });
                };

        while(connComponents.size() > 1){
            //NOTE:
            //  sort the components in descending order (node count). descending so that we can easily remove the smallest with a pop_back.
            //  - jeh
            std::sort(std::begin(connComponents),std::end(connComponents),[&](auto &c0,auto &c1){ return connComponentOrder(c0) > connComponentOrder(c1); });
            auto &small = connComponents.back();
            auto &big =
                    [&]()->auto &{ //NOTE: find cross edge to bigger component, auto & again to get reference - jeh
                        for (int i = 0; i < r; ++i){
                            if(!small[i])
                                continue;
                            for (int j = 0; j < r; ++j){
                                if(adjMatrix[i][j] == 1 && !small[j]){
                                    treeAdjMat[i][j] = 1;struct Matrix {
                                        Matrix(int r) : storage_(r*r), size_(r) {}

                                        int& operator()(int i, int j)       noexcept {
                                            // std::cerr << "i= " << i << "and j= " << j << " and size_ =" << size_ << "\n";
                                            return storage_[i + size_*(j)]; }
                                        const int&  operator()(int i, int j) const noexcept { return storage_[i + size_*(j)]; }

                                        int size() const noexcept { return size_; }

                                    private:
                                        std::vector<int> storage_;
                                        int size_;
                                    };

                                    treeAdjMat[j][i] = 1;
                                    return toComponent(j);
                                }
                            }
                        }
                        __builtin_unreachable(); //NOTE: we should never reach this - jeh
                    }();

            //NOTE: move the nodes from small component to big component - jeh
            for (int i = 0; i < r; ++i)
                if(small[i])
                    big[i] = true;
            connComponents.pop_back();
        }
        // BLOCK 4
        /* Collect all edges eliminated from the original adjacency matrix to
        build the spanning tree matrix
        */
        std::vector<std::pair<int, int>> eliminatedEdges;
        for (size_t i = 0; i < r; ++i)
            for (size_t j = i; j < r; ++j)
                if (treeAdjMat[i][j] !=adjMatrix[i][j])
                    eliminatedEdges.emplace_back(i, j);

        // count how many sides been eliminated to get the spanning tree
        // FINAL: Iterate through each eliminated edge, try adding it into mat B(treeAdjMat)
        // The use DFS to check the cycle, the source node is the first node of the edge
        for (auto edge: eliminatedEdges)
        {
            visited = std::vector<bool>(r, false);
            std::vector<int> parents(r, -1);
            treeAdjMat[edge.first][edge.second] = treeAdjMat[edge.second][edge.first] = 1;
            DFSCheckCycle(treeAdjMat, edge.first, -1, visited, parents, edge.first, cycles);
            treeAdjMat[edge.first][edge.second] = treeAdjMat[edge.second][edge.first] = 0;
        }
        return cycles;
    }

    const std::vector<std::vector<int>> &getAdjMat() const
    {
        return adjMatrix;
    }

    auto Gotlieb4()
    {
        int m = 0;
        int k, c, nu, done; size_t i, j;

        for(i=0; i<r; ++i)
        {
            std::vector<int> &treeAdjMat_i = treeAdjMat[i];
            std::vector<int> &adjMatrix_i = adjMatrix[i];
            assert(r==treeAdjMat_i.size());
            m += countDifference(treeAdjMat_i,adjMatrix_i);
        }
        int &count = m;
        count /= 2;
        //count how many sides have to be eliminated to obtain the tree graph = number of independent links
        c = r*count + count + 1;
        std::vector<std::vector<int>> indm(r);
        for (i = 0; i<r; ++i)
        {
            indm[i].resize(c);
        }
        for (j = 0; j < c-r; j = j+r+1)
            for (size_t i = 0; i<r; ++i)
                indm.at(i).at(j) = -4;
        for (i = 0; i < r; ++i)
            indm.at(i).at(c-1)=-5;
        for (k = 1; k < c; k=k+r+1)
            for(i = 0; i < r; ++i)
                for(j = 0; j < r; ++j)
                    indm.at(i).at(j+k) = treeAdjMat.at(i).at(j);
        // add the sides at a time
        k = 1;
        for(i = 0; i < r; ++i)
            for(j = i+1; j<r; ++j)
                if(adjMatrix.at(i).at(j)==1 && treeAdjMat.at(i).at(j)==0)
                {
                    //std::cerr << " i = " << i << " and indm.size() = " << indm.size() << "\n";
                    std::cerr << " j+k = " << j+k << " and indm.at(i).size() = " << indm.at(i).size() << "\n";
                    indm.at(i).at(j+k)=1;
                    indm.at(j).at(i+k)=1;
                    k = k + r + 1;
                }
        /*I remove the segments that are outside the loop (see drawing)*/
        nu = 0; /*nu is the number one on a line*/
        done=0;
        for(k=1; k<c; k=k+r+1){
            while(done==0){
                done=1;
                for(i=0; i<r; i++){
                    for(j=0; j<r; j++) /*Count how many ones are on a line*/
                        if(indm.at(i).at(j+k)==1)
                            nu++;
                    if(nu==1)       /*if there is only one,  make it null*/
                        for(j=0; j<r; j++)    /*I am in the j of 1*/
                            if(indm.at(i).at(j+k)==1){
                                indm.at(i).at(j+k)=0;
                                indm.at(j).at(i+k)=0;
                                done=0;
                            }
                    nu=0;
                }
            }
            done=0;
        }

        return std::make_tuple(m, indm);
    }
    void printMat()
    {
        int i, j;
        for (i = 0; i < r; ++i )
        {
            for (j = 0; j < r; ++j)
            {
                std::cout << std::to_string(adjMatrix.at(i).at(j)) << " ";
            }
            std::cout << "\t";

            for (j = 0; j < r; ++j)
            {
                std::cout << std::to_string(treeAdjMat.at(i).at(j)) << " ";
            }
            std::cout << "\n";
        }
    }


};

// Requires a sequence of closed cycles.
template <class ForwardIterator, class OutputStream>
void print_cycles(ForwardIterator first, ForwardIterator last, OutputStream &os)
{
    using T = typename std::iterator_traits<ForwardIterator>::value_type;
    while (first != last)
    {
        auto const cycle_start = first++;
        first = ++find(first, last, *cycle_start);
        copy(cycle_start, first, std::ostream_iterator<T>(os, " "));
        os << "\n";
    }
}



#endif //GRIDSIMULATOR_GRAPH_H
