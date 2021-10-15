#ifndef BUILDING_HPP
#define BUILDING_HPP

#include <string>
#include <vector>

class Building
{

public:
    Building(int y, int x);
    int get_width();
    int get_height();
    int get_x();
    int get_y();
    /**
     * You DO NOT have to have either of the below methods.
     * I leave them as suggestions.
     */

    /**
     * Draws self onto the given board.
     */
    // void drawIn(std::vector<std::string>& board) const;

    /**
     * Returns true if the given coordinates are contained
     * in this building.
     */
    // bool contains(int y, int x) const;

private:
    static const int width;
    static const int height;
    int y_coordinate;
	  int x_coordinate;
};

#endif // BUILDING_HPP
