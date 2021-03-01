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
    int GameBoard::toNum(int i, int j)
    {
        return i*getWide() + j;
    }
    int GameBoard::getBoard(int i, int j) const
    {
        return board[i][j];
    }
    void GameBoard::setBoard(int i, int j, int that)
    {
        if(i < 0 || j < 0 || i >= getHeight() || j >= getWide())
        {
            std::cout << "i " << i << " j: " << j << "\n";
            std::cout<<"BOUND ERROR\n";
            return;
        }
        board[i][j] = that;
    }
    bool GameBoard::isRange(int i, int j, int deepth)
    {
        int hLoHi[2] = { 1 + deepth * 2, getHeight() - 2 - deepth * 2 };
        int wLoHi[2] = { 1 + deepth * 2, getWide() - 2 - deepth * 2 };
        return (hLoHi[0] == i || i == hLoHi[1]) && (wLoHi[0] <= j && j <= wLoHi[1])
            || (hLoHi[0] <= i && i <= hLoHi[1]) && (wLoHi[0] == j || j == wLoHi[1]);
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
                    ch = '.';
                std::cout<<ch<<" ";
            }
            std::cout<<"\n";
        }
    }
    void GameBoard::pushToPq(int deepth, int i, int j)
    {

        if (!isRange(i, j, deepth))
        {
         //   std::cout << "cant push "<< i<< " " << j <<"\n"; 
            return;
        }
        int priority[3] = { 0, 0, 0 };
        for (int d = 0; d < 4; d++)
            priority[getBoard(i + dy[d], j + dx[d])]++;

        int thisPriority = (priority[1] == 2) ? 10 : 
                                                ((priority[0] + priority[2] == 3) ? (priority[0] > priority[2]? 5: 8) :
                                                                                    ((priority[0] + priority[2] == 4)? (priority[0] > priority[2] ? 3:7) : 2));
        if (visited[i][j] >= thisPriority)
        {
            return;
        }
        visited[i][j] = thisPriority;
        if (priority[0] > 0)
            pq.push(std::make_pair(thisPriority, toNum(i, j)));
    }

    void GameBoard::merge(int deepth, int i, int j, int ii, int jj)
    {
        setBoard(ii, jj, ROAD);
        edge.merge(toNum(i, j), toNum(ii, jj));
    }
    bool GameBoard::merge(int deepth, int i, int j, int d)
    {
        pushToPq(deepth, i + dy[d] * 2, j + dx[d] * 2);
        merge(deepth, i, j, i + dy[d], j + dx[d]);
        merge(deepth, i, j, i + dy[d]*2, j + dx[d]*2);
        setBoard(i, j, ROAD); 
        return true;
    }

    //merge tile if there are 2 WALL nearby
    //mergeAdjEdge(deepth, 2, road, undetected)
    void GameBoard::mergeTwoWall(int deepth)
    {
        int hLoHi[2] = {1+ deepth*2, getHeight()- 2 - deepth*2};
        int wLoHi[2] = {1+ deepth*2, getWide() - 2 - deepth*2};
        std::vector<int> dir;
        std::vector<int> walldir;
        //left and right
        for(int i = hLoHi[0]; i <= hLoHi[1] ; i+=2)
        {
            for(int j = 0; j < 2; j++)
            {
                for (int d = 0; d < 4; d++)
                {
                    if (getBoard(i + dy[d], wLoHi[j] + dx[d]) != WALL)
                        dir.push_back(d);
                }
                if (dir.size() == 2)
                    for (int d = 0; d < dir.size(); d++)
                        merge(deepth, i, wLoHi[j], dir[d]);
                dir.clear();
                /*
                for(int d = 0; d < 4; d++)
                {
                    if(getBoard(i + dy[d], wLoHi[j] + dx[d]) == UNDEFINED)
                        dir.push_back(d);
                    if (getBoard(i + dy[d], wLoHi[j] + dx[d]) == WALL)
                        walldir.push_back(d);
                }
                if(dir.size() == 2 && walldir.size() == 2)
                    for(int d = 0; d < dir.size(); d++)
                         merge(deepth, i, wLoHi[j], dir[d]);
                dir.clear();
                walldir.clear();
                */
            }
        }
        //up and down
        for(int j = wLoHi[0] + 2; j <= wLoHi[1] - 2; j+=2)
        {
            for(int i = 0; i < 2; i++)
            {
                for (int d = 0; d < 4; d++)
                {
                    if (getBoard(hLoHi[i] + dy[d], j + dx[d]) != WALL)
                        dir.push_back(d);
                }
                if (dir.size() == 2)
                    for (int d = 0; d < dir.size(); d++)
                        merge(deepth, hLoHi[i], j, dir[d]);
                dir.clear();
      
                /*
                for (int d = 0; d < 4; d++)
                {
                    if (getBoard(hLoHi[i] + dy[d], j + dx[d]) == UNDEFINED)
                        dir.push_back(d);
                    if (getBoard(hLoHi[i] + dy[d], j + dx[d]) == WALL)
                        walldir.push_back(d);
                }
                if (dir.size() == 2 && walldir.size() == 2)
                    for (int d = 0; d < dir.size(); d++)
                        merge(deepth, hLoHi[i], j, dir[d]);
                dir.clear();
                walldir.clear();
                */
            }
        }
    }

    bool GameBoard::currentDeepthSetting(int deepth, GameBoard& gameBoard)
    {
        if(deepth == 3) return true;
        GameBoard tmp = gameBoard;
        tmp.visited = std::vector<std::vector<int> >(getHeight(), std::vector<int>(getWide(), -1));
        tmp.pq = std::priority_queue<std::pair<int, int> >();
        tmp.mergeTwoWall(deepth);
        tmp.searchAndPush(deepth, ROAD);
        std::cout << "start "<<tmp.pq.size()<<"\n";
        while (!tmp.pq.empty()) //|| tmp.searchAndPush(deepth, ROAD, UNDEFINED))
        {
            int here = tmp.pq.top().second; tmp.pq.pop();
            int j = here % getWide();
            int i = here / getWide();
            std::cout << " i j " << i << " " << j << "\n";
            if (tmp.getBoard(i, j) == WALL) continue;
            if (!tmp.setRoad(deepth, i, j))
            {
                std::cout << "false\n";
                return false;
            }
            tmp.mergeTwoWall(deepth);

        }
        tmp.printBoard();
        std::cout << "\n";
        // -end-
        //if() return true   
        // if one of set cant out next deepth return false;
        while(!currentDeepthSetting(deepth + 1, tmp))
        {}
        gameBoard = tmp;
       return true;
    }

bool GameBoard::searchAndPush(int deepth, int what, int dontNeed)
{
    int hLoHi[2] = {1+ deepth*2, getHeight() - 2 - deepth*2};
    int wLoHi[2] = {1+ deepth*2, getWide() - 2 - deepth*2};
    bool findDontNeed = false;

    //left and right
    for(int i = hLoHi[0]; i <= hLoHi[1] ; i++)
    {
        for(int j = 0; j < 2; j++)
        {
            if(i%2 == 1 &&getBoard(i, wLoHi[j]) == what)
                pushToPq(deepth, i, wLoHi[j]);
            if(getBoard(i, wLoHi[j]) == dontNeed)
                findDontNeed = true;
        }
    }
    //up and down
    for(int j = wLoHi[0] + 1; j <= wLoHi[1] - 1 ; j++)
    {
        for(int i = 0; i < 2; i++)
        {
            if(j%2 == 1 && getBoard(hLoHi[i], j) == what)
                pushToPq(deepth, hLoHi[i], j);

            if(getBoard(hLoHi[i], j) == dontNeed)
                findDontNeed = true;
        }
    }

    return findDontNeed;
}

int getRandomNumber(int min, int max)
{
    static const double fraction = 1.0 / (RAND_MAX + 1.0); // static used for efficiency, so we only calculate this value once
     // evenly distribute the random number across our range 
    return min + static_cast<int>((max - min + 1) * (std::rand() * fraction));
}
     bool GameBoard::setRoad(int deepth, int i , int j)
    {
        std::vector<int> undefined;
        std::vector<int> road;

        for(int dir = 0; dir < 4; dir++)
        {
            if(getBoard(i+dy[dir], j + dx[dir]) == UNDEFINED)
                undefined.push_back(dir);
            else if(getBoard(i+dy[dir], j + dx[dir]) == ROAD)
                road.push_back(dir);
        }
        edge.addState(toNum(i, j), -1);
        // road -> undefined -> undefined : edgeState not change
        // road -> undefined -> road2      : edgeState road2.road 
        if(undefined.size() == 1 && road.size() == 1)
        {
            merge(deepth, i, j, undefined[0]);
        }
        // road - road : edgeState not change
        // dangerous: dont close = > if close false => false
        else if(undefined.size() == 1 && road.size() == 2)
        {
            if(edge.getState(toNum(i, j)) == 0)
                return false;
            setBoard(i + dy[undefined[0]], j + dx[undefined[0]], WALL);
        }
        // road - undefined : edgeState not change
        // if road - undefined - road
        else if(undefined.size() == 2 && road.size() == 1)
        {
            int s = getRandomNumber(0, 1);
            int us = (s == 0)? 1 : 0;

            merge(deepth, i, j, undefined[s]);
            setBoard(i + dy[undefined[us]] , j + dx[undefined[us]], WALL);
        }
        else if(undefined.size() == 2 && road.size() == 0)
        {
            merge(deepth, i, j, undefined[0]);
            merge(deepth, i, j, undefined[1]);
        }
        else if(undefined.size() == 3 && road.size()== 0)
        {
            int us = getRandomNumber(0, 2);
            for(int s = 0; s < 3; s++)
            {
                if(s == us)
                    setBoard(i + dy[undefined[us]] , j + dx[undefined[us]], WALL);
                else
                {
                    merge(deepth, i, j, undefined[s]);
                }
            }
        }
        else if(undefined.size() == 1 && road.size() == 0)
        {
            setBoard(i, j, WALL);
            setBoard(i + dy[undefined[0]], j + dx[undefined[0]], WALL);
        }        
        // 4 = > 2 + 2 => dont close!
        else if(undefined.size() == 2 && road.size() == 2)
        {
            int isfour = getRandomNumber(0, 6);
            if(isfour == 4||edge.getState(toNum(i, j)) == 0)
            {
                merge(deepth, i, j, undefined[0]);
                merge(deepth, i, j, undefined[1]);
            }
            else
            {
                setBoard(i + dy[undefined[0]], j + dx[undefined[0]], WALL);
                setBoard(i + dy[undefined[1]], j + dx[undefined[1]], WALL);
            }

        }
        //4 => 3 + 1 => dont close
        else if(undefined.size() == 3 && road.size() == 1)
        {
            int isfour = getRandomNumber(0, 6);
            if(isfour == 4 || edge.getState(toNum(i, j)) == 0)
            {
                merge(deepth, i, j, undefined[0]);
                merge(deepth, i, j, undefined[1]);
                merge(deepth, i, j, undefined[2]);
            }
            else
            {
                int s = getRandomNumber(0, 2);
                for(int us = 0; us < 3; us++)
                {
                    if(s == us)
                    {
                        merge(deepth, i, j, undefined[s]);
                    }
                    else
                        setBoard(i + dy[undefined[us]] , j + dx[undefined[us]], WALL);
                }
            }

        }
        else if(undefined.size() == 1 && road.size() == 3)
        {
            merge(deepth, i, j, undefined[0]);
        }

        return true;
    }
} 
