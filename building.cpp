#include "building.hpp"

const int Building::width = 6;
const int Building::height = 4;
using namespace std;

Building::Building(int y, int x)
    : y_coordinate(y), x_coordinate(x)
{
}

int Building::get_width(){
  return width;
}

int Building::get_height(){
  return height;
}

int Building::get_x(){
  return x_coordinate;
}

int Building::get_y(){
  return y_coordinate;
}
