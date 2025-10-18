#pragma once
#include "engproj/gl_utils/viewport.hpp"

#include <cstdint>
#include <memory>
#include <vector>
#include <map>
#include <optional>

namespace engproj::gl_utils{

class manager;

struct context_PIMPL;
struct window_PIMPL;

class window{
public:
  enum class flags{
    opengl,
    fullscreen,
    hidden,
    borderless,
    resizable,
    minimized,
    maximized
  };
  struct props{
    std::string title_;
    uint32_t width_;
    uint32_t height_;
    std::vector<flags> flags_;
  };
  explicit window(const std::string title,size_t width, size_t height,std::vector<flags> flags_par,std::shared_ptr<manager> manager);
  void* getptr();//dont use this, just for debugging
  ~window();
  void swap();
  const props& getprops() const;
  std::unique_ptr<window_PIMPL> window_;
  std::shared_ptr<manager> manager_;
  bool isvalid();
private:
  props props_;
};

class context{//only a single renderer should use a context
public:
  struct state{
    std::string viewport_;
    bool scissor_test_;
  };
  explicit context(std::shared_ptr<window> window_ptr, std::shared_ptr<context> context_ptr = nullptr );
  ~context();
  int makecurrent();
  void* getptr();//dont use this, just for debugging
  bool isvalid();
  int activate_viewport(std::string,bool enable_scissor=true);
  void deactivate_viewport();
  std::shared_ptr<window> window_;//points to window its bound,  can only have 1
private:
  std::unique_ptr<context_PIMPL> context_;//actual context data for SDL3
  std::shared_ptr<context> shared_toward_ = nullptr;//*this context is sharing its stuff with context shared_toward_
  std::map<std::string,std::shared_ptr<viewport>> viewport_map_;
};


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
