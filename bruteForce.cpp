#include <iostream>
#include <vector>

using namespace std;
typedef vector<int> vi;
typedef vector<vi> vvi;

/*
 * 
 */
int main(int argc, char** argv) {
    int N; cin >> N;
    while(N--){
        vvi grid(4, vi(4));
        for(int i=0;i<4;i++)
            for(int j=0;j<4;j++)
                cin >> grid[i][j];
    }
    return 0;
}

