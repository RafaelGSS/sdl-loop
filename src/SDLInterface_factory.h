#ifndef SDLINTERFACE_FACTORY_H
#define SDLINTERFACE_FACTORY_H

#include <SDL2/SDL.h>
#include "./game_object.h"

class SDLInterfaceFactory {
  public:
    static SDLInterfaceFactory* instance();

    SDL_Window* create_window(GameObject obj, const char* title = NULL, Uint32 flags = SDL_WINDOW_MAXIMIZED);
    SDL_Renderer* create_renderer(SDL_Window* window);
};
#endif
