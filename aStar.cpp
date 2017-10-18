/*
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
 */
#include <bits/stdc++.h>
using namespace std;

const int dr[4] = {0, 0, +1, -1};
const int dc[4] = {+1, -1, 0, 0};
const int dir[4] = {'R', 'L', 'D', 'U'};
const int INF = 0x3f3f3f3f;
const int FOUND = -1;
vector<char> path;
int A[15][15], Er, Ec;

int H() {
    int h = 0;
    for (int r = 0; r < 4; r++) {
        for (int c = 0; c < 4; c++) {
            if (A[r][c] == 0) continue;
            int expect_r = (A[r][c] - 1) / 4;
            int expect_c = (A[r][c] - 1) % 4;
            h += abs(expect_r - r) + abs(expect_c - c);
        }
    }
    return h;
}

int dfs(int g, int pdir, int bound) {
    int h = H();
    int f = g + h;
    if (f > bound) return f;
    if (h == 0) return FOUND;

    int mn = INF;
    for (int i = 0; i < 4; i++) {
        if (i == (pdir ^ 1)) continue;

        int nr = Er + dr[i];
        int nc = Ec + dc[i];
        if (nr < 0 || nr >= 4) continue;
        if (nc < 0 || nc >= 4) continue;

        path.push_back(dir[i]);
        swap(A[nr][nc], A[Er][Ec]);
        swap(nr, Er);
        swap(nc, Ec);
        int t = dfs(g + 1, i, bound);
        if (t == FOUND) return FOUND;
        if (t < mn) mn = t;
        swap(nr, Er);
        swap(nc, Ec);
        swap(A[nr][nc], A[Er][Ec]);
        path.pop_back();
    }

    return mn;
}

bool IDAstar() {
    int bound = H();
    for (;;) {
        int t = dfs(0, -1, bound);
        if (t == FOUND) return true;
        if (t == INF) return false;
        // 下次要搜的 bound >= 50，真的解也一定 >= 50，剪枝
        if (t >= 50) return false;
        bound = t;
    }
    return false;
}

bool solvable() {
    int cnt = 0;
    bool occur[16] = {false};
    for (int r = 0; r < 4; r++) {
        for (int c = 0; c < 4; c++) {
            if (A[r][c] == 0) {
                Er = r;
                Ec = c;
            } else {
                cnt += count(occur + 1, occur + A[r][c], false);
                occur[A[r][c]] = true;
            }
        }
    }
    return (cnt + (Er + 1)) % 2 == 0;
}

int main(int argc, char **argv) {
    int TC;
    scanf("%d", &TC);
    while (TC--) {
        for (int r = 0; r < 4; r++) {
            for (int c = 0; c < 4; c++) {
                scanf("%d", &A[r][c]);
            }
        }

        path.clear();

        if (!solvable() || !IDAstar()) {
            puts("This puzzle is not solvable.");
        } else {
            for (char c : path) {
                printf("%c", c);
            }
            puts("");
        }
    }

    return 0;
}