#ifndef EULERGEN_H_
#define EULERGEN_H_

#include <iostream>
#include <cstdlib>
#include <queue>
#include <utility>
#include "disjoint.h"
namespace euler
{
    const int dy[] = {-1, 0, 1,  0};
    const int dx[] = { 0, 1, 0, -1};
    
    enum State
    {
        UNDEFINED, 
        WALL,
        ROAD
    };
    class GameBoard 
    {
        private:
            std::priority_queue<std::pair<int, int> > pq;
            std::vector<std::vector<int> > board;
            std::vector<std::vector<int> > visited;
            disjoint::Disjoint edge;
        public:
            //Setting GameBoard height x wide
            //initillize board, edge
            explicit GameBoard(int height = 0, int wide = 0);
            int toNum(int i, int j);
            int getBoard(int i, int j) const;
            void setBoard(int i, int j, int that); 
            int getHeight() const;
            int getWide() const;
            
            int getEdgeState(int i, int j);
            void setEdgeState(int i, int j, int x);
            void addEdgeState(int i, int j, int x);

            bool isRange(int i, int j, int deepth);
            //initial setting UNDEFINED
            void initialSetting();
            //print board 
            void printBoard();
            //merge i, j <==> ii , jj
            void merge(int deepth, int i, int j, int ii, int jj);
            bool merge(int deepth, int i, int j, int d);
            //merge tile if there are 2 WALL nearby
            void mergeTwoWall(int deepth);
            //set Road i, j => select direction up right down left
            bool setRoad(int deepth, int i, int j);
            //all deepth setting all tile have even edge
            //if can't set return false and reconstruct
            //if next deepth have not even road return false and reconstruct
            //    even road mean: out int => can Euler circuit
            bool currentDeepthSetting(int deepth, GameBoard& gameBoard);
            bool searchAndPush(int deepth, int what, int dontNeed = UNDEFINED);
            int pushToPq(int deepth, int i, int j);

    };
    int getRandomNumber(int min, int max);
} 
#endif