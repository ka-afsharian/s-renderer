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
  explicit window(const std::string title,size_t width, size_t height,std::vector<flags> flags_par,std::shared_ptr<manager> manager);
  ~window();
  void swap();
  const props& getprops() const;
  bool isvalid();
  void* get_window_ptr();
  void set_height(uint32_t height);
  void set_width(uint32_t width);
  void set_title(std::string);
private:
  struct window_PIMPL;
  std::unique_ptr<window_PIMPL> window_;
  std::shared_ptr<manager> manager_;
  props props_;
  mutable std::shared_mutex mtx_;
};

}
