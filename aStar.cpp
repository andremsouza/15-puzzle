/*
This program receives one or more 15-puzzle grids, and solves them using an A* approach.
The input/output of this program is specified the UVA 10181 problem, plus the processing time.

IMPORTANT: compile with the following command line:
	g++ aStar.cpp -o aStar -O3 -Wall
*/

#include <iostream>
#include <iomanip>
#include <vector>
#include <queue>
#include <cmath>
using namespace std;
typedef vector<int> vi;
typedef vector<vi> vvi;
typedef pair<int,int> ii;

vvi solvedGrid(4, vi(4)); //solved grid. Used for some functions below.

/*
Below are the utility functions of this program.
*/

void mountSolvedGrid(){ //mount solved grid.
	int v = 1;
	for(int i=0; i<4; i++)
		for(int j=0; j<4; j++)
			solvedGrid[i][j] = v++;
	solvedGrid[3][3] = 0;
}

int inversionCount(vi f){ //Count number of inversions from the grid array
	int result = 0;
	for(int i=0; i<16; i++)
		for(int j=i+1; j<16; j++){
			if (f[i] == 0 || f[j] == 0) continue;
			if (f[i] > f[j]) result++;
		}
	return result;
}

ii findPosition(vvi grid, int c){ //find coordinates of c in the grid
	for(int i=0; i<4; i++)
		for(int j=0; j<4; j++)
			if (grid[i][j] == c) return make_pair(i,j);
}

/*
is solvable if:
	(1) the blank is on an even row counting from the bottom (second-last, fourth-last, etc.) and number of inversions is odd.
	or
	(2) the blank is on an odd row counting from the bottom (last, third-last, fifth-last, etc.) and number of inversions is even.
else not solvable
*/
bool isSolvable(vvi grid) {
	vi f(16, 0);
	for(int i=0; i<4; i++)
		for(int j=0; j<4; j++)
			f[i*4+j] = grid[i][j]; //Transforms grid into an array
	ii blank = findPosition(grid, 0);
	int count = inversionCount(f);

	if ((4 - blank.first) % 2 == 0) return count % 2 == 1; // (1)
	return count % 2 == 0; // (2)
}

bool isSolved(vvi &grid){ //Compares grid with a solved one.
	for(int i=0;i<4;i++)
		for(int j=0;j<4;j++)
			if(grid[i][j] != solvedGrid[i][j]) return false;
	return true;
}

int manhattan(vvi grid){ //priority function
	int h = 0;
	for(int i = 1; i<16; i++){ //Doesn't need to count the 0's distances.
		ii a = findPosition(grid, i), b = findPosition(solvedGrid, i);
		h += abs(a.first - b.first) + abs(a.second - b.second);
	}
	return h;
}

/*
Below are the functions and classes used to calculate the solution.
*/

struct State{
	vvi grid; // the current grid
	int f; // The result of the A* heuristic, calculated at the constructor
	string path; // The path from the initial state to this state
	State(vvi grid, string path) : grid(grid), path(path){
		f = manhattan(grid) + path.size();
	}
	bool operator>(const State &b) const{return f > b.f;}
	char prev(){ // The previous state direction (From the previous state, we went to R, L, D or U?)
		if(path.size()) return *(path.end()-1);
		else return 0;
	} 
};
typedef priority_queue<State, vector<State>, greater<State> > pqs;

//Solve the solution with an A* algorithm, with the manhattan priority function
string solve_r(pqs &pq){
	while(!pq.empty()){
		State s = pq.top(); pq.pop();
		if(isSolved(s.grid)) return s.path;
		ii zero = findPosition(s.grid, 0);
		//up
		if(zero.first > 0 && s.prev() != 'D'){
			vvi ng = s.grid; swap(ng[zero.first][zero.second], ng[zero.first-1][zero.second]); // "moves" blank
			State ns(ng, s.path + 'U'); // new state
			pq.push(ns);
		}
		//down
		if(zero.first < 3 && s.prev() != 'U'){
			vvi ng = s.grid; swap(ng[zero.first][zero.second], ng[zero.first+1][zero.second]); // "moves" blank
			State ns(ng, s.path + 'D'); // new state
			pq.push(ns);
		}
		//left
		if(zero.second > 0 && s.prev() != 'R'){
			vvi ng = s.grid; swap(ng[zero.first][zero.second], ng[zero.first][zero.second-1]); // "moves" blank
			State ns(ng, s.path + 'L'); // new state
			pq.push(ns);
		}
		//right
		if(zero.second < 3 && s.prev() != 'L'){
			vvi ng = s.grid; swap(ng[zero.first][zero.second], ng[zero.first][zero.second+1]); // "moves" blank
			State ns(ng, s.path + 'R'); // new state
			pq.push(ns);
		}
	}
}

int main(int argc, char** argv) {
	mountSolvedGrid();
	int N; cin >> N; //number of sets
	while (N--) {
		//input of each set
		vvi grid(4, vi(4));
		for (int i = 0; i < 4; i++)
			for (int j = 0; j < 4; j++)
				cin >> grid[i][j];	
		//check if not solvable
		if(!isSolvable(grid)){ cout << "This puzzle is not solvable." << endl; continue;}
		//if solvable
		State initial(grid, "");
		pqs pq;
		pq.push(initial);
		string solution = solve_r(pq);
		cout << solution << endl;
	}
	return 0;
}
