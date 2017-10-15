/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   main.cpp
 * Author: andre
 *
 * Created on 15 de Outubro de 2017, 18:32
 */

#include <iostream>
#include <vector>

using namespace std;
typedef vector<int> vi;
typedef vector<vi> vvi;

/*
 * Main function. Controls I/O.
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

