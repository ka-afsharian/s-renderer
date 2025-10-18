#pragma once

#include <cstdint>
namespace engproj::gl_utils{

class viewport{
public:
  viewport(const float x,const float y,const float x2,const float y2);//x and y are bottom left,
  viewport();
  float get_aspect_ratio();
  bool is_valid() const;
  float x_,y_,x2_,y2_;
private:
};

}
