#include "eulerGen.h"
#include <string>

namespace euler
{
    //Setting GameBoard height x wide
    //initillize '#' = wall and roadSetting
    GameBoard::GameBoard(int n)
    {
        int wide = n * 2 + 1;
        int height = n * 2 + 1;
        edge = disjoint::Disjoint(wide * height);
        pq = nlib::maxHeap<double, int>();
        board = std::vector<std::vector<int> >(height, std::vector<int>(wide, WALL));
        initialSetting();
    }
    int GameBoard::toNum(int i, int j) { return i * getWide() + j; }
    int GameBoard::getBoard(int i, int j) const { return board[i][j]; }
    void GameBoard::setBoard(int i, int j, int that)
    {
        if (i < 0 || j < 0 || i >= getHeight() || j >= getWide())
        {
            std::cout << "i " << i << " j: " << j << "\n";
            std::cout << "BOUND ERROR\n";
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
        int hLoHi[2] = { 1 + deepth * 2, getHeight() - 2 - deepth * 2 };
        int wLoHi[2] = { 1 + deepth * 2, getWide() - 2 - deepth * 2 };
        return (hLoHi[0] == i || i == hLoHi[1]) && (wLoHi[0] <= j && j <= wLoHi[1])
            || (hLoHi[0] <= i && i <= hLoHi[1]) && (wLoHi[0] == j || j == wLoHi[1]);
    }
    //initial setting UNDEFINED
    void GameBoard::initialSetting()
    {
        for (int i = 1; i < getHeight() - 1; i++)
            for (int j = 1; j < getWide() - 1; j++)
                if (i % 2 == 1 || j % 2 == 1)
                    setBoard(i, j, UNDEFINED);
    }
    //print board
    void GameBoard::printBoard()
    {
        for (int i = 0; i < getHeight(); i++)
        {
            for (int j = 0; j < getWide(); j++)
            {
                std::string ch ="бр";
                if (getBoard(i, j) == ROAD)
                    ch = "  ";
                if (getBoard(i, j) == UNDEFINED)
                    ch = "..";
                std::cout << ch;
            }
            std::cout << i << "\n";
        }
        for (int i = 0; i < getWide(); i++)
            std::cout << i<<".";
        std::cout << "\n";
    }
}





namespace euler
{
    std::vector<std::vector<int> > GameBoard::getAdj(int i, int j)
    {
        std::vector<std::vector<int> > adj = std::vector<std::vector<int> >(3, std::vector<int>());
        for (int d = 0; d < 4; d++)
            adj[getBoard(i + dy[d], j + dx[d])].push_back(d);
        return adj;
    }
    void GameBoard::updatePriority(int deepth, int i, int j)
    {
        double priority = getPriority(deepth, i, j);
      //  std::cout << " i: " << i << " j: " << j <<" p: " << priority << " ";
        if (pq.update(std::make_pair(priority, toNum(i, j))))
        { }
       //    std::cout << " succc\n";
      //  else
      //      std::cout << " fail\n";
    }
    void GameBoard::changePriority(int deepth, int i, int j, std::vector<int>& undefined)
    {
        for (int d = 0; d < (int)undefined.size(); d++)
        {
            if (getBoard(i + dy[undefined[d]], j + dx[undefined[d]]) == WALL)
            {
                updatePriority(deepth, i + dy[undefined[d]] * 2, j + dx[undefined[d]]*2);
            }
        }
 //       std::cout << "    end change Priority\n";
    }
    void GameBoard::setWall(int deepth, int i, int j, std::vector<int>& undefined)
    {
 //   std::cout << "start SetWall "<<i <<" "<<j<<"\n";
        for (int d = 0; d < (int)undefined.size(); d++)
        {
            if (getBoard(i + dy[undefined[d]], j + dx[undefined[d]]) == UNDEFINED)
            {
      //          std::cout<<"      wall   "<<i <<" "<<j<<"\n";
                setBoard(i + dy[undefined[d]], j + dx[undefined[d]], WALL);
            }
        }
        //changePriority(deepth, i, j, undefined);
 //   std::cout << "end SetWall\n";
    }
    double GameBoard::getPriority(int deepth, int i, int j)
    {
        int hLoHi[2] = { 1 + deepth * 2, getHeight() - 2 - deepth * 2 };
        int wLoHi[2] = { 1 + deepth * 2, getWide() - 2 - deepth * 2 };
     //   std::cout << "            get Priority\n";
        std::vector<std::vector<int> > adj = getAdj(i, j);
        double diff = (double)getRandomNumber(0, 99) / 100.0;
        double priority = 1;
        if (adj[1].size() == 2)
            priority = 100;
        else if (adj[1].size() == 1)
        {
            if (adj[0].size() == 2 && getEdgeState(i, j) == 1)
                priority = 100;
            else if (adj[0].size() == 2)
                priority = 50;
            else if (adj[0].size() == 1)
                priority = 30;
            else
                priority = 10;
        }
        else if (adj[1].size() == 0)
        {
            if (adj[2].size() == 2 && ((i == hLoHi[0] || i == hLoHi[1]) && (j == wLoHi[0] || j == wLoHi[1]) || getEdgeState(i, j) == 1))
                priority = 100;
            else if (adj[2].size() == 2)
                priority = 50;
            else if (adj[2].size() == 1)
                priority = 10;
            else if (adj[2].size() == 3)
                priority = 100;
        }
        else if (adj[1].size() == 3)
            priority = 1000;
        if (getBoard(i, j) == ROAD)
            priority += 20;
        return priority - diff;
    }
    void GameBoard::pushToPq(int deepth, int i, int j)
    {
        double priority = getPriority(deepth, i, j);
        pq.push(std::make_pair(priority, toNum(i, j)));
    }

    void GameBoard::merge(int deepth, int i, int j, int ii, int jj)
    {
        setBoard(ii, jj, ROAD);
        edge.merge(toNum(i, j), toNum(ii, jj));
    }
    bool GameBoard::merge(int deepth, int i, int j, int d)
    {
//    std::cout << "    merge i, j: " << i << " " << j << "\n";
        int ii = i + dy[d], jj = j + dx[d], iii = i + dy[d] * 2, jjj = j + dx[d] * 2;
        if (getBoard(i, j) == ROAD && getBoard(ii, jj) == ROAD)
            return false;
        std::vector<std::vector<int> > undef = getAdj(iii, jjj);
        if (undef[0].size() == 1)
            addEdgeState(i, j, -1);
        if (undef[0].size() == 0)
            return false;

        addEdgeState(i, j, -1);
        merge(deepth, i, j, ii, jj);
        merge(deepth, i, j, iii, jjj);
        setBoard(i, j, ROAD);
       // updatePriority(deepth, iii, jjj);
   //     std::cout << "    merge end i, j: " << i << " " << j << "\n";
        return true;
    }

    bool GameBoard::currentDeepthSetting(int deepth, GameBoard& gameBoard)
    {
        if (deepth > getHeight() / 4)
        {
            return true;
        }
        GameBoard tmp = gameBoard;
        tmp.pushAllDeepth(deepth);
 
        while (!tmp.pq.empty())
        {
            int here = tmp.pq.top().second; 
            int j = here % getWide(), i = here / getWide();
            tmp.pq.pop();
            if (!tmp.setRoad(deepth, i, j))
            {
                std::cout << "false>> "<<deepth<<"\n";
                return false;
            }
            tmp.updateAllDeepth(deepth);
        }
        if (!check(deepth)) return false;
        tmp.printBoard();

        while (!currentDeepthSetting(deepth + 1, tmp))
        {}
        gameBoard = tmp;
        return true;
    }
    void GameBoard::updateAllDeepth(int deepth)
    {
        int hLoHi[2] = { 1 + deepth * 2, getHeight() - 2 - deepth * 2 };
        int wLoHi[2] = { 1 + deepth * 2, getWide() - 2 - deepth * 2 };

        //left and right
        for (int i = hLoHi[0]; i <= hLoHi[1]; i += 2)
            for (int j = 0; j < 2; j++)
                updatePriority(deepth, i, wLoHi[j]);
        //up and down
        for (int j = wLoHi[0] + 2; j <= wLoHi[1] - 2; j += 2)
            for (int i = 0; i < 2; i++)
                updatePriority(deepth, hLoHi[i], j);
    }
    bool GameBoard::check(int deepth)
    {
        int ret = true;
        for (int i = 1; i < getHeight() - 1; i++)
            for (int j = 1; j < getWide() - 1; j++)
                if (i % 2 == 1 || j % 2 == 1)
                {
                    std::vector<std::vector<int> > adj = getAdj(i, j);
                    if ((adj[ROAD].size() == 1 && adj[WALL].size() == 3) || (adj[ROAD].size() == 3 && adj[WALL].size() == 1))
                    {
                        std::cout << "i: " << i << " j: " << j<<" \n";
                        ret= false;
                    }
                }
        return ret;
    }
    

    void GameBoard::pushAllDeepth(int deepth)
    {
        int hLoHi[2] = { 1 + deepth * 2, getHeight() - 2 - deepth * 2 };
        int wLoHi[2] = { 1 + deepth * 2, getWide() - 2 - deepth * 2 };

        //left and right
        for (int i = hLoHi[0]; i <= hLoHi[1]; i += 2)
            for (int j = 0; j < 2; j++)
                pushToPq(deepth, i, wLoHi[j]);

        //up and down
        for (int j = wLoHi[0] + 2; j <= wLoHi[1] - 2; j+=2)
            for (int i = 0; i < 2; i++)
                pushToPq(deepth, hLoHi[i], j);
    }

    bool GameBoard::setRoad(int deepth, int i, int j)
    {
        std::vector<std::vector<int> > adj = getAdj(i, j);
        std::vector<int> undefined = adj[0];
        std::vector<int> road = adj[2];

        if (undefined.size() == 0 && road.size() == 1)
        {
            setBoard(i, j, WALL);
            setBoard(i+dy[road[0]], j + dx[road[0]], WALL);
            return true;
        }
            
        if (undefined.size() == 0) return true;

        addEdgeState(i, j, -1);

        if (undefined.size() == 1 && road.size() == 1)
            merge(deepth, i, j, undefined[0]);
        //3
        else if (undefined.size() == 1 && road.size() == 2)
        {
            if (edge.getState(toNum(i, j)) == 0)
                return false;
        }
        else if (road.size() == 3 && undefined.size() == 0)
        {
            return false;
        }
        //3
        else if (undefined.size() == 2 && road.size() == 1)
        {
            int s = getRandomNumber(0, 1);
            int us = (s == 0) ? 1 : 0;

            merge(deepth, i, j, undefined[s]);
        }
        else if (undefined.size() == 2 && road.size() == 0)
        {
            merge(deepth, i, j, undefined[0]);
            merge(deepth, i, j, undefined[1]);
        }
        //3
        else if (undefined.size() == 3 && road.size() == 0)
        {
            int us = getRandomNumber(0, 2);
            for (int s = 0; s < 3; s++)
                if (s != us)
                    merge(deepth, i, j, undefined[s]);
        }
        else if (undefined.size() == 1 && road.size() == 0)
        {
            setBoard(i, j, WALL);
        }
        // 4 = > 2 + 2 => dont close!
        else if (undefined.size() == 2 && road.size() == 2)
        {
            int isfour = getRandomNumber(0, 6);
            if (isfour == 4 || edge.getState(toNum(i, j)) == 0)
            {
                merge(deepth, i, j, undefined[0]);
                merge(deepth, i, j, undefined[1]);
            }
        }
        //4 => 3 + 1 => dont close
        else if (undefined.size() == 3 && road.size() == 1)
        {
            int isfour = getRandomNumber(0, 6);
            if (isfour == 4 || edge.getState(toNum(i, j)) == 0)
            {
                merge(deepth, i, j, undefined[0]);
                merge(deepth, i, j, undefined[1]);
                merge(deepth, i, j, undefined[2]);
            }
            else
            {
                int s = getRandomNumber(0, 2);
                merge(deepth, i, j, undefined[s]);
            }
        }
        else if (undefined.size() == 1 && road.size() == 3)
            merge(deepth, i, j, undefined[0]);

        setWall(deepth, i, j, undefined);

        return true;
    }
    int getRandomNumber(int min, int max)
    {
        static const double fraction = 1.0 / (RAND_MAX + 1.0); // static used for efficiency, so we only calculate this value once
         // evenly distribute the random number across our range 
        return min + static_cast<int>((max - min + 1) * (std::rand() * fraction));
    }
}
