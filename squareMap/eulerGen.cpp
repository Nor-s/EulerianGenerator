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
    int GameBoard::getBoard(int i, int j) const
    {
        return board[i][j];
    }
    void GameBoard::setBoard(int i, int j, int that)
    {
        if(i < 0 || j < 0 || i >= getHeight() || j >= getWide())
        {
            std::cout<<"BOUND ERROR\n";
            return;
        }
        board[i][j] = that;
    }
    int GameBoard::getHeight() const {return board.size();}
    int GameBoard::getWide() const {return board[0].size();}
    //initial setting UNDEFINED
    void GameBoard::initialSetting()
    {
       for(int i = 1; i < getHeight() - 1; i++)
            for(int j = 1; j < getWide() -1; j++)
                if(i % 2 == 1 || j % 2 == 1)
                    setBoard(i, j, UNDEFINED);
    }
    //print board
    void GameBoard::printBoard()
    {
        for(int i = 0; i < getHeight(); i++)
        {
            for(int j = 0; j < getWide(); j++)
            {
                char ch = '#';
                if(getBoard(i, j) == ROAD)
                    ch = ' ';
                if(getBoard(i, j) == UNDEFINED)
                    ch = '-';
                std::cout<<ch;
            }
            std::cout<<"\n";
        }
    }
    
    bool GameBoard::mergeIt(int i, int j)
    {
        bool canMerge = false;
        setBoard(i, j, ROAD);
        for(int dir = 0; dir < 4; dir++)
        {
            int nexti = i + dy[dir];
            int nextj = j + dx[dir];
            int ii = nexti + dy[dir];
            int jj = nextj + dx[dir];
            if(getBoard(nexti, nextj) != WALL)
            {
                setBoard(nexti, nextj, ROAD);
                setBoard(ii, jj, ROAD);
                edge.merge(i*j + j, nexti*nextj + nextj);
                edge.merge(i*j + j, ii*jj + jj);
                canMerge = true;
            }
        }
        return canMerge;
    }
    //merge tile if there are 2 WALL nearby
    void GameBoard::mergeAdjTwoEdge(int deepth)
    {
        int hLoHi[2] = {1+ deepth*2, getHeight()- 2 - deepth*2};
        int wLoHi[2] = {1+ deepth*2, getWide() - 2 - deepth*2};

        //left and right
        for(int i = hLoHi[0]; i <= hLoHi[1] ; i+=2)
        {
            for(int j = 0; j < 2; j++)
            {
                int count = 0;
                for(int dir = 0; dir < 4; dir++)
                   if(getBoard(i + dy[dir], wLoHi[j] + dx[dir]) != WALL)
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
                   if(getBoard(hLoHi[i] + dy[dir], j + dx[dir]) != WALL)
                    count++;
                if(count == 2)
                {
                    mergeIt(hLoHi[i], j);
                }
            }
        }
    }
    
    int GameBoard::searchAndPush(int deepth, std::queue<int>& q, int what, bool dontPush)
    {
        int hLoHi[2] = {1+ deepth*2, getHeight() - 2 - deepth*2};
        int wLoHi[2] = {1+ deepth*2, getWide() - 2 - deepth*2};
        int canFind = 0;

        //left and right
        for(int i = hLoHi[0]; i <= hLoHi[1] ; i+=2)
        {
            for(int j = 0; j < 2; j++)
            {
                if(getBoard(i, j) == what)
                {
                    canFind++;
                    if(!dontPush)
                        q.push(i*j + j);
                }
            }
        }
        //up and down
        for(int j = wLoHi[0] + 2; j <= wLoHi[1] - 2 ; j+=2)
        {
            for(int i = 0; i < 2; i++)
            {
                if(getBoard(i, j) == what)
                {
                    canFind++;
                    if(!dontPush)
                        q.push(i*j + j);
                }
            }
        }
        return canFind;
    }
    //
    bool GameBoard::currentDeepthSetting(int deepth, GameBoard& gameBoard)
    {
        GameBoard tmp = gameBoard;
        std::queue<int> q;
        tmp.mergeAdjTwoEdge(deepth);
        tmp.searchAndPush(deepth, q, ROAD);
        tmp.searchAndPush(deepth, q, UNDEFINED);
        /*
        while(!q.empty())
        {
            int here = q.front(); q.pop();
            int j = here%getWide();
            int i = here/getWide();
            if(tmp.getBoard(i, j)== WALL) continue;
            tmp.setRoad(i, j);
            tmp.setWall(i, j);
        }
        int nextRoadNum = tmp.searchAndPush(deepth + 1, q, ROAD, true);
        
        // -end-
        //if() return true   
        
        if(nextRoadNum == 0 || nextRoadNum %2 != 0)
            return false;
        
        while(!currentDeepthSetting(deepth + 1, tmp))
        {}
        gameBoard = tmp;
        return true;
        */
        gameBoard = tmp;
       return true;
    }
} 
