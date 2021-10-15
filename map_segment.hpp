#ifndef MAP_SEGMENT_HPP
#define MAP_SEGMENT_HPP

#include <string>
#include <vector>
#include <iostream> // for debugging (delete later)

#include "building.hpp"
#include "item.hpp"
#include "portal.hpp"

using namespace std;

/**
 * This is probably the class that will have the second biggest portion of
 * your code for this assignment. (The biggest would likely be held
 * by the Game class.)
 *
 * Some things I did that you don't have to do:
 * - Define an enum class for portal directions.
 * - Method for connecting two map segments' portals.
 * - Methods for adding buildings or items to a map segment.
 */

class MapSegment
{
	vector<string> map_segment;
	vector<Portal*> portals;

public:
    MapSegment(int height, int width);
    ~MapSegment();
    int getHeight() const { return mHeight; }
    int getWidth() const { return mWidth; }
    int getPortalX() const { return mPortalX; }
    int getPortalY() const { return mPortalY; }
    vector<string> get_map_segment();

    /**
     * Returns representation of this map segment as a vector of strings,
     * including any contained items and buildings.
     */
    // std::vector<std::string> getAsLines() const;
    void addBuilding(Building b);
    void addItem(Item i);
    void addPlayer(int y_coordinate, int x_coordinate, string direction);
    void addPortal(string side);
    void set_coords(string side, int& x_coord, int& y_coord);
    int getPlayerXCoordinate();
    int getPlayerYCoordinate();
    char get_direction_char();
    void set_direction_char(char direction);
    bool check_item(int y_coordinate, int x_coordinate);
    bool check_walls(int y_coordinate, int x_coordinate);
    bool check_portals(int y_coordinate, int x_coordinate);
    Portal* get_portal_from_coordinates(int portal_y, int portal_x, int seg_index);
    string find_side(int y_coordinate, int x_coordinate);
    int get_id();
    void set_id(int new_id);
    
    void printSeg(){
      for(auto seg : map_segment){
        cout << seg << "\n";
      }
    }


private:
    static char VERTICAL_BORDER_CHAR;
    static char HORIZONTAL_BORDER_CHAR;
    static char PORTAL_CHAR;
    char direction_char;
    /**
     * As always, you don't need to have these member variables if you don't
     * think they would be useful to you.
     */

    int mHeight;
    int mWidth;
    /**
     * For helping position the portals.
     * See how they are initialized in the constructor.
     */
    int mPortalY;
    int mPortalX;
    int id; 

};


#endif // MAP_SEGMENT_HPP
