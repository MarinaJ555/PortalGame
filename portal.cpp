#include "portal.hpp"
using namespace std;

Portal::Portal(int seg1, int seg2, string s_1, string s_2){
  segment_1 = seg1;
  segment_2 = seg2;
  side_1 = s_1;
  side_2 = s_2;

}

int Portal::get_segment_1(){
  return segment_1;
}

int Portal::get_segment_2(){
  return segment_2;
}

string Portal::get_side_1(){
  return side_1;
}

string Portal::get_side_2(){
  return side_2;
}