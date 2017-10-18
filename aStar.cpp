#include <iostream>
#include <vector>

using namespace std;
typedef vector<int> vi;
typedef vector<vi> vvi;

bool is_solved(vvi &grid){
	int solvedGrid[4][4] = {{1, 2, 3, 4}, {5, 6, 7, 8}, {9, 10, 11, 12}, {13, 14, 15, 0}};
	for(int i=0;i<4;i++)
		for(int j=0;j<4;j++)
			if(grid[i][j] != solvedGrid[i][j]) return false;
	return true;
}

int main(int argc, char** argv) {
	int N;
	cin >> N;
	while (N--) {
		vvi grid(4, vi(4)); //input of each set
		for (int i = 0; i < 4; i++)
			for (int j = 0; j < 4; j++)
				cin >> grid[i][j];
		
		
	}
	return 0;
}