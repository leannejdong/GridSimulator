/*!
 * This class wasn't quite needed since we can just use Gotlieb's
 * graph class.
 */

#ifndef GRIDSIMULATOR_ADHMATRIX_H
#define GRIDSIMULATOR_ADHMATRIX_H
#include <vector>
#include <utility>
using Pair = std::vector<std::pair<int, int>>;
struct Matrix {
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

void addEdge(Matrix &adj, int u, int v);
void displayMatrix(Matrix const& inc);

#endif //GRIDSIMULATOR_ADHMATRIX_H
