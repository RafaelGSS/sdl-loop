#include "./SDLInterface_factory.h"

SDLInterfaceFactory* SDLInterfaceFactory::instance() {
  static SDLInterfaceFactory* singleton = nullptr;
  if (!singleton) {
    singleton = new SDLInterfaceFactory();
  }
  return singleton;
}

SDL_Window* SDLInterfaceFactory::create_window(
    GameObject obj,
    const char* title = NULL,
    Uint32 flags = SDL_WINDOW_MAXIMIZED
    ) {
  return SDL_CreateWindow(title, obj.x, obj.y, obj.width, obj.height, flags);
}

SDL_Renderer* SDLInterfaceFactory::create_renderer(SDL_Window* window) {
  return SDL_CreateRenderer(window, -1, 0);
}

