#include "eulerGen.h"

namespace euler
{
    //Setting GameBoard height x wide
    //initillize '#' = wall and roadSetting
    GameBoard::GameBoard(int n)
    {
        edge = disjoint::Disjoint(n * n);
        pq = nlib::maxHeap<double, int>();
        board = std::vector<std::vector<int> >(n, std::vector<int>(n, 1));
        InitialBoard();
    }
    void GameBoard::InitialBoard()
    {
        for(int i = 0; i < getHeight(); i++)
        {
            for(int j = 0; j < getWide(); j++)
            {
                if((i >= 1 && i < getHeight()-1) && (j >= 1 && j <getWide() -1))
                    setBoard(i, j, FOUR);
                else
                {
                    for(int d = 0; d < 4; d++)
                    {
                        if(getBoard(getNextI(i, d), getNextJ(j, d)) != -1)
                            setBoard(i, j, getBoard(i, j) * dir[d]);
                    }
                }
            }
        }
        printBoard();
    }
    int GameBoard::toNum(int i, int j) { return i * getWide() + j; }
    int GameBoard::getBoard(int i, int j) const 
    {
        if (i < 0 || j < 0 || i >= getHeight() || j >= getWide())
        {
            return -1;
        }
         return board[i][j]; 
    }
    int GameBoard::getNextI(int i, int dir) 
    {
        return i + dy[dir];
    }
    int GameBoard::getNextJ(int j, int dir) {return j + dx[dir];}
    void GameBoard::setBoard(int i, int j, int that)
    {
        if (i < 0 || j < 0 || i >= getHeight() || j >= getWide())
        {
            return;
        }
        board[i][j] = that;
    }
    int GameBoard::getHeight() const { return board.size(); }
    int GameBoard::getWide() const { return board[0].size(); }
    int GameBoard::getEdgeState(int i, int j) { return edge.getState(toNum(i, j)); }
    void GameBoard::setEdgeState(int i, int j, int x) { edge.setState(toNum(i, j), x); }
    void GameBoard::addEdgeState(int i, int j, int x) { edge.addState(toNum(i, j), x); }
    bool GameBoard::isRange(int i, int j, int deepth)
    {
        int hLoHi[2] = { deepth, getHeight() - 1 - deepth};
        int wLoHi[2] = { deepth, getWide() - 1 - deepth};
        return (hLoHi[0] == i || i == hLoHi[1]) && (wLoHi[0] <= j && j <= wLoHi[1])
            || (hLoHi[0] <= i && i <= hLoHi[1]) && (wLoHi[0] == j || j == wLoHi[1]);
    }
    //print board
    void GameBoard::printBoard()
    {
        std::wstring_convert<std::codecvt_utf8<wchar_t>,wchar_t> convert; // converts between UTF-8 and UCS-4 (given sizeof(wchar_t)==4)
        int height = getHeight() * 2 + 1;
        int width = getWide() * 2 + 1;
        std::vector<std::vector<int> > tmpBoard(height, std::vector<int>(width, WALL));
        /*
        for(int i = 0; i < getHeight(); i++)
        {
            for(int j = 0; j < getWide(); j++)
            {
                std::cout<<getBoard(i, j)<<" ";
            }
            std::cout<<"\n";
        }
        */
        for(int i = 0; i < getHeight(); i++)
            for(int j = 0; j < getWide(); j++)
            {
                if(getBoard(i, j) == 1) continue;
                int ii = i*2+1, jj = j*2+1;
                tmpBoard[ii][jj] = ROAD;
                for(int d = 0; d < 4; d++)
                {
                    int iii = getNextI(ii, d), jjj = getNextJ(jj, d);
                    int iiii = getNextI(iii, d), jjjj = getNextJ(jjj, d);
                    if(getBoard(getNextI(i, d), getNextJ(j, d)) == -1) continue;
                    if(getBoard(i, j)%dir[d] == 0)
                    {
                        tmpBoard[iii][jjj] = ROAD;
                        tmpBoard[iiii][jjjj] = ROAD;
                    }
                }
            }

        for (int i = 0; i < height; i++)
        {
            for (int j = 0; j < width; j++)
            {
                std::wstring ch = L"��";
                if (tmpBoard[i][j] == ROAD)
                    std::cout<<"  ";
                else if (tmpBoard[i][j] == UNDEFINED)
                    std::cout<<"..";
                else
                 std::cout << convert.to_bytes(ch);
            }
            std::cout << i << "\n";
        }
        std::cout << "\n";
    }
    std::vector<std::vector<int> > GameBoard::getAdj(int i, int j)
    { 
        std::vector<std::vector<int> > ret = std::vector<std::vector<int> > (3);
        for(int d = 0; d < 4; d++)
        {
            int ii = getNextI(i, d), jj = getNextJ(j, d);
            if(getBoard(ii, jj) != -1)
            {
                if(getBoard(ii, jj)% DEFINED == 0 && (getBoard(ii, jj)/DEFINED)% dir[reverseDir(d)] == 0 )
                    ret[ROAD].push_back(d);
                else if(getBoard(ii, jj)% DEFINED == 0 && (getBoard(ii, jj)/DEFINED)% dir[reverseDir(d)] != 0)
                    ret[WALL].push_back(d);
                else
                    ret[UNDEFINED].push_back(d);
            }
            else
                ret[WALL].push_back(d);
        }
        return ret;
    }
    int GameBoard::reverseDir(int d)
    {
        return (d+2)%4;
    }
}





namespace euler
{
    void GameBoard::updatePriority(int deepth, int i, int j)
    {
        double priority = getPriority(deepth, i, j);
        pq.update(std::make_pair(priority, toNum(i, j)));
    }
    void GameBoard::changePriority(int deepth, int i, int j, std::vector<int>& undefined)
    {
        for (int d = 0; d < (int)undefined.size(); d++)
            if (getBoard(getNextI(i, undefined[d]), getNextJ(j, undefined[d])) == WALL)
                updatePriority(deepth, getNextI(i , undefined[d]) , getNextJ(j, undefined[d]));
    }
    double GameBoard::getPriority(int deepth, int i, int j)
    {
        int hLoHi[2] = {deepth, getHeight() - 1 - deepth};
        int wLoHi[2] = {deepth, getWide() - 1 - deepth};

        std::vector<std::vector<int> > adj = getAdj(i, j);
        double diff = (double)getRandomNumber(0, 99) / 100.0;
        double priority = 1;
        if (adj[WALL].size() == 2)
            priority = 10000;
        else if (adj[WALL].size() == 1)
        {
            if (adj[UNDEFINED].size() == 2 && getEdgeState(i, j) == 1)
                priority = 100;
            else if (adj[UNDEFINED].size() == 2)
                priority = 50;
            else if (adj[UNDEFINED].size() == 1)
                priority = 1000;
            else
                priority = 10;
        }
        else if (adj[WALL].size() == 0)
        {
            if (adj[ROAD].size() == 2 && ((i == hLoHi[0] || i == hLoHi[1]) && (j == wLoHi[0] || j == wLoHi[1]) || getEdgeState(i, j) == 1))
                priority = 100;
            else if (adj[ROAD].size() == 2)
                priority = 50;
            else if (adj[ROAD].size() == 1)
                priority = 10;
            else if (adj[ROAD].size() == 3)
                priority = 1000;
        }
        else if (adj[ROAD].size() == 3)
            priority = 1000;
       if (adj[ROAD].size() > 0)
            priority += 20;
        return priority - diff;
    }
    void GameBoard::pushToPq(int deepth, int i, int j)
    {
        double priority = getPriority(deepth, i, j);
        pq.push(std::make_pair(priority, toNum(i, j)));
    }

    bool GameBoard::merge(int deepth, int i, int j, int d)
    {
        int ii = getNextI(i, d), jj = getNextJ(j, d);
        std::vector<std::vector<int> > adj = getAdj(ii, jj);

        if (adj[UNDEFINED].size() == 1)
            addEdgeState(i, j, -1);
        if (adj[UNDEFINED].size() == 0)
            return false;

        edge.merge(toNum(i, j), toNum(ii, jj));
        return true;
    }

    bool GameBoard::currentDeepthSetting(int deepth, GameBoard& gameBoard)
    {
        if (deepth > getHeight() / 2)
            return true;
        GameBoard tmp = gameBoard;
        tmp.pushAllDeepth(deepth);

        while (!tmp.pq.empty())
        {
            int here = tmp.pq.top().second; 
            int j = here % getWide(), i = here / getWide();
            tmp.pq.pop();
            if (!tmp.setRoood(deepth, i, j))
            {
   //            std::cout << "false>> "<<deepth<<"\n";
                return false;
            }
            tmp.updateAllDeepth(deepth);
        }
      //  char ch;
     //   tmp.printBoard();
     //   std::cin>>ch;

        int count = 0;
        while (!currentDeepthSetting(deepth + 1, tmp))
        { if(count++ == 3) return false;}
        gameBoard = tmp;
        return true;
    }
    void GameBoard::updateAllDeepth(int deepth)
    {
        int hLoHi[2] = {deepth, getHeight() - 1 - deepth };
        int wLoHi[2] = {deepth, getWide() - 1 - deepth};

        //left and right
        for (int i = hLoHi[0]; i <= hLoHi[1]; i++)
            for (int j = 0; j < 2; j++)
                updatePriority(deepth, i, wLoHi[j]);
        //up and down
        for (int j = wLoHi[0] + 1; j <= wLoHi[1] - 1; j++ )
            for (int i = 0; i < 2; i++)
                updatePriority(deepth, hLoHi[i], j);
    }
    bool GameBoard::check()
    {
        int ret = true;
        for (int i = 0; i < getHeight(); i++)
            for (int j = 0; j < getWide(); j++)
                {
                    if(getBoard(i, j)%DEFINED != 0) continue;
    
                    int count = 0;
                    for(int d = 0; d < 4; d++)
                    {
                        if((getBoard(i, j)/DEFINED)%dir[d] == 0)
                        {
                            count++;
                        }
                    }
                    if(count == 3 || count == 1)
                    {
                        std::cout << "odd i: " << i << " j: " << j<<" \n";
                        ret= false;
                    }
                }
        return ret;
    }
    

    void GameBoard::pushAllDeepth(int deepth)
    {
        int hLoHi[2] = {deepth, getHeight() - 1 - deepth};
        int wLoHi[2] = {deepth, getWide() - 1 - deepth };

        //left and right
        for (int i = hLoHi[0]; i <= hLoHi[1]; i++ )
            for (int j = 0; j < 2; j++)
                pushToPq(deepth, i, wLoHi[j]);

        //up and down
        for (int j = wLoHi[0] + 1; j <= wLoHi[1] - 1; j++)
            for (int i = 0; i < 2; i++)
                pushToPq(deepth, hLoHi[i], j);
    }
    bool GameBoard::setRoood(int deepth, int i, int j)
    {
        std::vector<std::vector<int> > adj = getAdj(i, j);
        std::vector<int> undefined = adj[UNDEFINED], road = adj[ROAD], wall = adj[WALL];
        std::vector<int> selected, cantSet, canSet, doSet, toWall;

        for(int ud = 0; ud < undefined.size(); ud++)
        {
            int ui = getNextI(i, undefined[ud]);
            int uj = getNextJ(j, undefined[ud]);

            std::vector<std::vector<int> > nextAdj = getAdj(ui, uj);
            if(nextAdj[UNDEFINED].size() == 1 && (nextAdj[ROAD].size() == 2 || nextAdj[ROAD].size() == 0))
                cantSet.push_back(undefined[ud]);
            else
                canSet.push_back(undefined[ud]);
            if(nextAdj[UNDEFINED].size() == 1 && (nextAdj[ROAD].size() == 1 || nextAdj[ROAD].size() == 3))
                doSet.push_back(undefined[ud]);
        }

        addEdgeState(i, j, -1);


        if(wall.size() == 0)
        {
            if(undefined.size() == 0) // ROAD == 4
            {
                addEdgeState(i, j, 1);
                return true;
            }
            else if(undefined.size() == 1) // ROAD == 3
            {
                if(canSet.size() == 0)
                    return false;
                else
                    selected = undefined;
            }
            else if(undefined.size() == 2) // ROAD == 2
            {
                if(cantSet.size() == 2) return false;
                int p = getRandomNumber(0, 5);
                if(cantSet.size() == 0 && (doSet.size() > 0 || getEdgeState(i, j) == 0 || p != 1))
                    selected = undefined;
                else if(doSet.size() > 0)
                    return false;
            }
            else if(undefined.size() == 3)
            {
                int p = getRandomNumber(0, 5);
                if(cantSet.size() == 3)
                    return false;
                if((p != 1 || doSet.size() > 1 ||getEdgeState(i, j) == 0 )&& cantSet.size() == 0)
                    selected = undefined;
                else
                {
                    if(doSet.size() > 1)
                        return false;
                    if(doSet.size() == 1)
                        selected = doSet;
                    else 
                        selected.push_back(canSet[getRandomNumber(0, canSet.size() -1)]);
                }
            }
            else return false;
        }
        else if(wall.size() == 1)
        {
            if(undefined.size() == 0) // ROAD == 3
            {
                return false;
            }
            else if(undefined.size() == 1) //ROAD == 2
            {
                if(doSet.size() == 1 || getEdgeState(i, j) == 0)
                    return false;
            }
            else if(undefined.size() == 2) // ROAD == 1
            {
                if(cantSet.size() == 2)
                    return false;
                if(doSet.size() == 1)
                    selected = undefined;
                else
                    selected.push_back(canSet[getRandomNumber(0, canSet.size()-1)]);
            }
            else return false;
        }
        else if(wall.size() == 2)
        {
            if(undefined.size() == 0)
            {
                addEdgeState(i, j, 1);
                return true;
            }
            if(undefined.size() == 1)
            {
                if(cantSet.size() == 1)
                    return false;
                selected = undefined;
            }
            if(undefined.size() == 2)
            {
                if(cantSet.size() != 0)
                    return false;
                else 
                    selected = undefined;
            }
        }
        else 
            setBoard(i, j, 1);
        
        for(int ud = 0; ud < undefined.size(); ud++)
        {
            bool isSelected = false;
            for(int sl = 0; sl < selected.size(); sl++)
            {
                if(undefined[ud] == selected[sl])
                    isSelected = true;
            }
            if(!isSelected)
                toWall.push_back(undefined[ud]);
        }
        setWall(deepth, i, j, toWall);
        setRoad(deepth, i, j, selected);
        return true;
    }
    void GameBoard::setWall(int deepth, int i, int j, std::vector<int>& toWall)
    {
        for(int tw = 0; tw < toWall.size(); tw++)
        {
            int ii = getNextI(i, toWall[tw]), jj = getNextJ(j, toWall[tw]);
            setBoard(i, j, getBoard(i, j)/dir[toWall[tw]]);
            setBoard(ii, jj, getBoard(ii, jj)/dir[reverseDir(toWall[tw])]);
        }
    }
    bool GameBoard::setRoad(int deepth, int i, int j, std::vector<int>& selected)
    {
        setBoard(i, j, getBoard(i, j) * DEFINED);
        for(int s = 0; s < selected.size(); s++)
            merge(deepth, i, j, selected[s]);
        return true;
    }


    int getRandomNumber(int min, int max)
    {
        static const double fraction = 1.0 / (RAND_MAX + 1.0); // static used for efficiency, so we only calculate this value once
         // evenly distribute the random number across our range 
        return min + static_cast<int>((max - min + 1) * (std::rand() * fraction));
    }
}
