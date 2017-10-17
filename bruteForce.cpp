#include <iostream>
#include <cstdio>
#include <vector>

/** André, ainda estou desenvolvendo as ideias ... Então nem tudo está perfeito ainda**/

using namespace std;

typedef vector<int> vi;
typedef vector<vi> vvi;
typedef pair<int, int> ii;

//Check if the grid is solved
bool is_solved(vvi &grid) {
  int solvedGrid[4][4] = {
    {1, 2, 3, 4},
    {5, 6, 7, 8},
    {9, 10, 11, 12},
    {13, 14, 15, 0}};
  for (int i = 0; i < 4; i++){
    for (int j = 0; j < 4; j++) if (grid[i][j] != solvedGrid[i][j]) return false;
  }
  return true;
}

void print(vvi grid){
  for (int i = 0; i < 4; i++){
      for (int j = 0; j < 4; j++) cout << grid[i][j] << " ";
      cout << "\n";
    }
    cout << "\n";
}

vvi moveLeft(vvi grid, ii zeroPos){
  grid[zeroPos.first][zeroPos.second] = grid[(zeroPos.first)][zeroPos.second-1];
  grid[(zeroPos.first)][zeroPos.second-1] = 0;
  return grid;
}

vvi moveRight(vvi grid, ii zeroPos){
  grid[zeroPos.first][zeroPos.second] = grid[(zeroPos.first)][zeroPos.second+1];
  grid[(zeroPos.first)][zeroPos.second+1] = 0;
  return grid;
}

vvi moveDown(vvi grid, ii zeroPos){
  grid[zeroPos.first][zeroPos.second] = grid[(zeroPos.first+1)][zeroPos.second];
  grid[(zeroPos.first+1)][zeroPos.second] = 0;
  return grid;
}

vvi moveUp(vvi grid, ii zeroPos){
  grid[zeroPos.first][zeroPos.second] = grid[(zeroPos.first-1)][zeroPos.second];
  grid[(zeroPos.first-1)][zeroPos.second] = 0;
  return grid;
}

void backTracking(vvi grid, ii zeroPos, int steps, char lastMove){
  if(steps < 1){
    ii newZero;
    if(lastMove == 'n'){                                                //lastMove = NULL -> First Move
      if(zeroPos.first != 0){                                             //Test Up
        steps++;
        newZero.first = zeroPos.first -1;
        newZero.second = zeroPos.second;
        backTracking(moveUp(grid, zeroPos), newZero, steps, 'u');
      }
      if(zeroPos.first != 3){                                             //Test Down
        steps++;
        newZero.first = zeroPos.first +1;
        newZero.second = zeroPos.second;
        backTracking(moveDown(grid, zeroPos), newZero, steps, 'd');
      }
      if(zeroPos.second != 3){                                            //Test Right
        steps++;
        newZero.first = zeroPos.first;
        newZero.second = zeroPos.second +1;
        backTracking(moveRight(grid, zeroPos), newZero, steps, 'r');
      }
      if(zeroPos.second != 0){                                            //Test Left
        steps++;
        newZero.first = zeroPos.first;
        newZero.second = zeroPos.second -1;
        backTracking(moveLeft(grid, zeroPos), newZero, steps, 'l');
      }
    }
  }
  //print(grid);
}

int main(int argc, char** argv) {
  int N;
  ii zeroPos;
  cin >> N;
  while (N--){
    vvi grid(4, vi(4));                                                 //input of each set
    for (int i = 0; i < 4; i++){
      for (int j = 0; j < 4; j++){
        cin >> grid[i][j];
        if(grid[i][j] == 0){                                            //finds the position of the 'blank'
          zeroPos.first = i;
          zeroPos.second = j;
        }
      }
    }
    backTracking(grid, zeroPos, 0, 'n');
    //print(grid);
  }
  return 0;
}

