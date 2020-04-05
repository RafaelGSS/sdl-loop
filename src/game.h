#ifndef GAME_H
#define GAME_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

#include <map>
#include <string>

#include "./abstract_game.h"
#include "./game_object.h"

class Game: public AbstractGame {
  private:
    int last_frame_time;
    int score;

    SDL_Window* window;
    SDL_Renderer* renderer;
    TTF_Font* font;

    std::map<std::string, GameObject> objects;

    bool setup();
    void create_objects();

    void render_text(std::string text, SDL_Rect text_rect);
    void render_texts();
    void render_objects();

  public:
    Game();
    ~Game();

    bool initialize();
    void process_input();
    void update();
    void render();
};
#endif
