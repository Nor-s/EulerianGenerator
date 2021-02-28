#ifndef EULERGEN_H_
#define EULERGEN_H_

#include <iostream>
#include <queue>
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
            std::vector<std::vector<int> > board;
            disjoint::Disjoint edge;
        public:
            //Setting GameBoard height x wide
            //initillize board, edge
            explicit GameBoard(int height = 0, int wide = 0);
            //initial setting UNDEFINED
            void initialSetting();
            //print board 
            void printBoard();
            //merge i, j => up right down left merge
            void mergeIt(int i, int j);
            //merge tile if there are 2 WALL nearby
            void mergeAdjTwoEdge(int deepth);
            //merge tile if there are ROAD
            void mergeAdjRoad();
            //set Wall i, j
            void setWall(int i, int j);
            //set Road i, j => select direction up right down left
            void setRoad(int i, int j);
            //all deepth setting all tile have even edge
            //if can't set return false and reconstruct
            //if next deepth have not even road return false and reconstruct
            //    even road mean: out int => can Euler circuit
            bool currentDeepthSetting(int deepth);


    };
} 
#endif