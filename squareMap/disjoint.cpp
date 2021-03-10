#include "disjoint.h"

namespace disjoint
{
    Disjoint::Disjoint(int n) : parent(n), rank(n, 1), state(n, 1), treeNum(n)
    {
        for (int i = 0; i < parent.size(); i++)
        {
            parent[i] = i;
        }
    }
    int Disjoint::find(int u)
    {
        if (parent[u] == u) return u;
        return parent[u] = find(parent[u]);
    }
    int Disjoint::getState(int u)
    {
        return state[find(u)];
    }
    void Disjoint::setState(int u, int x)
    {
        state[find(u)] = x;
    }
    void Disjoint::addState(int u, int x)
    {
        state[find(u)] += x;
    }
    void Disjoint::merge(int u, int v)
    {
        u = find(u); v = find(v);
        if (u == v)
        {
            return;
        }
        if (rank[u] > rank[v])
            std::swap(u, v);
        parent[u] = v;
        state[v] = state[u] + state[v];
        treeNum--;
        if (rank[u] == rank[v])
            rank[v]++;
    }
}