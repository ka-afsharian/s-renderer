#include <iostream>
#include "engproj/gl_utils/manager.hpp"
#include <glad/glad.h>
#include "SDL3/SDL.h"//this is used!!
using namespace engproj::gl_utils;

//each context has its own bindings, current pipeline?,vao bidnings, framebufferbinding, states blend depth stencil, viewport
//comnmands queed in driver, binded textures

struct engproj::gl_utils::window::window_PIMPL{
  SDL_Window* window_;
};

window::window(const std::string title,size_t width, size_t height,std::vector<flags> flags_par,std::shared_ptr<manager> manager)
                 : window_(std::make_unique<window_PIMPL>()), manager_(manager){
  SDL_WindowFlags flags_fin = 0;
  for(auto& e : flags_par){
    switch(e){
      case window::flags::opengl:
        flags_fin |= SDL_WINDOW_OPENGL;
      break;
      case window::flags::borderless:
        flags_fin |= SDL_WINDOW_BORDERLESS;
      break;
      case window::flags::fullscreen:
        flags_fin |= SDL_WINDOW_FULLSCREEN;
      break;
      case window::flags::hidden:
        flags_fin |= SDL_WINDOW_HIDDEN;
      break;
      case window::flags::maximized:
        flags_fin |= SDL_WINDOW_MAXIMIZED;
      break;
      case window::flags::minimized:
        flags_fin |= SDL_WINDOW_MINIMIZED;
      break;
      case window::flags::resizable:
        flags_fin |= SDL_WINDOW_RESIZABLE;
      break;
    }
  }
  window_->window_ = SDL_CreateWindow(title.c_str(), width, height,flags_fin);
    //SDL_Window* window = SDL_CreateWindow("SDL OPENGL", 1920,1080,SDL_WINDOW_OPENGL);//|SDL_WINDOW_RESIZABLE);
  props_.flags_ = flags_par;
  props_.height_ = height;
  props_.width_ = width;
  props_.title_ = title;
}

window::~window(){
  std::cout << "WINDOW DESTROYED" << std::endl;
  SDL_DestroyWindow(window_->window_);
}


bool window::isvalid(){
  if(!(window_->window_)){
    return false;
  }else{return true;}
}

void window::swap(){
  SDL_GL_SwapWindow(window_->window_);
}
const window::props& window::getprops() const{
  return props_;
}

void* window::get_window_ptr(){
  return window_->window_;
}
