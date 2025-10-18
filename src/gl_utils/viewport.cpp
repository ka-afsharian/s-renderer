#include <glad/glad.h>
#include "engproj/gl_utils/viewport.hpp"



using namespace engproj::gl_utils;


viewport::viewport(const float x,const float y,const float x2,const float y2) : x_(x),y_(y),x2_(x2),y2_(y2){

}

float viewport::get_aspect_ratio(){
  if(!is_valid()) return 1.0f;
  return x2_/y2_;
}


bool viewport::is_valid() const{
  return (x_ >= 0 && y_ >= 0 && x_ < 1 && y_ < 1 && x2_ > 0 && y2_ > 0 && (x_+x2_)<= 1 && (y_+y2_) <= 1);
}
