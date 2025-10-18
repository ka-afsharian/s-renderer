#pragma once

#include <cstdint>
namespace engproj::gl_utils{

class viewport{
public:
  explicit viewport(const float x,const float y,const float x2,const float y2);//x and y are bottom left,
  float get_aspect_ratio();
  bool is_valid() const;
private:
  float x_,y_,x2_,y2_;
};

}
