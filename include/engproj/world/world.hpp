#pragma once

#include "engproj/gl_utils/shader.hpp"

#include <vector>
#include <map>

namespace engproj::world{

enum class handletype{
   pipeline,
   buffer,
   texture
};

template<handletype T>
struct handle{
  uint32_t id=0;//zero is invalid
  uint32_t gen=0;//generation
};

using pipeline_hdl = handle<handletype::pipeline>;
using buffer_hdl = handle<handletype::buffer>;
using texture_hdl = handle<handletype::texture>;

class world{



private:
  std::vector<handle<handletype::pipeline>> pipeline_ids_;
  std::vector<handle<handletype::pipeline>> buffer_ids_;
  std::vector<handle<handletype::pipeline>> texture_ids_;
  std::map<uint32_t,engproj::gl_utils::pipeline>;
};



}
