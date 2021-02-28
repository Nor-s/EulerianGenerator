#include <iostream>
#include "eulerGen.h"

using namespace std;

int main()
{
    //input height, wide
    int height, wide;
    cout<<"\nInput height : ";
    cin>>height;
    cout<<"\nInput wide : ";
    cin>>wide;
    //genEuler
    euler::GameBoard gameboard(height, wide);
    //showEuler
    gameboard.printBoard();
    gameboard.currentDeepthSetting(0, gameboard);
    gameboard.printBoard();
    cout<<"END\n";
    return 0;
}