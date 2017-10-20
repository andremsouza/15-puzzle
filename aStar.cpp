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

/*
Below are the utility functions and global variables of this program.
*/

vvi solvedGrid(4, vi(4)); //solved grid. Used for some functions below.

void mountSolvedGrid(){ //mount the solved grid. Used at the start of the program
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
	return make_pair(-1, -1); //not found
}

/*
is solvable if:
	(1) the blank is on an even row counting from the bottom (second-last, fourth-last, etc.) and number of inversions is odd.
	or
	(2) the blank is on an odd row counting from the bottom (last, third-last, fifth-last, etc.) and number of inversions is even.
else not solvable
*/
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

bool isSolved(vvi &grid){ //Compares grid with a solved one. Returns true only if the two grids are equal.
	for(int i=0;i<4;i++)
		for(int j=0;j<4;j++)
			if(grid[i][j] != solvedGrid[i][j]) return false;
	return true;
}

int manhattan(vvi grid){ //priority function for A* approach.
	int h = 0;
	for(int i = 1; i<16; i++){ //Doesn't need to count the 0's distances.
		ii a = findPosition(grid, i), b = findPosition(solvedGrid, i);
		h += abs(a.first - b.first) + abs(a.second - b.second);
	}
	return h; //returns the sum of the distances (sum of the vertical and horizontal distance) from the blocks to their goal positions
}

/*
Below are the functions and classes used to calculate the solution.
*/

//A state of the puzzle.
struct State{
	vvi grid; // the current grid
	int f; // The result of the heuristic, calculated at the constructor. Used in the priority_queue
	string path; // The path from the initial state to this one.
	State(vvi grid, string path) : grid(grid), path(path){ f = manhattan(grid) + path.size();} //constructor
	bool operator>(const State &b) const{ return f > b.f;} //operator overload for the priority_queue
	char prev(){ // The previous state direction, that is, "From the previous state, we went to R, L, D or U?"
		if(path.size()) return *(path.end()-1);
		else return 0;
	} 
};

//priority_queue used to solve the problem.
typedef priority_queue<State, vector<State>, greater<State> > pqs;

//Solve the solution with an A* algorithm, with the manhattan priority function
/*
Complexity:

*Be n the number of moves from the initial state to the solution state.

Worst-case:
	S(n) = 3*S(n-1) + C
	S(0) = C

	S(n-1) = 3*S(n-2) + C
		=> S(n) = 3*(3*S(n-2) + C) + C = 3^2 * S(n-2) + (3^0 + 3^1)*C
	S(n-2) = 3*S(n-3) + C
		=> S(n) = 3^2*(3*S(n-3)+c) + (3^0 + 3^1)*C = 3^3*S(n-3) + (3^0 + 3^1 + 3^2)C
	By mathematical induction:
		S(n) = 3^(n+1)*S(0) + sum(i=0..n)(3^i) * C
		S(n) = 1/2 * C * (3^(n+1) - 1)
	Therefore, we have the following complexity:
		O(3^n)

Best-case:
	If the heuristic makes the best decision making to the solution "without backtracking", we have:
	S(n) = S(n-1) + C
	S(0) = C

	S(n-1) = S(n-2) + C
		=> S(n) = S(n-2) + 2C
	S(n-2) = S(n-3) + C
		=> S(n) = S(n-3) + 3C
	By mathematical induction:
		S(n) = S(0) + n*C = C + n*c
		=> S(n) = C*(n+1)
	Therefore, we have the following complexity:
		O(n)
*/
string solve(pqs &pq){
	while(!pq.empty()){
		State s = pq.top(); pq.pop(); //dequeue state
		if(isSolved(s.grid) && s.path.size() <= 50) return s.path; //if solved and path found is below 50 steps
		ii zero = findPosition(s.grid, 0);
		//up
		if(zero.first > 0 && s.prev() != 'D'){ //does not enqueue previous state
			vvi ng = s.grid; swap(ng[zero.first][zero.second], ng[zero.first-1][zero.second]); // "moves" blank
			State ns(ng, s.path + 'U'); // new state
			pq.push(ns); //enqueue new state
		}
		//down
		if(zero.first < 3 && s.prev() != 'U'){
			vvi ng = s.grid; swap(ng[zero.first][zero.second], ng[zero.first+1][zero.second]);
			State ns(ng, s.path + 'D');
			pq.push(ns);
		}
		//left
		if(zero.second > 0 && s.prev() != 'R'){
			vvi ng = s.grid; swap(ng[zero.first][zero.second], ng[zero.first][zero.second-1]);
			State ns(ng, s.path + 'L');
			pq.push(ns);
		}
		//right
		if(zero.second < 3 && s.prev() != 'L'){
			vvi ng = s.grid; swap(ng[zero.first][zero.second], ng[zero.first][zero.second+1]);
			State ns(ng, s.path + 'R');
			pq.push(ns);
		}
	}
	return "This puzzle is not solvable."; //Should not reach this point
}

int main(int argc, char** argv){
	mountSolvedGrid();
	int N; cin >> N; //number of sets
		clock_t totalT=0; //total processing time
	while (N--) {
		//input of each set
		vvi grid(4, vi(4));
		for (int i = 0; i < 4; i++)
			for (int j = 0; j < 4; j++)
				cin >> grid[i][j];
		//Start timer
		clock_t t = clock();
		if(!isSolvable(grid)) cout << "This puzzle is not solvable." << endl; //not solvable
		else if(isSolved(grid)) cout << "" << endl; //already solved
		else{
			State initial(grid, "");
			pqs pq; pq.push(initial);
			string solution = solve(pq); //get solution
			cout << solution << endl; //print solution, if found
		}
		//End timer and print time of this case
		t = clock() - t;
		cout << endl << "Processing time of current case: " << setiosflags(ios::fixed) << setprecision(6) << ((double)t)/CLOCKS_PER_SEC << "s" << endl;
		totalT += t;
	}
	//Print total processing time
	cout << endl << "Total processing time: " << setiosflags(ios::fixed) << setprecision(6) << ((double)totalT)/CLOCKS_PER_SEC << "s" << endl;
	return 0;
}