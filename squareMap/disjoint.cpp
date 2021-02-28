#include "disjoint.h"

namespace disjoint
{
    Disjoint::Disjoint(int n): parent(n), rank(n, 1)
    {
        for(int i = 0; i < parent.size();i++)
            parent[i] = i;
    }
    int Disjoint::find(int u)
    {
        if(parent[u] == u) return u;
        return parent[u] = find(parent[u]);
    }
    void Disjoint::merge(int u, int v)
    {
        u = parent[u]; v = parent[v];
        if(u == v) return;
        if(rank[u] > rank[v]) 
            std::swap(u, v);
        parent[u] = v;
        if(rank[u] == rank[v])
            rank[v]++;
    }
}