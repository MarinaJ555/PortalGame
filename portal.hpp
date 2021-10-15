#ifndef PORTAL_HPP
#define PORTAL_HPP

#include <string>
#include <vector>
#include <iostream> // delete later

using namespace std;

class Portal //should this be its own class?
{
  public:
    Portal(int seg1, int seg2, string s_1, string s_2);

    int get_segment_1();
    int get_segment_2();
    string get_side_1();
    string get_side_2();

  private: 
    int segment_1;
    int segment_2; 
    string side_1;
    string side_2; 
};


#endif // PORTAL_HPP
