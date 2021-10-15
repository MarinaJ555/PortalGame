#ifndef GAME_HPP
#define GAME_HPP

#include "controller.hpp"
#include "view.hpp"
#include "interface_type.hpp"
#include "map_segment.hpp"
using namespace std;

class Level{
  int first_map_segment_index; //which map player starts on
  int current_map_segment_index; //the current map segment index (will be updated as player moves between maps)
  //coordinates where player starts relative to map segment:
  int player_height_position;
  int player_width_position;
  string direction; //which direction the player is facing at the start
  vector<MapSegment> map_segments; //contains all the map segments inside this level
  int max_player_moves;
  int items_remaining;
  int level_index;
  vector<Portal*> portals; 

  public:
  int moves_remaining;
  Level();
  Level(const std::string& filename);
  ~Level();
  void printMaps();
  vector<MapSegment> get_map_segments();
  int get_first_map_segment_index();
  int get_current_map_segment_index();
  int get_items_remaining();
  void subtract_item();
  int get_moves_remaining();
  void subtract_move();
  int get_player_height();
  int get_player_width();
  void set_player_height(int height);
  void set_player_width(int width);
  int get_level_index();
  void set_level_index(int level);
  void set_current_map_segment_index(int index);
  vector<Portal*> get_portals();
};

class Game
{
public:  // public methods
    /**
     * DO NOT change the prototype of this function.
     * The autograder WILL call this function.
     */
    Game(const std::string& filename, InterfaceType interfaceType);

    ~Game();
    Game(const Game&) = delete;
    Game& operator=(const Game&) = delete;
    
    /**
     * DO NOT change the prototype of this function.
     * The autograder WILL call this function.
     */
    void run();
    void process_input(Command cur_command, vector<string>& map_segment_vector, MapSegment* map_segment);
    void move(MapSegment& map_segment, vector<string>& map_segment_vector);

private:  // private methods
    /**
     * You DO NOT need to have any of the private methods below.
     * I just left some commented out declarations in case it
     * helps guide your thinking, but you don't need to use them.
     */

    /**
     * Set up the 2D vector of strings that is then given to the draw()
     * method of an instance of a subclass of View.
     */
     //void draw();

    /**
     * Loop until the game is over. On each iteration, use the controller
     * to get input from the user.
     */
    // void doGameLoop();

    /**
     * Update matters related to game logic, e.g. collisions, level completion.
     */
    // void update();

    /**
     * Load information related to new level into member variables,
     * being careful to not leave traces of information from previous levels. 
     */
    // void loadLevel();

private:
    static const int MIN_VIEW_HEIGHT;
    static const int MIN_VIEW_WIDTH;
    static const char HERO_ICON_LEFT;
    static const char HERO_ICON_RIGHT;
    static const char HERO_ICON_UP;
    static const char HERO_ICON_DOWN;

    /**
     * Because one of the constructor parameters must be checked in order
     * to determine the subclass types for the below members, they can't
     * be references, because there's no default constructors for them.
     * (Recall that each member in a class must either be initialized by an
     * initializer list in the constructor OR have a default constructor.)
     */
    View* mView;
    Controller* mController;
    InterfaceType mInterfaceType;
    vector<Level*> levels;
    Level* current_level;
    int num_levels;
    int screen_height;
    int screen_width;

};

#endif // GAME_HPP
