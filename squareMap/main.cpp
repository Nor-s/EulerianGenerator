#include <iostream>
#include <ctime>
#include "eulerGen.h"
using namespace std;

int main()
{
    ios_base::sync_with_stdio(false); cin.tie(0); cout.tie(0);
    
    std::srand(static_cast<int>(time(0)));
    //input height, wide
    while (true)
    {
        int n;
        cout << "Input n  : ";
        cin >> n;
        if (n < 0) break;
        if(n==3) n++;
        //genEuler
        euler::GameBoard gameboard(n);
        //showEuler
        while (!gameboard.currentDeepthSetting(0, gameboard))
        {
        }
        gameboard.printBoard();
        if (!gameboard.check())
        {
            cout << "======ODODODODODDO======\n\n";
        }
        cout << "END\n";
    }
    
    return 0;
}