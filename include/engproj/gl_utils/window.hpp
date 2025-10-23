#pragma once
#include <cstdint>
#include <memory>
#include <shared_mutex>
#include <vector>

namespace engproj::gl_utils{

class manager;


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
  explicit window(const std::string title,const size_t width,const size_t height,const std::vector<flags>& flags_par,std::shared_ptr<manager> manager);
  ~window();
  void swap();
  const props& getprops() const;
  bool isvalid();
  void* get_window_ptr();//remember to cast it to (SDL_Window*) after you call
  void set_height(const uint32_t height);
  void set_width(const uint32_t width);
  void set_title(const std::string);
private:
  struct window_PIMPL;
  std::unique_ptr<window_PIMPL> window_;
  std::shared_ptr<manager> manager_;
  props props_;
  mutable std::shared_mutex mtx_;//there are multple context on a window so it needs a mutex
};

}
