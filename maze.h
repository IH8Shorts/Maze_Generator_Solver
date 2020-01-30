/* Your code here! */
#ifndef MAZE_H
#define MAZE_H

#include <iostream>
#include <vector>
#include "cs225/PNG.h"

using namespace cs225;
using namespace std;


class SquareMaze{
public:
  SquareMaze();
  void makeMaze(int width, int height);
  bool canTravel(int x, int y, int dir);
  void setWall(int x, int y, int dir, bool exists);
  vector<int> solveMaze();
  PNG * drawMaze() const;
  PNG * drawMazeWithSolution();
private:
  int width_;
  int height_;
  vector<vector<pair<bool, bool>>> grid; //TODO check for the syntax



};
#endif

/*Thoughts
make a 2d vector of Pairs of bool bottom and right to signify if there is a wall or not
we can store a disjoint set in this class too but we can also have it be a object we have to delete in the destructor
*/
