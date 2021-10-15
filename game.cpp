#include "game.hpp"
#include "curses_view.hpp"
#include "curses_controller.hpp"
#include "print_view.hpp"
#include "print_controller.hpp"
#include <iostream> //debugging only
#include <fstream>
#include <cstdio> //debugging purposes

using namespace std;

#define INVALID_GAME_FILE_MSG "Invalid game input file"
#define INVALID_LEVEL_FILE_MSG "Invalid level input file"
#define INVALID_VIEW_TYPE_MSG "Invalid view type"

const int Game::MIN_VIEW_HEIGHT = 15;
const int Game::MIN_VIEW_WIDTH = 15;
const char Game::HERO_ICON_LEFT = '<';
const char Game::HERO_ICON_RIGHT = '>';
const char Game::HERO_ICON_UP = '^';
const char Game::HERO_ICON_DOWN = 'v';

Level::Level(){ //initialize variables to avoid problems with empty level
	moves_remaining = 0;
	items_remaining = 0;
	max_player_moves = 0;
	first_map_segment_index = 0;
	current_map_segment_index = 0;
	level_index = 0;
	player_width_position = 0;
	player_height_position = 0;
}

Level::~Level(){
  //de-constructor
}

Level::Level(const std::string& filename){
  string level_text;
  level_index = 0; //have to initialize this in the constructor, can change it later
  moves_remaining = 0;
  items_remaining = 0;
  //read the level file to get level info:
  ifstream read_file("./game_files/"+filename);
  getline(read_file, level_text); //get map segment index from first line of file (which map segment the player starts in)
  first_map_segment_index = stoi(level_text);
  current_map_segment_index = first_map_segment_index;
  getline(read_file, level_text, ' '); //get height position of player
  player_height_position = stoi(level_text);
  getline(read_file, level_text); //get width position of player
  player_width_position = stoi(level_text);
  getline(read_file, level_text); // get direction player is facing
  direction = level_text;
  //parse through next lines to get details about map segments in this level:
  while(getline(read_file, level_text, ' ')){

	  if(level_text == "M"){ //M indicates new map segment
		  getline(read_file, level_text, ' '); //get height
		  int segment_height = stoi(level_text);
		  getline(read_file, level_text); //get width
		  int segment_width = stoi(level_text);
		  MapSegment m(segment_height, segment_width); //construct map segment
      m.set_id(map_segments.size());
		  map_segments.push_back(m);
	  }
	  else if(level_text == "B"){ //B indicates a building
		  getline(read_file, level_text, ' '); //get corresponding map segment index
		  int building_segment_index = stoi(level_text);
		  getline(read_file, level_text, ' '); //get y coordinate
		  int building_y_coordinate = stoi(level_text);
		  getline(read_file, level_text); //get x coordinate
		  int building_x_coordinate = stoi(level_text);
		  Building b(building_y_coordinate, building_x_coordinate); //construct building
		  map_segments[building_segment_index].addBuilding(b);
	  }
	  else if(level_text == "I"){ //I indicates an item
		  getline(read_file, level_text, ' '); //get corresponding map segment index
		  int item_segment_index = stoi(level_text);
		  getline(read_file, level_text, ' '); //get y coordinate
		  int item_y_coordinate = stoi(level_text);
		  getline(read_file, level_text); //get x coordinate
		  int item_x_coordinate = stoi(level_text);
		  Item i(item_y_coordinate, item_x_coordinate); //construct an item
		  //call draw item function in corresponding map segment
		  map_segments[item_segment_index].addItem(i);
		  items_remaining += 1;
	  }
	  else if(level_text == "P"){
		  getline(read_file, level_text, ' '); //get first segment index
		  int portal_segment1 = stoi(level_text);
		  getline(read_file, level_text, ' '); //get location of portal (entrance)
		  string portal_side1 = level_text;
		  getline(read_file, level_text, ' '); //get second segment index
		  int portal_segment2 = stoi(level_text);
		  getline(read_file, level_text); //get location of second portal (exit)
		  string portal_side2 = level_text;
		  //place a portal in each map segment:
      Portal* p1 = new Portal(portal_segment1, portal_segment2, portal_side1, portal_side2);
      Portal* p2 = new Portal(portal_segment2, portal_segment1, portal_side2, portal_side1);
      portals.push_back(p1);
      portals.push_back(p2);
      map_segments[portal_segment1].addPortal(portal_side1);
      map_segments[portal_segment2].addPortal(portal_side2);
      
      
	  }
	  else if(level_text == "N"){ //N indicates max number of moves allowed
		  getline(read_file, level_text); //get number of moves
		  max_player_moves = stoi(level_text);
		  moves_remaining += max_player_moves;
	  }
  }
  //add player to map segment:
  map_segments[current_map_segment_index].addPlayer(player_height_position, player_width_position, direction);
}

vector<MapSegment> Level::get_map_segments(){
  return map_segments;
}

int Level::get_current_map_segment_index(){
  return current_map_segment_index;
}

int Level::get_first_map_segment_index(){
	return first_map_segment_index;
}

int Level::get_items_remaining(){
  return items_remaining;
}

int Level::get_moves_remaining(){
  return moves_remaining;
}

int Level::get_player_height(){
  return player_height_position;
}

int Level::get_player_width(){
  return player_width_position;
}

void Level::subtract_item(){
	items_remaining = items_remaining - 1;
}

void Level::subtract_move(){
	moves_remaining = moves_remaining - 1;
}

void Level::set_player_height(int height){
  player_height_position = height;
}

void Level::set_player_width(int width){
  player_width_position = width;
}

int Level::get_level_index(){
  return level_index;
}

void Level::set_level_index(int level){
  level_index = level;
}

void Level::set_current_map_segment_index(int index){
	current_map_segment_index = index;
}

vector<Portal*> Level::get_portals(){
  return portals;
}

Game::Game(const std::string& filename, InterfaceType interfaceType)
    : mView{nullptr}
    , mController{nullptr}
    , mInterfaceType{interfaceType}
{
	//read the game file passed to the game constructor:
	string game_file_text;
	ifstream read_file(filename);
	getline(read_file, game_file_text, ' '); //first line is dimensions of screen
	screen_height = stoi(game_file_text); //screen height
	getline(read_file, game_file_text);
	screen_width = stoi(game_file_text); //screen width
	getline(read_file, game_file_text); //second line is number of levels
	num_levels = stoi(game_file_text);
	//next lines in game file contain relative path to each level file
	//set up each level:
	while(getline(read_file, game_file_text)){
		//get relative path to level file from game file:
		string level_file_path = game_file_text;
		Level* l = new Level(level_file_path);
		l->set_level_index(levels.size() + 1);
		levels.push_back(l); //add level to the levels vector
	}
	//do something with screen width and height here?
		 cout << screen_height;
		 cout << screen_width;
		 cout << num_levels;

	current_level = levels[0]; //start at very first level
  if(mInterfaceType == InterfaceType::Curses) { //use curses view and curses controller
		mView = new CursesView(screen_height, screen_width);
		mController = new CursesController();
	}
	else{ //use print view and print controller
		mView = new PrintView(screen_height, screen_width);
		mController = new PrintController();
	}
}



Game::~Game()
{
    // TODO: Usually, deallocate anything that was dynamically
    // allocated in the ctor (constructor).

}

void Game::move(MapSegment& map_segment, vector<string>& map_segment_vector){
	//get current player position & direction (player_height/width_position, get_direction_char)
	char direction = map_segment.get_direction_char();
	int row = current_level->get_player_height();
	int col = current_level->get_player_width();
	int new_row;
	int new_col;

	//get new position coordinates:
	if(direction == '^'){ //move up one row
		new_row = row - 1;
		new_col = col;
	}
	else if(direction == 'v'){ //move down one row
		new_row = row + 1;
		new_col = col;
	}
	else if(direction == '>'){ //move one column to the right
		new_row = row;
		new_col = col + 1;
	}
	else if(direction == '<'){ //move one column to the left
		new_row = row;
		new_col = col - 1;
	}
  
  bool is_portal = map_segment.check_portals(new_row, new_col);
  if(is_portal == true){
    string side = map_segment.find_side(new_row, new_col);
    for(auto portal: current_level->get_portals()){
      
      if(portal->get_segment_1() == map_segment.get_id() && portal->get_side_1() == side){
        int new_x = 0;
        int new_y = 0; 
        map_segment_vector[row][col] = ' ';
        map_segment = current_level->get_map_segments()[portal->get_segment_2()]; //update map segment
        map_segment.set_coords(portal->get_side_2(), new_x, new_y);
        cout << new_x << " | " << new_y << "\n";
        map_segment_vector = map_segment.get_map_segment();
        map_segment_vector[new_y][new_x + 1] = direction;
        //if statements for side
        current_level->set_player_height(new_y); //update player coordinates stored in level class
		    current_level->set_player_width(new_x + 1);
        // map_segment.addPlayer(new_y, new_x + 1, "right");
        cout << current_level->get_player_width() << " | " << current_level->get_player_height();
        
      }

    }

    
  }

	bool can_move = map_segment.check_walls(new_row, new_col); //check for walls and buildings and portals
	if(can_move == true){
		bool is_item = map_segment.check_item(new_row, new_col); //check to see if the player gets an item
		if(is_item == true){
			current_level->subtract_item(); //update items remaining
		}
		map_segment_vector[row][col] = ' '; //remove player from last position
		map_segment_vector[new_row][new_col] = direction; //update map with new player position
		current_level->set_player_height(new_row); //update player coordinates stored in level class
		current_level->set_player_width(new_col);

	}
}


void Game::process_input(Command cur_command, vector<string>& map_segment_vector, MapSegment* map_segment){
	//left = rotate left (counterclockwise)
	//right = rotate right(clockwise)
	char direction = map_segment->get_direction_char();
	if(cur_command == Command::Left){
    if(direction == '^'){
      direction = '<';
    }
    else if(direction == 'v'){
    	direction = '>';
    }
    else if(direction == '>'){
      direction = '^';
    }
    else if(direction == '<'){
      direction = 'v';
    }
  }
  else if(cur_command == Command::Right){
    if(direction == '^'){
      direction = '>';
    }
    else if(direction == 'v'){
    	direction = '<';
    }
    else if(direction == '>'){
      direction = 'v';
    }
    else if(direction == '<'){
      direction = '^';
    }
  }
  else if(cur_command == Command::Forward){
	  //don't change direction
	  move(*map_segment, map_segment_vector); //move the player, update map segment (check for items/walls)

  }

  map_segment_vector[current_level->get_player_height()][current_level->get_player_width()] = direction;
  map_segment->set_direction_char(direction);


}

void Game::run()
{

	bool play = true;
	int first_seg_index = current_level->get_first_map_segment_index();
	MapSegment current_map_segment = current_level->get_map_segments()[first_seg_index]; //start at map segment inde
	vector<string> map_to_draw = current_map_segment.get_map_segment();
	cout << "Current level: " << current_level->get_level_index() << "\n";
	cout << "Items remaining: "<< current_level->get_items_remaining() << "\n";
	cout << "Moves remaining: "<< current_level->get_moves_remaining() << "\n";

  vector<string> new_map; 


  for(int i = current_level->get_player_height() -  (screen_height/2); i < (current_level->get_player_height() + (screen_height/2)); i++){
    string new_row = "";
    for(int j = current_level -> get_player_width() - (screen_width/2); j <  (current_level->get_player_width() + (screen_width/2)); j++) {
      if(i >= 0 && i < current_map_segment.getHeight() && j >= 0 && j < current_map_segment.getWidth()){
        new_row += map_to_draw[i][j];
      }
      else{
        new_row += "~";
      }

    }
    new_map.push_back(new_row);
  }


	mView->draw(new_map); //print out empty starting board/map segment first
	while(play == true){
		Command current_command = mController->getInput(); //get command for where to move
		process_input(current_command, map_to_draw, &current_map_segment); //change direction or move forward
    // map_to_draw = current_map_segment.get_map_segment();
		//what if the input is to quit the game?
		current_level->subtract_move(); //update moves remaining
		//determine if the player lost the level (ran out of moves before getting all items)
		if(current_level->get_moves_remaining() == 0 && current_level->get_items_remaining() > 0){
			cout << "You lost the game.";
			play = false;
		}
		//what if you have no more moves but also no more items?
		else if(current_level->get_items_remaining() == 0){ //You either win or go to next level
			if(current_level->get_level_index() < num_levels){
				current_level = levels[current_level->get_level_index()]; //move to next level
				vector<MapSegment> map_segments_vector = current_level->get_map_segments();
				current_map_segment = map_segments_vector[current_level->get_current_map_segment_index()];
				map_to_draw = current_map_segment.get_map_segment();
				//vector is 0 indexed, level indexes start at 1

			}
			else{
				cout << "You won the game."; //game ends
				play = false;
			}
		}
		if(play == true){
			cout << "Current level: " << current_level->get_level_index() << "\n";
			cout << "Items remaining: "<< current_level->get_items_remaining() << "\n";
			cout << "Moves remaining: "<< current_level->get_moves_remaining() << "\n";
			mView->draw(map_to_draw); //print out current board/map segment with user's location
		}
	}

}



