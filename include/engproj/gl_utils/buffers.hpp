#pragma once

#include "buffertype.hpp"

#include <cstdint>

namespace engproj::gl_utils{

class buffer{
public:
  buffer();
  ~buffer();
  int bind(buffertype);
private:
  uint32_t id;

};

}
