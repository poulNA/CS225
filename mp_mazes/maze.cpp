/* Your code here! */
#include "maze.h"
#include <time.h>
#include <random>
#include <map>
#include <stack>


SquareMaze::SquareMaze() {}

void SquareMaze::makeMaze(int width, int height) {
    // if (mazes != NULL) {delete mazes;}
    width_ = width;
    height_ = height;
    int size = width * height;

    rightWalls = vector<bool> (size, true);
    downWalls = vector<bool> (size, true);

    // Set dsets
    sets.addelements(size);

    // Random number generator for cell and wall
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> distrib(0, size-1);
    std::uniform_int_distribution<> distribw(0, 1);

    // Loop
    int count = 0;
    while (count < size-1) {

        // Choose cell and wall
        int cell = distrib(gen);
        int walltype = distribw(gen);

        // Perimeter check
        if ((cell % width == width-1 && walltype == 0) || (cell / width == height-1 && walltype == 1)) {continue;}
        // Cycle check
        if (walltype == 0 && sets.find(cell) == sets.find(cell+1)) {continue;}
        if (walltype == 1 && sets.find(cell) == sets.find(cell+width)) {continue;}

        // delete wall
        setWall(cell % width, cell/width, walltype, false);
        count++;
    }
}

bool SquareMaze::canTravel(int x, int y, int dir) const {
    int pos = y * width_ + x;
    // Right
    if (dir == 0 && x < width_-1 && !rightWalls[pos]) {return true;}
    // Down
    if (dir == 1 && y < height_-1 && !downWalls[pos]) {return true;}
    // Left
    if (dir == 2 && 0 < x && !rightWalls[pos-1]) {return true;}
    // Up
    if (dir == 3 && 0 < y && !downWalls[pos-width_]) {return true;}
    return false;
}

void SquareMaze::setWall(int x, int y, int dir, bool exists) {
    int pos = y * width_ + x;

    if (dir == 0) {
        rightWalls[pos] = exists;
        sets.setunion(pos, pos+1);
    } else {
        downWalls[pos] = exists;
        sets.setunion(pos, pos + width_);
    }
}

vector<int> SquareMaze::solveMaze() {

    // BFS
    std::queue<int> qu;
    vector<bool> visited (width_ * height_, false);
    int v = 0;
    qu.push(v);

    std::map<int, int> map;

    while (!qu.empty()) {
        int curr = qu.front();
        qu.pop();  

        if (canTravel(curr%width_, curr/width_, 0) && !visited[curr+1]) {
            qu.push(curr+1);
            visited[curr+1] = true;
            map.insert({curr+1, curr});
        }
        if (canTravel(curr%width_, curr/width_, 1) && !visited[curr+width_]) {
            qu.push(curr+width_);
            visited[curr+width_] = true;
            map.insert({curr+width_, curr});
        }
        if (canTravel(curr%width_, curr/width_, 2) && !visited[curr-1]) {
            qu.push(curr-1);
            visited[curr-1] = true;
            map.insert({curr-1, curr});
        }
        if (canTravel(curr%width_, curr/width_, 3) && !visited[curr-width_]) {
            qu.push(curr-width_);
            visited[curr-width_] = true;
            map.insert({curr-width_, curr});
        }
    }

    // Backtrack
    vector<int> sol;
    stack<int> endsta;
    int dest = 0;
    int max = 0;

    for (int i = 0; i < width_; i++) {
        stack<int> sta;
        int count = 0;
        int curr = (height_ - 1)* width_ + i;

        while (curr != dest) {
            int prev = map[curr];
            if (curr-prev == 1) {sta.push(0);}
            if (curr-prev == width_) {sta.push(1);}
            if (curr-prev == -1) {sta.push(2);}
            if (curr-prev == -width_) {sta.push(3);}
            curr = map[curr];
            count++;
        }

        if (max < count) {
            endsta = sta;
            max = count;
        }
    }

    while (!endsta.empty()) {
        sol.push_back(endsta.top());
        endsta.pop();
    }

    return sol;
}

PNG * SquareMaze::drawMaze() const {
    PNG * img = new PNG(width_*10+1, height_*10+1);
    unsigned int size = width_ * height_;

    for (unsigned int x = 10; x < img->width(); x++) {
        img->getPixel(x, 0).l = 0;
    }
    for (unsigned int y = 0; y < img->height(); y++) {
        img->getPixel(0, y).l = 0;
    }

    for (unsigned int i = 0; i < size; i++) {
        unsigned int x = i%width_;
        unsigned int y = i/width_;

        if (rightWalls[i]) {
            for (int k = 0; k <= 10; k++) {
                img->getPixel((x+1)*10,y*10+k).l = 0;
            }
        }
        if (downWalls[i]) {
            for (int k = 0; k<= 10; k++) {
                img->getPixel(x*10+k, (y+1)*10).l = 0;
            }
        }
    }
    return img;
}

PNG * SquareMaze::drawMazeWithSolution() {
    PNG * img = drawMaze();
    vector<int> sol = solveMaze();
    HSLAPixel red = HSLAPixel(0,1,0.5,1);
    unsigned int x = 5;
    unsigned int y = 5;
    unsigned int newx = 5;
    unsigned int newy = 5;

    for (unsigned int i = 0; i < sol.size(); i++) {
        if (sol[i] == 0) {newx = x + 10;}
        if (sol[i] == 1) {newy = y + 10;}
        if (sol[i] == 2) {newx = x - 10;}
        if (sol[i] == 3) {newy = y - 10;}

        // Draw Red
        for (unsigned int j = 0; j <= 10; j++) {
            if (newx > x) {
                img->getPixel(x+j, y) = red;
            } else if (newx < x){
                img->getPixel(x-j, y) = red;
            } else if (newy > y) {
                img->getPixel(x, y+j) = red;
            } else if (newy < y) {
                img->getPixel(x, y-j) = red;
            }
        }

        x = newx;
        y = newy;
    }

    // Draw Exit
    for (int i = 1; i < 10; i++) {
        img->getPixel(x-5+i, y+5).l = 1;
    }
    return img;
}