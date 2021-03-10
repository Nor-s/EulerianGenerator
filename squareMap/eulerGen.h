#ifndef EULERGEN_H_
#define EULERGEN_H_

#include <iostream>
#include <cstdlib>
#include <utility>
#include <string>
#include "disjoint.h"
#include "maxHeap.h"

namespace euler
{
    const int dy[] = { -1, 0, 1,  0 };                                                  // up right down left
    const int dx[] = { 0, 1, 0, -1 };                                                   // up right down left                       
    const int dir[] = { 3, 5, 7, 11 };                                                  // up right down left

    enum State {
        WALL = 0,
        UNDEFINED = 1,
        ROAD = 2,
        UP = 3, 
        RIGHT = 5,
        DOWN = 7,
        LEFT = 11,
        FOUR = 3 * 5 * 7 * 11,
        DEFINED = 13,
    };
    class GameBoard
    {
    private:
        nlib::maxHeap<double, int> pq;
        std::vector<std::vector<int> > board;
        disjoint::Disjoint edge;
    public:
        explicit GameBoard(int n = 0);                                                   // Setting GameBoard n x n
        void InitialBoard();                                                             // initillize board, edge
        void printBoard();                                                               // print board 

        int toNum(int i, int j);                                                         // i, j => i* width + j
        int reverseDir(int d);                                                           // up(0) <=> down(3) , right(1) <=> left(2)  

        std::vector<std::vector<int> > getAdj(int i, int j);                             // get Adj => {adj[WALL], adj[UNDEFINED], adj[ROAD]}
        int getEdgeState(int i, int j);                                                  // get edge i, j State == get tree leaf size
        double getPriority(int deepth, int i, int j);                                    // get priority => select priority based adj state
        int getBoard(int i, int j) const;                                                // get board[i][j]
        int getHeight() const;                                                           // get n*m .. n
        int getWide() const;                                                             // get n*m .. m
        int getNextI(int i, int d);                                                      // get i + dy[d]
        int getNextJ(int j, int d);                                                      // get j + dx[d]
        void setBoard(int i, int j, int that);                                           // set board[i][j] = that
        bool setRoad(int deepth, int i, int j, std::vector<int>& selected);              // set board[i][j] => road .. selected dir => road
        bool setRoad(int deepth, int i, int j);                                          // set board[i][j] => road .. selected dir => setRoad()
        void setWall(int deepth, int i, int j, std::vector<int>& toWall);                // set board[i + toWall][j + towall] => WALL

        void pushAllDeepth(int deepth);                                                  // push to pq(maxHeap) all vertex in deepth
        void pushToPq(int deepth, int i, int j);                                         // push and set priority
        void updatePriority(int deepth, int i, int j);                                   // priority(i, j) update: based adj state (wall 1, wall 1, wall 2, wall 3)
        void changePriority(int deepth, int i, int j, std::vector<int>& undefined);      // priority(i, j) 
        void addEdgeState(int i, int j, int x);                                          // edge.state(i, f) = edge.state(i, j) + x

        bool merge(int deepth, int i, int j, int d);                                     // merge i, j and next dir : d

        bool makeEven(int deepth, GameBoard& gameBoard);                                 // all edge to even in deepth
        void updateAllDeepth(int deepth);                                                // all vertex update priority_queue(max_Heap)
        bool check();                                                                    // if find edge have odd => false
    };
    int getRandomNumber(int min, int max);                                               // get x  (min<= x <= max)
}
#endif