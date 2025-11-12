#pragma once

#include "engproj/engine/handle.hpp"
#include <vector>

namespace engproj::gl_utils::renderer{

class fg_resource{
public:
  enum class format{
    r8,
    rg16f,
    rgb16f,
    rgba16f,
    rgba32f,
    rgba8,
    srgb8_alpha8,
    depth24_stencil8,
  };

  enum usage_flag : uint32_t{
    none = 0,//final output
    present = 1 << 0,
    color_attachment = 1 << 1,//render target
    shader_resource = 1 << 2,
    depth_attachment = 1 << 3//depth/stencil buffer
  };

  fg_resource(uint32_t w, uint32_t h, format fmt, uint32_t usages = usage_flag::none) : width_(w), height_(h),
                                                                                    format_(fmt), usage_(usages){
  }

  uint32_t width_;
  uint32_t height_;
  format format_;
  uint32_t usage_;

  engine::texture_hdl get_handle() const {
    return texture_;
  }

  void set_handle(engine::texture_hdl handle){
    texture_ = handle;
    initialized_ = true;
  }

  void set_flag(usage_flag flag){
    usage_ = usage_ | flag;
  }
  void unset_flag(usage_flag flag){
    usage_ &= ~flag;
  }

  bool has_flag(usage_flag flag) const{
    return usage_ & flag;
  }

  bool is_initialized() const{
    return initialized_;
  }

private:
  engine::texture_hdl texture_ = engine::texture_hdl::invalid;
  bool initialized_ = false;//wether texture_hdl is created and valid
};

}
