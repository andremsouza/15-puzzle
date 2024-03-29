#include <iostream>
#include <cstdio>
#include <vector>
#include <queue>

using namespace std;

typedef vector<int> vi;
typedef vector<vi> vvi;
typedef pair<int, int> ii;

bool solved = false;

/**
 * Count number of inversions from the grid array
 * */
int inversionCount(vi f){
	int result = 0;
	for(int i=0; i<16; i++)
		for(int j=i+1; j<16; j++){
			if (f[i] == 0 || f[j] == 0) continue;
			if (f[i] > f[j]) result++;
		}
	return result;
}

/**
 * find coordinates of c in the grid
 * */
ii findPosition(vvi grid, int c){
	for(int i=0; i<4; i++)
		for(int j=0; j<4; j++)
			if (grid[i][j] == c) return make_pair(i,j);
	return make_pair(-1, -1); //not found
}

/**
 * is solvable if:
 * 	(1) the blank is on an even row counting from the bottom (second-last, fourth-last, etc.) and number of inversions is odd.
 *  or
 *  (2) the blank is on an odd row counting from the bottom (last, third-last, fifth-last, etc.) and number of inversions is even.
 *  else not solvable
 * */
bool isSolvable(vvi grid){
	vi f(16, 0);
	for(int i=0; i<4; i++)
		for(int j=0; j<4; j++)
			f[i*4+j] = grid[i][j]; //Transforms grid into an array
	ii blank = findPosition(grid, 0);
	int count = inversionCount(f);

	if ((4 - blank.first) % 2 == 0) return count % 2 == 1; // (1)
	return count % 2 == 0; // (2)
}



/**
 * Check if the grid is solved
 * */
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

/**
 * Auxiliar function that prints the situation.
 * */
void print(vvi grid){
  for (int i = 0; i < 4; i++){
      for (int j = 0; j < 4; j++) cout << grid[i][j] << " ";
      cout << "\n";
    }
    cout << "\n";
}

/**
 * The next four functions are used to manipulate the element '0' of the 
 * grid.
 * */

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

/**
 * The next four functions are used to calculate the next postion of the
 * element '0' of the grid, after some operation above.
 * */

ii newZeroUp(ii zeroPos){
  ii newZero;
  newZero.first = zeroPos.first -1;
  newZero.second = zeroPos.second;
  return newZero;
}

ii newZeroDown(ii zeroPos){
  ii newZero;
  newZero.first = zeroPos.first +1;
  newZero.second = zeroPos.second;
  return newZero;
}

ii newZeroRight(ii zeroPos){
  ii newZero;
  newZero.first = zeroPos.first;
  newZero.second = zeroPos.second +1;
  return newZero;
}

ii newZeroLeft(ii zeroPos){
  ii newZero;
  newZero.first = zeroPos.first;
  newZero.second = zeroPos.second -1;
  return newZero;
}

/**
 * Recursively tests the possibilities until finds(or not) the answer. 
 * Gets respectively the grid, the position of the element '0', 
 * how many interactions, lastMovement and a queue with all the movements
 * that were made to get at that point. 
 * 
 * */
void backTracking(vvi grid, ii zeroPos, int steps, char lastMove, queue <char> q){
  if(is_solved(grid)){
    q.push(lastMove);                                                   //Adds the lastMove to the queue
    while(!q.empty()){
      cout << q.front();
      q.pop();
    }
    cout << "\n";
    solved = true;
  }
  
  else if(steps < 46 && solved == false){
    if(lastMove == 'n'){                                                //lastMove = NULL -> First Move
      steps++;                                                          //Tests respectively up, down, right, left 
      if(zeroPos.first != 0) backTracking(moveUp(grid, zeroPos), newZeroUp(zeroPos), steps, 'U', q); 
      if(zeroPos.first != 3) backTracking(moveDown(grid, zeroPos), newZeroDown(zeroPos), steps, 'D', q);
      if(zeroPos.second != 3) backTracking(moveRight(grid, zeroPos), newZeroRight(zeroPos), steps, 'R', q);
      if(zeroPos.second != 0) backTracking(moveLeft(grid, zeroPos), newZeroLeft(zeroPos), steps, 'L', q);
    }
    
    if(lastMove == 'U'){                                                //lastMove = Up -> Can't go down
      steps++;                                                          //Tests respectively right, left, up  
      queue <char> u = q;
      u.push('U');
      if(zeroPos.second != 3) backTracking(moveRight(grid, zeroPos), newZeroRight(zeroPos), steps, 'R', u);
      if(zeroPos.second != 0) backTracking(moveLeft(grid, zeroPos), newZeroLeft(zeroPos), steps, 'L', u);
      if(zeroPos.first != 0) backTracking(moveUp(grid, zeroPos), newZeroUp(zeroPos), steps, 'U', u);
    }
    
    if(lastMove == 'D'){                                                //lastMove = Down -> Can't go up
      steps++;                                                          //Tests respectively down, right, left
      queue <char> d = q;
      d.push('D');
      if(zeroPos.first != 3) backTracking(moveDown(grid, zeroPos), newZeroDown(zeroPos), steps, 'D', d);
      if(zeroPos.second != 3) backTracking(moveRight(grid, zeroPos), newZeroRight(zeroPos), steps, 'R', d);
      if(zeroPos.second != 0) backTracking(moveLeft(grid, zeroPos), newZeroLeft(zeroPos), steps, 'L', d);
    }
    
    if(lastMove == 'R'){                                                //lastMove = Right -> Can't go left
      steps++;                                                          //Tests respectively down, right, up 
      queue <char> r = q;
      r.push('R');
      if(zeroPos.first != 3) backTracking(moveDown(grid, zeroPos), newZeroDown(zeroPos), steps, 'D', r);
      if(zeroPos.second != 3) backTracking(moveRight(grid, zeroPos), newZeroRight(zeroPos), steps, 'R', r);
      if(zeroPos.first != 0) backTracking(moveUp(grid, zeroPos), newZeroUp(zeroPos), steps, 'U', r);
    }
    
    if(lastMove == 'L'){                                                //lastMove = Left -> Can't go right
      steps++;                                                          //Tests respectively down, left, up  
      queue <char> l = q;
      l.push('L');
      if(zeroPos.first != 3) backTracking(moveDown(grid, zeroPos), newZeroDown(zeroPos), steps, 'D', l);
      if(zeroPos.second != 0) backTracking(moveLeft(grid, zeroPos), newZeroLeft(zeroPos), steps, 'L', l);
      if(zeroPos.first != 0) backTracking(moveUp(grid, zeroPos), newZeroUp(zeroPos), steps, 'U', l);
    }
  }
}

int main(int argc, char** argv) {
  int N;
  queue <char> q;
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
    solved = false;
    if(isSolvable(grid)) backTracking(grid, zeroPos, 0, 'n', q);
    else cout << "This puzzle is not solvable.\n";
  }
  return 0;
}
