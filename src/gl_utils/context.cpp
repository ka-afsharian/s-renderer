#include <iostream>
#include "engproj/gl_utils/window.hpp"
#include "engproj/gl_utils/context.hpp"
#include <glad/glad.h>
#include "SDL3/SDL.h"//this is used!!
using namespace engproj::gl_utils;

//each context has its own bindings, current pipeline?,vao bidnings, framebufferbinding, states blend depth stencil, viewport
//comnmands queed in driver, binded textures
struct engproj::gl_utils::context::context_PIMPL{
  SDL_GLContext context_;
  bool valid(){
    if(!context_){
      return false;
    }else{return true;}
  }
};

//----------------------
context::context(std::shared_ptr<window> window_ptr, std::shared_ptr<context> context_ptr)
                         : context_(std::make_unique<context_PIMPL>()) {
  if(!window_ptr){return;}
  if(context_ptr){//this sharing code might be wrong, there is no documentation on sdl3 website//i might need to create a window after setting shareing???
    SDL_GLContext oldcontext = SDL_GL_GetCurrentContext();
    SDL_Window* oldwindow = SDL_GL_GetCurrentWindow();

    context_ptr->makecurrent();
    SDL_GL_SetAttribute(SDL_GL_SHARE_WITH_CURRENT_CONTEXT,1);//test if this works?
    context_->context_ = SDL_GL_CreateContext((SDL_Window*)window_ptr->get_window_ptr());
    shared_toward_ = context_ptr; //set the context im sharing my stuff with
    //SDL_GL_GetAttribute(SDL_GL_SHARE_WITH_CURRENT_CONTEXT) //check attribute later!
    SDL_GL_MakeCurrent(oldwindow,oldcontext);//set them back to what they were before - no state tracking

  }else{
    SDL_GL_SetAttribute(SDL_GL_SHARE_WITH_CURRENT_CONTEXT,0);//test if this works?
    context_->context_ = SDL_GL_CreateContext((SDL_Window*)window_ptr->get_window_ptr());
  }
  window_ = window_ptr;
}

context::~context(){
  SDL_GL_DestroyContext(context_->context_);

}

int context::makecurrent(){
  if (SDL_GL_MakeCurrent((SDL_Window*)window_->get_window_ptr(), context_->context_) != true) {
    SDL_Log("SDL_GL_MakeCurrent failed: %s", SDL_GetError());
    return -1;
  }
  return 0;
}

void* context::getptr(){
  return context_->context_;
}



bool context::isvalid(){
  if(!(context_->context_ && window_->isvalid())){
    return false;
  }else{return true;}
}
