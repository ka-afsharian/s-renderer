#pragma once
#include <cstdint>
#include <memory>
#include <vector>

namespace engproj::gl_utils{

class manager;

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
  void* get_window_ptr();
private:
  props props_;
};

}
