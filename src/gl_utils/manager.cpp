#include "engproj/gl_utils/manager.hpp"
#include "engproj/gl_utils/window.hpp"
#include "engproj/gl_utils/context.hpp"
#include "engproj/sdl_utils/sdl_init.hpp"
#include "engproj/logger/logger.hpp"
#include <glad/glad.h>
#include "SDL3/SDL.h"//this is used!!
#include <memory>
using namespace engproj::gl_utils;
using namespace engproj::sdl_utils;
using namespace engproj::logger;

manager::manager() : hardware_props_(), initial(false){
 //init(); DO NOT CALL INIT IN CONSTRUCTOR, constructor must finish so shared_from_this can work!!!
}


int manager::init(){
  if(!initial){

    if(sdl_init()){
      return -1;
    }

    auto mywindow= std::make_shared<window>("BLAH BLAH",1920,1080,std::vector<window::flags>{window::flags::opengl,window::flags::resizable},
                                            shared_from_this());
    auto mycontext = std::make_shared<engproj::gl_utils::context>(mywindow);

    if (mycontext->isvalid() && mywindow->isvalid()){
      context_map_["main"] = mycontext;
      window_map_["main"] = mywindow;
    }else{return -1;}

    if(context_map_["main"]->makecurrent() == -1){
      return -1;
    }
    if (!gladLoadGLLoader((GLADloadproc)SDL_GL_GetProcAddress)) {
        g_logger.debug("Failed to initialize OpenGL context");
        SDL_Quit();
        return -1;
    }
    initial = true;
  }

  hardware_props_.renderer = reinterpret_cast<const char*>(glGetString(GL_RENDERER));//lol
  hardware_props_.vendor = reinterpret_cast<const char*>(glGetString(GL_VENDOR));//lol
  hardware_props_.version = reinterpret_cast<const char*>(glGetString(GL_VERSION));//lol

  GLint maxbindings=0;
  glGetIntegerv(GL_MAX_UNIFORM_BUFFER_BINDINGS, &maxbindings);
  hardware_props_.max_ubo_bindings = maxbindings;

  maxbindings=0;
  glGetIntegerv(GL_MAX_SHADER_STORAGE_BUFFER_BINDINGS,&maxbindings);
  hardware_props_.max_ssbo_bindings = maxbindings;

  GLint texunits = 0;
  glGetIntegerv(GL_MAX_TEXTURE_IMAGE_UNITS,&texunits);
  hardware_props_.max_combined_texture_image_units = texunits;

  GLint maxsamples = 0;
  glGetIntegerv(GL_MAX_SAMPLES,&maxsamples);
  hardware_props_.max_samples = maxsamples;

  GLint numext = 0;
  glGetIntegerv(GL_NUM_EXTENSIONS, &numext);
  for (GLint i = 0; i < numext; ++i) {
    std::string ext = reinterpret_cast<const char*>(glGetStringi(GL_EXTENSIONS, i));
    hardware_props_.gl_supported_extensions.push_back(std::move(ext));
  }
  return 0;
}

void manager::print_hardware_props(){
  logger::g_logger.info("Renderer: {}",hardware_props_.renderer);
  logger::g_logger.info("Vendor: {}",hardware_props_.vendor);
  logger::g_logger.info("Version: {}",hardware_props_.version);
  logger::g_logger.info("UBO Bindings: {}",hardware_props_.max_ubo_bindings);
  logger::g_logger.info("SSBO Bindings: {}",hardware_props_.max_ssbo_bindings);
  logger::g_logger.info("Texture Units: {}",hardware_props_.max_combined_texture_image_units);
  logger::g_logger.info("Max Samples: {}",hardware_props_.max_samples);
    for(auto& e : hardware_props_.gl_supported_extensions){
      logger::g_logger.info("Supported Extension: {}",e);
    }
}

std::shared_ptr<context> manager::get_context(std::string){
  auto ele = context_map_.find("main");
  if(ele != context_map_.end()){
    return ele->second;
  }else{return nullptr;}
}
