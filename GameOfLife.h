#pragma once

#include <cstdlib>
#include <ctime>

class GameOfLife {
private:
	bool** grid;
    bool** tempGrid;
    
    int *xdomain;
    int *ydomain;
    
	int width;
	int height;
    int countNeighbors(int, int);
public:
    GameOfLife(int w, int h);
	~GameOfLife();
	void randomInit(double probability = 0.1);
	void iterate();
    bool organismAt(int x, int y);
};
