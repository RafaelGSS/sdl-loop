#include <iostream>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include "./constants.h"

bool game_is_running = false;
SDL_Window* window = NULL;
SDL_Renderer* renderer = NULL;
TTF_Font* font = NULL;
unsigned int score = 0;

int last_frame_time = 0;

struct game_object {
  float x;
  float y;
  float width;
  float height;
  float vel_x;
  float vel_y;
} ball, paddle;

bool initialize_window() {
  if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
    std::cout << "Error initializing SDL." << std::endl;
    return false;
  }

  if (TTF_Init() != 0) {
    std::cout << "Error initializing TTF." << std::endl;
    return false;
  }

  window = SDL_CreateWindow(
      NULL,
      SDL_WINDOWPOS_CENTERED,
      SDL_WINDOWPOS_CENTERED,
      WINDOW_WIDTH,
      WINDOW_HEIGHT,
      SDL_WINDOW_BORDERLESS
  );

  if (!window) {
    std::cout << "Error creating SDL Window." << std::endl;
    return false;
  }

  renderer = SDL_CreateRenderer(window, -1, 0);
  if (!renderer) {
    std::cout << "Error creating SDL Renderer." << std::endl;
    return false;
  }

  return true;
}

void process_input() {
  SDL_Event event;
  SDL_PollEvent(&event);

  switch (event.type) {
    case SDL_QUIT:
      game_is_running = false;
      break;
    case SDL_KEYDOWN:
      if (event.key.keysym.sym == SDLK_ESCAPE)
        game_is_running = false;
      if (event.key.keysym.sym == SDLK_LEFT)
        paddle.vel_x = -400;
      if (event.key.keysym.sym == SDLK_RIGHT)
        paddle.vel_x = +400;
      break;
    case SDL_KEYUP:
      if (event.key.keysym.sym == SDLK_LEFT)
        paddle.vel_x = 0;
      if (event.key.keysym.sym == SDLK_RIGHT)
        paddle.vel_x = 0;
      break;
  }
}

void setup() {
  ball.width = 15;
  ball.height = 15;
  ball.x = 20;
  ball.y = 20;
  ball.vel_x = 300;
  ball.vel_y = 300;

  paddle.width = 100;
  paddle.height = 20;
  paddle.x = (WINDOW_WIDTH / 2) - (paddle.width / 2);
  paddle.y = WINDOW_HEIGHT - 40;
  paddle.vel_x = 0;
  paddle.vel_y = 0;

  font = TTF_OpenFont("/home/rafaelgss/repos/game/sdl-c/src/res/zorque.ttf", 70);
  if (font == NULL) {
    std::cout << "Error font not loaded " << TTF_GetError() << std::endl;
    exit(1);
  }
}

void update() {
  // Waste some time / sleep until we reach the frame target time
  while (!SDL_TICKS_PASSED(SDL_GetTicks(), last_frame_time + FRAME_TARGET_TIME));

  // Get a delta time factor converted to seconds to be used to update my objects
  float delta_time = (SDL_GetTicks() - last_frame_time) / 1000.0f;

  // Store the milliseconds of the current frame
  last_frame_time = SDL_GetTicks();

  // Check for ball collision with the walls
  if (ball.x + ball.width > WINDOW_WIDTH || ball.x < 0)
    ball.vel_x = -ball.vel_x;
  if (ball.y < 0)
    ball.vel_y = -ball.vel_y;

  // Check for ball collision with the paddle
  if (ball.y + ball.height >= paddle.y && ball.x + ball.width > paddle.x && ball.x < paddle.x + paddle.width) {
    ball.vel_y = -ball.vel_y;
    score++;
  }

  // Check for game over
  if (ball.y + ball.height > WINDOW_HEIGHT)
    game_is_running = false;

  // update ball position
  ball.x += ball.vel_x * delta_time;
  ball.y += ball.vel_y * delta_time;

  // update paddle position
  paddle.x += paddle.vel_x * delta_time;
  paddle.y += paddle.vel_y * delta_time;

  if (paddle.x + paddle.width > WINDOW_WIDTH) {
    paddle.x = WINDOW_WIDTH - paddle.width;
  } else if (paddle.x < 0) {
    paddle.x = 0;
  }

}

void render_score() {
  std::string score_text = "Score: " + std::to_string(score);
  SDL_Surface* text_surface = TTF_RenderText_Solid(font, score_text.c_str(), { 255, 255, 255 });
  SDL_Texture* text = SDL_CreateTextureFromSurface(renderer, text_surface);
  SDL_Rect score_rect = {
    (int)((WINDOW_WIDTH / 2) - 100 / 2),
    40,
    100,
    50
  };

  SDL_RenderCopy(renderer, text, NULL, &score_rect);
}

void render() {
  SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
  SDL_RenderClear(renderer);

  // Draw a rectangle
  SDL_Rect ball_rect = {
    (int)ball.x,
    (int)ball.y,
    (int)ball.width,
    (int)ball.height
  };
  SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
  SDL_RenderFillRect(renderer, &ball_rect);

  // Render the left paddle
  SDL_Rect paddle_rect = {
    (int)paddle.x,
    (int)paddle.y,
    (int)paddle.width,
    (int)paddle.height
  };
  SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
  SDL_RenderFillRect(renderer, &paddle_rect);

  render_score();

  SDL_RenderPresent(renderer);
}

void destroy_window() {
  TTF_CloseFont(font);
  TTF_Quit();
  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
  SDL_Quit();
}

int main(int argc, char **argv) {
  game_is_running = initialize_window();

  setup();

  while (game_is_running) {
    process_input();
    update();
    render();
  }

  destroy_window();

  return 0;
}
