//commands that the command queue will hold
#pragma once

#include "engproj/engine/handle.hpp"
#include "engproj/data_structures/command_queue.hpp"
#include "engproj/gl_utils/renderer/render_cmd.hpp"
#include "engproj/gl_utils/renderer/render_pass.hpp"
#include <string>
#include <unordered_map>

namespace engproj::gl_utils::renderer{

class render_pass;

class framegraph{
public:
  struct resource{
    engine::texture_hdl texture_;
  };

  framegraph(engproj::data_structures::command_queue<renderer::render_cmd>& command_queue);
  framegraph(const framegraph&) = delete;
  framegraph& operator=(const framegraph&) = delete;
  framegraph(framegraph&&) = delete;
  framegraph& operator=(framegraph&&) = delete;

  template<typename Render_pass, typename... Args>
  Render_pass& add_pass(const std::string& name, Args&&... args);

  void compile();
  void execute();



private:
  engproj::data_structures::command_queue<renderer::render_cmd>& command_queue_;
  std::vector<std::unique_ptr<renderer::render_pass>> passes_;
  std::unordered_map<std::string, resource> resources_;

};

}
