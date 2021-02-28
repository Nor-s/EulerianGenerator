#ifndef DISJOINT_H_
#define DISJOINT_H_

#include <vector>

namespace disjoint
{
    struct Disjoint 
    {
        std::vector<int> parent, rank;
        Disjoint(int n = 0);
        int find(int u);
        void merge(int u, int v);
    };
}

#endif

