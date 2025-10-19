#pragma once
#include "engproj/gl_utils/viewport.hpp"
#include "engproj/gl_utils/window.hpp"

#include <cstdint>
#include <memory>
#include <map>

namespace engproj::gl_utils{



class context{//only a single renderer should use a context
public:

  struct state{
    std::string viewport_;
    bool scissor_test_;
    bool depth_test;
  };
  explicit context(std::shared_ptr<window> window_ptr, std::shared_ptr<context> context_ptr = nullptr );
  ~context();
  int makecurrent();
  void* get_context_ptr();//dont use this, just for debugging
  bool isvalid() const;
  int activate_viewport(std::string);
  int scissor_viewport();
  void deactivate_viewport();
  void add_viewport(std::string,viewport);
  void refresh_viewport();
  std::shared_ptr<window> window_;//points to window its bound,  can only have 1
private:
  struct context_PIMPL;
  std::unique_ptr<context_PIMPL> context_;//actual context data for SDL3
  std::shared_ptr<context> shared_toward_ = nullptr;//*this context is sharing its stuff with context shared_toward_
  std::map<std::string,viewport> viewport_map_;
  state state_;
};

}
