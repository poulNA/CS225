/* Your code here! */
#pragma once
#include <vector>
#include <queue>
#include "cs225/PNG.h"
#include "dsets.h"

using namespace std;
using namespace cs225;

class SquareMaze {
    public:
        SquareMaze();
        void makeMaze(int width, int height);
        bool canTravel(int x, int y, int dir) const;
        void setWall(int x, int y, int dir, bool exists);
        vector<int> solveMaze();
        PNG * drawMaze() const;
        PNG * drawMazeWithSolution();
    
    private:
        DisjointSets sets;
        int width_;
        int height_;
        vector<bool> rightWalls;
        vector<bool> downWalls;
};