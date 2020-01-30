#include "dsets.h"
#include "maze.h"
#include <iostream>
#include <vector>
#include <queue>

//default constructor
SquareMaze::SquareMaze(){

}

//make a new maze with no cycles and it is randomized
void SquareMaze::makeMaze(int width, int height){
  
  width_ = 0;
  height_ = 0;
  grid.resize(0);
  //the array is cleared

  width_ = width;
  height_ = height;
  grid.resize(width_);
  for(int i=0; i<width_; i++){
    grid[i].resize(height_);
  }
  //now we have our CELLS
  for(int i = 0; i<width; i++){
    for(int j = 0; j<height; j++){
      setWall(i,j,0,true);
      setWall(i,j,1,true);
    }
  }
  DisjointSets cycleChecker;
  cycleChecker.addelements(width_*height_);
  //srand(time(NULL));
  int numOfRemovals = 0;
  int maxRemovals = (width_*height_)-1;
  while(numOfRemovals!=maxRemovals){
    int x_pos = rand()%width_;
    int y_pos = rand()%height_;
    int dir = rand()%2;
    int disjointSetVal = (y_pos*width_+x_pos);
    if((y_pos==height-1&&dir==1)||(x_pos==width_-1&&dir==0)){
      continue;
    }
    if(dir==0){
      //check for right and make sure it is not in the same setunion
      if(grid[x_pos][y_pos].first==false){
        continue;
      }
      if(cycleChecker.find(disjointSetVal)==cycleChecker.find(disjointSetVal+1)){
        continue;
      }
      //now we know they are not connected
      setWall(x_pos,y_pos,0,false);
      cycleChecker.setunion(disjointSetVal, disjointSetVal+1);
      numOfRemovals++;
    }

    if(dir==1){
      //check for right and make sure it is not in the same setunion
      if(grid[x_pos][y_pos].second==false){
        continue;
      }
      if(cycleChecker.find(disjointSetVal)==cycleChecker.find(disjointSetVal+width_)){
        continue;
      }
      //now we know they are not connected
      setWall(x_pos,y_pos,1,false);
      cycleChecker.setunion(disjointSetVal, disjointSetVal+width_);
      numOfRemovals++;
    }

  }



}

//is it possible to travel in the direction specified at the given location?
//0=right
//1=down
//2=left
//3=up
//cant go through walls or edges
//used for solving the maze and the grading program
//return 1 if we can travel, 0 if no

//checks if you can travel form your current cell to the one in the direction you choose
bool SquareMaze::canTravel(int x, int y, int dir){
  //plan
  //check if the x,y coordinate is on an  edge
  //then we do special cases just for movin gthrough boundaries, which we cant
  //if not, then if up or left, then move to that cell and check the boundary
  if(dir==0){
    if(x==width_-1){
      return false;
    }
    else if(grid[x][y].first==true){
      return false;
    }
    else{
      return true;
    }
  }

  if(dir==1){
    if(y==height_-1){
      return false;
    }
    else if(grid[x][y].second==true){
      return false;
    }
    else{
      return true;
    }
  }

  if(dir==2){
    if(x==0){
      return false;
    }
    else if(grid[x-1][y].first==true){
      return false;
    }
    else{
      return true;
    }
  }

  if(dir==3){
    if(y==0){
      return false;
    }
    else if(grid[x][y-1].second==true){
      return false;
    }
    else{
      return true;
    }
  }
  return false;
}

//will change if the wall exists or not
//must be fast (constant) -> array
//dir->>
//0 is right
//1 is down
//only need bottom and right
//exists-> true if we want a wall, false if we want to remove it

//adds or removes a wall in the direction you choose at the cell you input
void SquareMaze::setWall(int x, int y, int dir, bool exists){
  //plan
  //access the element in the x,y,array
  //get that direciton
  //if true, create a walls
  //if false, destroy the wall
  if(x+1==width_&&dir==0){
    return;
  }
  if(y+1==height_&&dir==1){
    return;
  }
  if(dir==0){
    grid[x][y].first = exists;
  }
  if(dir==1){
    grid[x][y].second = exists;
  }
  return;
}

//solves the square maze
//Need to go from top right to bottom
//the cell at on the bottom row which is the furthest distance away from (0,0) - top left
//BFS the whole maze and label every point with a distance and chose the furthest distance away
//if two have the smallest distance, choose the smallest x value, so the leftmost
//returns a vector for the path they took to that cell
//0 is right
//1 is down
//2 is leftmost
//3 is up

//solevs the maze and gives out a vector of the path it had used.
vector<int> SquareMaze::solveMaze(){
  vector<int> paths;

  queue<int> qx;
  queue<int> qy;
  vector<vector<int>> back;
  vector<vector<int>> distance;
  vector<vector<bool>> visited;

  back.resize(width_);
  distance.resize(width_);
  visited.resize(width_);

  for(int i = 0; i<width_; i++){
    back[i].resize(height_);
    distance[i].resize(height_);
    visited[i].resize(height_);
  }

  int curr_x = 0;
  int curr_y = 0;


  back[0][0] = -1;
  distance[0][0] = 0;
  visited[0][0] = true;

  qx.push(curr_x);
  qy.push(curr_y);
  while(qx.empty()!=1&&qy.empty()!=1){
    curr_x = qx.front();
    curr_y = qy.front();
    qx.pop();
    qy.pop();
    visited[curr_x][curr_y] = true;
    //time to push the nearest values as long as they havae never been visited
    //max 4 values to check
    //special cases for corners
    //then edges
    //then everything else



    //TOP LEFT CORNER
    if(curr_x==0&&curr_y==0){
      //only right and DOWN


      //right case
      if(canTravel(curr_x, curr_y, 0)){
        //can go RIGHT
        if(visited[curr_x+1][curr_y]==false){
          qx.push(curr_x+1);
          qy.push(curr_y);
          distance[curr_x+1][curr_y] = distance[curr_x][curr_y]+1;
          back[curr_x+1][curr_y] = 0;
        }
      }

      //down case
      if(canTravel(curr_x, curr_y, 1)){
        //can go DOWN
        if(visited[curr_x][curr_y+1]==false){
          qx.push(curr_x);
          qy.push(curr_y+1);
          distance[curr_x][curr_y+1] = distance[curr_x][curr_y]+1;
          back[curr_x][curr_y+1] = 1;
        }
      }
    }

    //TOP RIGHT CORNER
    else if(curr_x==width_-1&&curr_y==0){
      //only left and DOWN
      //left case
      if(canTravel(curr_x, curr_y, 2)){
        if(visited[curr_x-1][curr_y]==false){
          qx.push(curr_x-1);
          qy.push(curr_y);
          distance[curr_x-1][curr_y] = distance[curr_x][curr_y]+1;
          back[curr_x-1][curr_y] = 2;
        }
      }
      //down case
      if(canTravel(curr_x,curr_y, 1)){
        if(visited[curr_x][curr_y+1]==false){
          qx.push(curr_x);
          qy.push(curr_y+1);
          distance[curr_x][curr_y+1] = distance[curr_x][curr_y]+1;
          back[curr_x][curr_y+1] = 1;
        }
      }
    }

    //BOTTOM LEFT CORNER
    else if(curr_x==0&&curr_y==height_-1){
      //only up and right
      //up case
      if(canTravel(curr_x, curr_y, 3)){
        if(visited[curr_x][curr_y-1]==false){
          qx.push(curr_x);
          qy.push(curr_y-1);
          distance[curr_x][curr_y-1] = distance[curr_x][curr_y]+1;
          back[curr_x][curr_y-1] = 3;
        }
      }
      //right case
      if(canTravel(curr_x, curr_y, 0)){
        //can go RIGHT
        if(visited[curr_x+1][curr_y]==false){
          qx.push(curr_x+1);
          qy.push(curr_y);
          distance[curr_x+1][curr_y] = distance[curr_x][curr_y]+1;
          back[curr_x+1][curr_y] = 0;
        }
      }
    }

    //BOTTOM RIGHT CORNER
    else if(curr_x==width_-1&&curr_y==height_-1){
      //up and left
      //up case
      if(canTravel(curr_x, curr_y, 3)){
        if(visited[curr_x][curr_y-1]==false){
          qx.push(curr_x);
          qy.push(curr_y-1);
          distance[curr_x][curr_y-1] = distance[curr_x][curr_y]+1;
          back[curr_x][curr_y-1] = 3;
        }
      }
      //left case
      if(canTravel(curr_x, curr_y, 2)){
        if(visited[curr_x-1][curr_y]==false){
          qx.push(curr_x-1);
          qy.push(curr_y);
          distance[curr_x-1][curr_y] = distance[curr_x][curr_y]+1;
          back[curr_x-1][curr_y] = 2;
        }
      }
    }

    //LEFT EDGE
    else if(curr_x==0){
      //up, right, down
      //right case
      if(canTravel(curr_x, curr_y, 0)){
        //can go RIGHT
        if(visited[curr_x+1][curr_y]==false){
          qx.push(curr_x+1);
          qy.push(curr_y);
          distance[curr_x+1][curr_y] = distance[curr_x][curr_y]+1;
          back[curr_x+1][curr_y] = 0;
        }
      }
      //up case
      if(canTravel(curr_x, curr_y, 3)){
        if(visited[curr_x][curr_y-1]==false){
          qx.push(curr_x);
          qy.push(curr_y-1);
          distance[curr_x][curr_y-1] = distance[curr_x][curr_y]+1;
          back[curr_x][curr_y-1] = 3;
        }
      }
      //down case
      if(canTravel(curr_x,curr_y, 1)){
        if(visited[curr_x][curr_y+1]==false){
          qx.push(curr_x);
          qy.push(curr_y+1);
          distance[curr_x][curr_y+1] = distance[curr_x][curr_y]+1;
          back[curr_x][curr_y+1] = 1;
        }
      }
    }

    //TOP EDGE
    else if(curr_y==0){
      //left, down, right
      //right case
      if(canTravel(curr_x, curr_y, 0)){
        //can go RIGHT
        if(visited[curr_x+1][curr_y]==false){
          qx.push(curr_x+1);
          qy.push(curr_y);
          distance[curr_x+1][curr_y] = distance[curr_x][curr_y]+1;
          back[curr_x+1][curr_y] = 0;
        }
      }
      //down case
      if(canTravel(curr_x,curr_y, 1)){
        if(visited[curr_x][curr_y+1]==false){
          qx.push(curr_x);
          qy.push(curr_y+1);
          distance[curr_x][curr_y+1] = distance[curr_x][curr_y]+1;
          back[curr_x][curr_y+1] = 1;
        }
      }
      //left case
      if(canTravel(curr_x, curr_y, 2)){
        if(visited[curr_x-1][curr_y]==false){
          qx.push(curr_x-1);
          qy.push(curr_y);
          distance[curr_x-1][curr_y] = distance[curr_x][curr_y]+1;
          back[curr_x-1][curr_y] = 2;
        }
      }
    }

    //RIGHT EDGE
    else if(curr_x==width_-1){
      //up, left, down
      //up case
      if(canTravel(curr_x, curr_y, 3)){
        if(visited[curr_x][curr_y-1]==false){
          qx.push(curr_x);
          qy.push(curr_y-1);
          distance[curr_x][curr_y-1] = distance[curr_x][curr_y]+1;
          back[curr_x][curr_y-1] = 3;
        }
      }
      //down case
      if(canTravel(curr_x,curr_y, 1)){
        if(visited[curr_x][curr_y+1]==false){
          qx.push(curr_x);
          qy.push(curr_y+1);
          distance[curr_x][curr_y+1] = distance[curr_x][curr_y]+1;
          back[curr_x][curr_y+1] = 1;
        }
      }
      //left case
      if(canTravel(curr_x, curr_y, 2)){
        if(visited[curr_x-1][curr_y]==false){
          qx.push(curr_x-1);
          qy.push(curr_y);
          distance[curr_x-1][curr_y] = distance[curr_x][curr_y]+1;
          back[curr_x-1][curr_y] = 2;
        }
      }
    }

    //BOTTOM EDGE
    else if(curr_y==height_-1){
      //up, left, right
      //right case
      if(canTravel(curr_x, curr_y, 0)){
        //can go RIGHT
        if(visited[curr_x+1][curr_y]==false){
          qx.push(curr_x+1);
          qy.push(curr_y);
          distance[curr_x+1][curr_y] = distance[curr_x][curr_y]+1;
          back[curr_x+1][curr_y] = 0;
        }
      }
      //up case
      if(canTravel(curr_x, curr_y, 3)){
        if(visited[curr_x][curr_y-1]==false){
          qx.push(curr_x);
          qy.push(curr_y-1);
          distance[curr_x][curr_y-1] = distance[curr_x][curr_y]+1;
          back[curr_x][curr_y-1] = 3;
        }
      }
      //left case
      if(canTravel(curr_x, curr_y, 2)){
        if(visited[curr_x-1][curr_y]==false){
          qx.push(curr_x-1);
          qy.push(curr_y);
          distance[curr_x-1][curr_y] = distance[curr_x][curr_y]+1;
          back[curr_x-1][curr_y] = 2;
        }
      }
    }

    //MIDDLE
    else{
      //up, down, left, right
      //right case
      if(canTravel(curr_x, curr_y, 0)){
        //can go RIGHT
        if(visited[curr_x+1][curr_y]==false){
          qx.push(curr_x+1);
          qy.push(curr_y);
          distance[curr_x+1][curr_y] = distance[curr_x][curr_y]+1;
          back[curr_x+1][curr_y] = 0;
        }
      }
      //up case
      if(canTravel(curr_x, curr_y, 3)){
        if(visited[curr_x][curr_y-1]==false){
          qx.push(curr_x);
          qy.push(curr_y-1);
          distance[curr_x][curr_y-1] = distance[curr_x][curr_y]+1;
          back[curr_x][curr_y-1] = 3;
        }
      }
      //down case
      if(canTravel(curr_x,curr_y, 1)){
        if(visited[curr_x][curr_y+1]==false){
          qx.push(curr_x);
          qy.push(curr_y+1);
          distance[curr_x][curr_y+1] = distance[curr_x][curr_y]+1;
          back[curr_x][curr_y+1] = 1;
        }
      }
      //left case
      if(canTravel(curr_x, curr_y, 2)){
        if(visited[curr_x-1][curr_y]==false){
          qx.push(curr_x-1);
          qy.push(curr_y);
          distance[curr_x-1][curr_y] = distance[curr_x][curr_y]+1;
          back[curr_x-1][curr_y] = 2;
        }
      }
    }

  }//end of while loop



  //I have all the data I need to compile a path list
  //first, search the bottom row for the longest path

  int longest_distance = 0;
  int x_position;
  for(int i = 0; i<width_; i++){
    if(distance[i][height_-1]>longest_distance){
      x_position=i;
      longest_distance = distance[i][height_-1];
    }
  }

  //now we have the exact endpoint;
  int prev_x_pos = x_position;
  int prev_y_pos = height_-1;
  vector<int> traceback;
  while(back[prev_x_pos][prev_y_pos]!=-1){
    traceback.push_back(back[prev_x_pos][prev_y_pos]);
    int movement = back[prev_x_pos][prev_y_pos];
    if(movement==0){
      prev_x_pos--;
    }
    if(movement==1){
      prev_y_pos--;
    }
    if(movement==2){
      prev_x_pos++;
    }
    if(movement==3){
      prev_y_pos++;
    }
  }
  //traceback should be good, but in reverse order
  size_t tracebackSize = traceback.size();
  for(size_t i = 0; i<tracebackSize; i++){
    paths.push_back(traceback.back());
    traceback.pop_back();
  }

  return paths;
}

//Draws the maze without the solution
//create a new PNG
//set dimensions to -> (width*10+1, height*10+1) where height and width are what was given
//make the entire top and left black
//NOT THE ENTRANCE AT (1,0) through (9,0) in pixel dimension
//for each cell, call it and check if you have walls.
//if you have a right wall -> blacken pixels ((x+1)*10,y*10+k) where k is from 0 to 10
//if you have a bottom wall -> blacken pixels (x*10+k, (y+1)*10) where k is from 0 to 10


//Outputs a PNG of the maze you have created
PNG * SquareMaze::drawMaze() const{
  PNG * temp = new PNG((width_*10)+1, (height_*10)+1);

  for(unsigned x = 0; x<temp->width(); x++){
    for(unsigned y = 0; y<temp->height();y++){
      HSLAPixel * pixel = temp->getPixel(x,y);
      if(x==0||y==0||x==temp->width()-1||y==temp->height()-1){
        pixel->h = 0;
        pixel->s = 0;
        pixel->l = 0;
        pixel->a = 1;
      }
      else{
        pixel->h = 0;
        pixel->s = 0;
        pixel->l = 1;
        pixel->a = 1;
      }
    }
  }
  for(int i = 0; i<width_; i++){
    for(int j = 0; j<height_; j++){
      if(grid[i][j].first==true){
        for(int k = 0; k<11; k++){
          HSLAPixel * pixel = temp->getPixel((i+1)*10,j*10+k);
          pixel->h = 0;
          pixel->s = 0;
          pixel->l = 0;
          pixel->a = 1;
        }
      }
      if(grid[i][j].second==true){
        for(int k = 0; k<11; k++){
          HSLAPixel * pixel = temp->getPixel(i*10+k, (j+1)*10);
          pixel->h = 0;
          pixel->s = 0;
          pixel->l = 0;
          pixel->a = 1;
        }
      }
    }
  }
  for(int i = 1; i<10; i++){
    HSLAPixel * pixel = temp->getPixel(i,0);
    pixel->h = 0;
    pixel->s = 0;
    pixel->l = 1;
    pixel->a = 1;
  }







  // plan
  // create that png
  // resize it or just create the png to that SIZE
  // make it all white
  // make the edges black
  // remove the black from the ENTRANCE
  // go through every cell and make a black splotch if there is a wall
  return temp;
}

//Calls drawMaze
//calls solveMaze
//edits the png from drawMaze
//start at pixel (5,5)
//the vector you use should signify 11 red pixels in the direction of the vector
//if first you go down, color (5,5) through (5,15) red
//red is (0,1,.5,1) HSLA
//right (5,15) to (15,15)
//up (15,15) to (15,5)
//do this till you get to the end of your solution vector
//make an exti
//remove the bottom wall of the final cell by whitining those pixels
//coordinates (x*10+k, (y+1)*10) for k from 1 to 9
//return that png

//outputs a png of the maze and the solution to that maze
PNG * SquareMaze::drawMazeWithSolution(){
  PNG * temp = drawMaze();
  vector<int> path = solveMaze();
  int curr_x = 5;
  int curr_y = 5;
  for(size_t i = 0; i<path.size(); i++){
    if(path[i]==0){
      //right
      for(int j = 0; j<11; j++){
        //color 11 pixels
        if(j==10){
          HSLAPixel * HSLAtemp = temp->getPixel(curr_x+j, curr_y);
          HSLAtemp->h = 0;
          HSLAtemp->s = 1;
          HSLAtemp->l = .5;
          HSLAtemp->a = 1;
          curr_x = curr_x+10;
        }
        else{
          HSLAPixel * HSLAtemp = temp->getPixel(curr_x+j, curr_y);
          HSLAtemp->h = 0;
          HSLAtemp->s = 1;
          HSLAtemp->l = .5;
          HSLAtemp->a = 1;
        }
      }
    }
    if(path[i]==1){
      //down
      for(int j = 0; j<11; j++){
        //color 11 pixels
        if(j==10){
          HSLAPixel * HSLAtemp = temp->getPixel(curr_x, curr_y+j);
          HSLAtemp->h = 0;
          HSLAtemp->s = 1;
          HSLAtemp->l = .5;
          HSLAtemp->a = 1;
          curr_y = curr_y+10;
        }
        else{
          HSLAPixel * HSLAtemp = temp->getPixel(curr_x, curr_y+j);
          HSLAtemp->h = 0;
          HSLAtemp->s = 1;
          HSLAtemp->l = .5;
          HSLAtemp->a = 1;
        }
      }
    }
    if(path[i]==2){
      //left
      for(int j = 0; j<11; j++){
        //color 11 pixels
        if(j==10){
          HSLAPixel * HSLAtemp = temp->getPixel(curr_x-j, curr_y);
          HSLAtemp->h = 0;
          HSLAtemp->s = 1;
          HSLAtemp->l = .5;
          HSLAtemp->a = 1;
          curr_x = curr_x-10;
        }
        else{
          HSLAPixel * HSLAtemp = temp->getPixel(curr_x-j, curr_y);
          HSLAtemp->h = 0;
          HSLAtemp->s = 1;
          HSLAtemp->l = .5;
          HSLAtemp->a = 1;
        }
      }
    }
    if(path[i]==3){
      //up
      for(int j = 0; j<11; j++){
        //color 11 pixels
        if(j==10){
          HSLAPixel * HSLAtemp = temp->getPixel(curr_x, curr_y-j);
          HSLAtemp->h = 0;
          HSLAtemp->s = 1;
          HSLAtemp->l = .5;
          HSLAtemp->a = 1;
          curr_y = curr_y-10;
        }
        else{
          HSLAPixel * HSLAtemp = temp->getPixel(curr_x, curr_y-j);
          HSLAtemp->h = 0;
          HSLAtemp->s = 1;
          HSLAtemp->l = .5;
          HSLAtemp->a = 1;
        }
      }
    }
  }
  curr_y = curr_y+5;
  for(int i = -4; i<5; i++){
    HSLAPixel * HSLAtemp =temp->getPixel(curr_x+i,curr_y);
    HSLAtemp->h = 0;
    HSLAtemp->s = 0;
    HSLAtemp->l = 1;
    HSLAtemp->a = 1;
  }

  //plan
  //call drawMaze
  //call solve MAZE
  //use the png form draw maze, and add red splotches
  //start at the pixel point 5,5
  //check the direction, travel in that direciton, marking the pixels red
  //do this till you get to the end
  //that last cell, create an exit
  //remove those black pixels
  return temp;
}
