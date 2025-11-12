//pushes commands onto render_queue
#pragma once

//#include "engproj/gl_utils/renderer/render_device.hpp"//maybe not
#include "engproj/engine/world.hpp"//change to snapshot later, should this just be forward decl?
#include "engproj/data_structures/command_queue.hpp"
#include "engproj/gl_utils/renderer/render_cmd.hpp"
#include "engproj/gl_utils/renderer/fg_resource.hpp"

namespace engproj::gl_utils::renderer{

class framegraph;

class render_pass{
public:
  virtual ~render_pass() = default;
  virtual void register_resources(renderer::framegraph& framegraph) = 0;
  virtual void execute(engproj::data_structures::command_queue<renderer::render_cmd>& command_queue,const engproj::engine::world& world) = 0;

  std::string name_;
  std::vector<renderer::fg_resource> reads_;
  std::vector<renderer::fg_resource> writes_;
};

}
