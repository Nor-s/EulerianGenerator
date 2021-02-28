#include "eulerGen.h"


namespace euler
{
    //Setting GameBoard height x wide
    //initillize '#' = wall and roadSetting
    GameBoard::GameBoard(int height, int wide)
    {
        if(height%2 == 0) height++;
        if(wide%2 == 0) wide++;

        edge = disjoint::Disjoint(wide * height);
        
        board = std::vector<std::vector<int> >(height, std::vector<int>(wide, WALL));
        initialSetting();

    }
    //initial setting UNDEFINED
    void GameBoard::initialSetting()
    {
       for(int i = 1; i < board.size() - 1; i++)
            for(int j = 1; j < board[i].size() -1; j++)
                if(i % 2 == 1 || j % 2 == 1)
                    board[i][j] = UNDEFINED;
    }
    //print board
    void GameBoard::printBoard()
    {
        for(int i = 0; i < board.size(); i++)
        {
            for(int j = 0; j < board[i].size(); j++)
            {
                char ch = '#';
                if(board[i][j] == ROAD)
                    ch = ' ';
                if(board[i][j] == UNDEFINED)
                    ch = '-';
                std::cout<<ch;
            }
            std::cout<<"\n";
        }
    }
    
    void GameBoard::mergeIt(int i, int j)
    {
        board[i][j] = ROAD;
        for(int dir = 0; dir < 4; dir++)
        {
            int nexti = i + dy[dir];
            int nextj = j + dx[dir];
            int ii = nexti + dy[dir];
            int jj = nextj + dx[dir];
            if(board[nexti][nextj] != WALL)
            {
                board[nexti][nextj] = ROAD;
                board[ii][jj] = ROAD;
                edge.merge(i*j + j, nexti*nextj + nextj);
                edge.merge(i*j + j, ii*jj + jj);
            }
        }
    }
    //merge tile if there are 2 WALL nearby
    void GameBoard::mergeAdjTwoEdge(int deepth)
    {
        int hLoHi[2] = {1+ deepth*2, board.size() - 2 - deepth*2};
        int wLoHi[2] = {1+ deepth*2, board[0].size() - 2 - deepth*2};

        //left and right
        for(int i = hLoHi[0]; i <= hLoHi[1] ; i+=2)
        {
            for(int j = 0; j < 2; j++)
            {
                int count = 0;
                for(int dir = 0; dir < 4; dir++)
                   if(board[i + dy[dir]][wLoHi[j] + dx[dir]] != WALL)
                        count++;
                if(count == 2)
                {
                    mergeIt(i, wLoHi[j]);
                }
            }
        }
        //up and down
        for(int j = wLoHi[0]; j <= wLoHi[1] ; j+=2)
        {
            for(int i = 0; i < 2; i++)
            {
                int count = 0;
                for(int dir = 0; dir < 4; dir++)
                   if(board[hLoHi[i] + dy[dir]][j + dx[dir]] != WALL)
                    count++;
                if(count == 2)
                {
                    mergeIt(hLoHi[i], j);
                }
            }
        }
    }

    //
    bool GameBoard::currentDeepthSetting(int deepth)
    {
        mergeAdjTwoEdge(deepth);
        return true;
        std::queue<int> q;
        while(!q.empty())
        {

        }

      //  currentDeepthSetting(deepth + 1);
        return false;
    }
} 
