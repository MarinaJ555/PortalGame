#include "map_segment.hpp"
using namespace std;

char MapSegment::VERTICAL_BORDER_CHAR = '|';
char MapSegment::HORIZONTAL_BORDER_CHAR = '=';
char MapSegment::PORTAL_CHAR = '@';



MapSegment::MapSegment(int height, int width)
    : mHeight(height), mWidth(width)
    , mPortalY{(mHeight - 1) / 2}
    , mPortalX{(mWidth - 1) / 2}
{
  direction_char = 'v';
	//construct an empty map segment given the height and width
	for(int i = 0; i < height; i ++){
    string row = "";
		for(int j = 0; j < width; j ++){
			if(i == 0 || i == height - 1){
				row += HORIZONTAL_BORDER_CHAR;
			}
			else if(j == 0 || j == width -1){
				row += VERTICAL_BORDER_CHAR;
			}
			else{
				row += ' ';
			}
		}
    map_segment.push_back(row);

	}
  
}

MapSegment::~MapSegment(){

}

void MapSegment::addBuilding(Building b){
	//draw the building onto the map segment here
	int building_lower_bound = b.get_y() + b.get_height();
	int building_right_bound = b.get_x() + b.get_width();
  if(building_lower_bound > getHeight()){ //if building goes outside bounds of map
     building_lower_bound = getHeight() - 1;
  }
  if(building_right_bound > getWidth()){
    building_right_bound = getWidth() - 1;
  }
  //figure out where the door starts (uppermost left coordinates):
	int door_y_coordinate = b.get_y() + 2;
	int door_x_coordinate = b.get_x() + 2;


	for(int i = b.get_y(); i < building_lower_bound; i ++){
		for(int j = b.get_x(); j < building_right_bound; j ++){
			if(i == door_y_coordinate && j == door_x_coordinate){
				//add the top half of the door (&&)
				map_segment[i][j] = '&';
				map_segment[i][j + 1] = '&';
				j = j + 1;
			}
			else if(i == door_y_coordinate + 1 && j == door_x_coordinate){
				//add the bottom half of the door (&&)
				map_segment[i][j] = '&';
				map_segment[i][j + 1] = '&';
				j = j + 1;
			}
			else{
				map_segment[i][j] = '.';
			}

		}
	}
}

void MapSegment::addItem(Item i){
	//add item to the map segment here
	map_segment[i.getY()][i.getX()] = i.ICON;//Icon is $
}

void MapSegment::addPlayer(int y_coordinate, int x_coordinate, string direction){
  if(direction == "down"){
     direction_char = 'v';
     
  }
  else if(direction == "up"){
     direction_char = '^';
  }
  else if(direction == "left"){
    direction_char = '<';
  }
  else{
    direction_char = '>';
  }
  map_segment[y_coordinate][x_coordinate] = direction_char;

}

char MapSegment::get_direction_char(){
  return direction_char;
}

void MapSegment::set_direction_char(char direction){
	direction_char = direction;
}
vector<string> MapSegment::get_map_segment(){
  return map_segment;
}

int MapSegment::get_id(){
  return id; 
}

void MapSegment::set_id(int new_id) {
  id = new_id;
}



bool MapSegment::check_item(int y_coordinate, int x_coordinate){
	Item i(y_coordinate, x_coordinate); //create an item but don't add it to the map
	bool is_item = false;
	if(map_segment[y_coordinate][x_coordinate] == i.ICON){ //it exists at those coordinates in the map
		is_item = true;
	}
	return is_item;
}

bool MapSegment::check_walls(int y_coordinate, int x_coordinate){
	bool can_move = true;
	char target = map_segment[y_coordinate][x_coordinate]; //the map location in question
	//check for buildings or walls:
	if(target == '.' || target == '&' || target == VERTICAL_BORDER_CHAR || target == HORIZONTAL_BORDER_CHAR || target == PORTAL_CHAR){
		can_move = false;
	}
	return can_move;
}

bool MapSegment::check_portals(int y_coordinate, int x_coordinate){
	bool is_portal = false;
	if(map_segment[y_coordinate][x_coordinate] == PORTAL_CHAR){
		is_portal = true;
	}
	return is_portal;
}

string MapSegment::find_side(int y_coordinate, int x_coordinate){
  string side;
  if(y_coordinate == 0){
    side = "top";
  }
  else if(y_coordinate == mHeight){
    side = "bottom";
  }
  else if(x_coordinate == 0){
    side = "left";
  }
  else{
    side = "right";
  }
  return side;
}

void MapSegment::set_coords(string side, int& x_coord, int& y_coord){
  if(side == "right"){ //place portal in the middle of rightmost side
		y_coord = mPortalY;
		x_coord = mWidth - 1;
	}
	else if(side == "left"){
		y_coord = mPortalY; //halfway down the column
		x_coord = 0; //should appear in first column
	}
	else if(side == "down"){
		y_coord = mHeight - 1;
		x_coord = mPortalX;
	}
	else if(side == "up"){
		y_coord = 0;
		x_coord = mPortalX;
	}
}


void MapSegment::addPortal(string side){
	//add portal char to middle of indicated side of map segment
	//store the map segment index/side it's connected to
	//should I store the portal coordinates somewhere? like in the portal class?
	int y_coordinate;
	int x_coordinate;
  
  set_coords(side, x_coordinate, y_coordinate);
  map_segment[y_coordinate][x_coordinate] = PORTAL_CHAR;
}


