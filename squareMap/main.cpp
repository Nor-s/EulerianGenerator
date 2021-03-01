#include <iostream>
#include <ctime>
#include "eulerGen.h"

using namespace std;

int main()
{
    std::srand(static_cast<int>(time(0)));
    //input height, wide
    while (true)
    {
        int height, wide;
        cout << "\nInput height : ";
        cin >> height;
        cout << "\nInput wide : ";
        cin >> wide;
        if (height < 0 || wide < 0) break;
        //genEuler
        euler::GameBoard gameboard(height, wide);
        //showEuler
        gameboard.printBoard();
        gameboard.currentDeepthSetting(0, gameboard);
        gameboard.printBoard();
        cout << "END\n";
    }

    return 0;
}