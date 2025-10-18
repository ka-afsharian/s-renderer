#include <engproj/sdl_utils/sdl_init.hpp>
#include "SDL3/SDL.h"//this is used!!
using namespace engproj::sdl_utils;


int engproj::sdl_utils::sdl_init(){

    if (SDL_Init(SDL_INIT_VIDEO) == 0) {
    SDL_Quit();
    return -1;
    }

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 6);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    return 0;
}
