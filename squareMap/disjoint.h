#ifndef DISJOINT_H_
#define DISJOINT_H_

#include <vector>

namespace disjoint
{
    struct Disjoint
    {
        int treeNum;
        std::vector<int> parent, rank, state;
        Disjoint(int n = 0);
        int find(int u);
        int getState(int u);
        void setState(int u, int x);
        void addState(int u, int x);
        void merge(int u, int v);
    };
}

#endif

