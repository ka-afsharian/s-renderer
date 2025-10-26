#pragma once

#include "engproj/engine/handle.hpp"
#include "engproj/gl_utils/shader.hpp"

#include <cstdint>
#include <vector>
#include <map>


namespace engproj::engine{


class world{
  world();//parameter should be a resource manager

  entity_hdl create_entity();

  template<typename C>
  void add_component(entity_hdl,C);



private:
  std::vector<handle<handletype::pipeline>> pipeline_ids_;
  std::vector<handle<handletype::pipeline>> buffer_ids_;
  std::vector<handle<handletype::pipeline>> texture_ids_;
  std::map<uint32_t,engproj::gl_utils::pipeline> map_;
};



}
