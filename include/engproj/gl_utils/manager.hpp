#pragma once
#include "engproj/gl_utils/viewport.hpp"
#include "engproj/gl_utils/window.hpp"
#include "engproj/gl_utils/context.hpp"

#include <cstdint>
#include <memory>
#include <vector>
#include <map>

namespace engproj::gl_utils{

class renderer{
public:
  renderer(std::shared_ptr<context> context);
  ~renderer();
private:
  std::shared_ptr<context> context_; //context its bound to, each renderer can only have 1 context
};

class manager : public std::enable_shared_from_this<manager>{//shared from this allows class to give shared ptrs of itself
public:

  struct hardware_props{
    std::string_view vendor;
    std::string_view renderer;
    std::string_view version;
    std::string_view shading_language_version;
    int32_t max_ubo_bindings;
    int32_t max_ssbo_bindings;
    int32_t max_combined_texture_image_units;
    int32_t max_samples;
    std::vector<std::string> gl_supported_extensions;
  };

  manager();
  int init();
  //~manager();
  std::shared_ptr<context> get_context(std::string);
  void print_hardware_props();
private:
  hardware_props get_binding_props() const;
  hardware_props hardware_props_;
  std::map<std::string,std::shared_ptr<window>> window_map_;
  std::map<std::string,std::shared_ptr<context>> context_map_;
  bool initial;//to make init not crash on multiple calls
};

}
