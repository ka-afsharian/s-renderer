//pushes commands onto render_queue
#pragma once

#include "engproj/gl_utils/renderer/framegraph.hpp"
//#include "engproj/gl_utils/renderer/render_device.hpp"//maybe not
#include "engproj/engine/world.hpp"//change to snapshot later

namespace engproj::gl_utils::renderer{

class framegraph;

class render_pass{
public:
  virtual ~render_pass() = default;
  virtual void register_resources(renderer::framegraph& framegraph) = 0;
  virtual void execute(engproj::engine::world& world) = 0;

  std::string name_;
};

}
