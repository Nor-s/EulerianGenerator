#ifndef EULERGEN_H_
#define EULERGEN_H_

#include <iostream>
#include <cstdlib>
#include <utility>
#include "disjoint.h"
#include "maxHeap.h"

namespace euler
{
    const int dy[] = { -1, 0, 1,  0 };
    const int dx[] = { 0, 1, 0, -1 };

    enum State
    {
        UNDEFINED,
        WALL,
        ROAD
    };
    class GameBoard
    {
    private:
        nlib::maxHeap<double, int> pq;
        std::vector<std::vector<int> > board;
        disjoint::Disjoint edge;
    public:
        //Setting GameBoard height x wide
        //initillize board, edge
        explicit GameBoard(int n = 0);
        //initial setting UNDEFINED
        void initialSetting();
        //print board 
        void printBoard();

        bool isRange(int i, int j, int deepth);
        int toNum(int i, int j);

        int getEdgeState(int i, int j);
        double getPriority(int deepth, int i, int j);
        int getBoard(int i, int j) const;
        int getHeight() const;
        int getWide() const;
        void setEdgeState(int i, int j, int x);
        void setBoard(int i, int j, int that);
        bool setRoad(int deepth, int i, int j);
        void setWall(int deepth, int i, int j, std::vector<int>& undefined);
        std::vector<std::vector<int> > getAdj(int i, int j);

        void pushAllDeepth(int deepth);
        void pushToPq(int deepth, int i, int j);
        void changePriority(int deepth, int i, int j, std::vector<int>& undefined);
        void updatePriority(int deepth, int i, int j);

        void addEdgeState(int i, int j, int x);

        //merge i, j <==> ii , jj
        void merge(int deepth, int i, int j, int ii, int jj);
        bool merge(int deepth, int i, int j, int d);
        //set Road i, j => select direction up right down left

        //all deepth setting all tile have even edge
        //if can't set return false and reconstruct
        //if next deepth have not even road return false and reconstruct
        //    even road mean: out int => can Euler circuit
        bool currentDeepthSetting(int deepth, GameBoard& gameBoard);
        void updateAllDeepth(int deepth);
        bool check(int deepth);
    };
    int getRandomNumber(int min, int max);
}
#endif