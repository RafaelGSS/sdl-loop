#include <iostream>

#include "./game.h"
#include "./SDLInterface_factory.h"
#include "./constants.h"

bool Game::setup() {
  this->create_objects();

  this->font = TTF_OpenFont("/home/rafaelgss/repos/game/sdl-c/src/res/zorque.ttf", 70);
  if (this->font == NULL) {
    std::cout << "Error font not loaded " << TTF_GetError() << std::endl;
    return false;
  }
  return true;
}

void Game::create_objects() {
  GameObject ball { 20, 20, 15, 15, 300, 300 };

  GameObject paddle;
  paddle.width = 100;
  paddle.height = 20;
  paddle.x = (WINDOW_WIDTH / 2) - (paddle.width / 2);
  paddle.y = WINDOW_HEIGHT - 40;
  paddle.vel_x = 0;
  paddle.vel_y = 0;

  this->objects.insert({ "ball", ball });
  this->objects.insert({ "paddle", paddle });
}

void Game::render_text(std::string text, SDL_Rect text_rect) {
  SDL_Surface* text_surface = TTF_RenderText_Solid(font, text.c_str(), { 255, 255, 255 });
  SDL_Texture* text_texture = SDL_CreateTextureFromSurface(renderer, text_surface);

  SDL_RenderCopy(renderer, text_texture, NULL, &text_rect);
}

void Game::render_texts() {
  std::string score_text = "Score: " + std::to_string(this->score);
  render_text(score_text, { (int)((WINDOW_WIDTH / 2) - 100 / 2), 40, 100, 50 });
}

void Game::render_objects() {
  for (auto& object : this->objects) {
    SDL_Rect object_rect {
      (int)object.second.x,
        (int)object.second.y,
        (int)object.second.width,
        (int)object.second.height
    };
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderFillRect(renderer, &object_rect);
  }
}

public:
Game::Game(): last_frame_time(0), score(0), font(NULL), window(NULL), renderer(NULL) {}

Game::~Game() {
  std::cout << "Game Over! Score: " << this->score << std::endl;

  TTF_CloseFont(this->font);
  TTF_Quit();
  SDL_DestroyRenderer(this->renderer);
  SDL_DestroyWindow(this->window);
  SDL_Quit();
}

bool Game::initialize() {
  if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
    std::cout << "Error initializing SDL." << std::endl;
    return false;
  }

  if (TTF_Init() != 0) {
    std::cout << "Error initializing TTF." << std::endl;
    return false;
  }

  this->window = SDLInterfaceFactory::instance()->create_window(
      { SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WINDOW_WIDTH, WINDOW_HEIGHT }
      );

  if (!this->window) {
    std::cout << "Error creating SDL Window." << std::endl;
    return false;
  }

  this->renderer = SDLInterfaceFactory::instance()->create_renderer(window);
  if (!this->renderer) {
    std::cout << "Error creating SDL Renderer." << std::endl;
    return false;
  }

  return this->setup();
}

void Game::process_input() {
  SDL_Event event;
  SDL_PollEvent(&event);

  switch (event.type) {
    case SDL_QUIT:
      this->game_is_running = false;
      break;
    case SDL_KEYDOWN:
      if (event.key.keysym.sym == SDLK_ESCAPE)
        this->game_is_running = false;
      if (event.key.keysym.sym == SDLK_LEFT)
        this->objects["paddle"].vel_x = -400;
      if (event.key.keysym.sym == SDLK_RIGHT)
        this->objects["paddle"].vel_x = +400;
      break;
    case SDL_KEYUP:
      if (event.key.keysym.sym == SDLK_LEFT || event.key.keysym.sym == SDLK_RIGHT)
        this->objects["paddle"].vel_x = 0;
      break;
  }
}

void Game::update() {
  // Waste some time / sleep until we reach the frame target time
  while (!SDL_TICKS_PASSED(SDL_GetTicks(), this->last_frame_time + FRAME_TARGET_TIME));

  // Get a delta time factor converted to seconds to be used to update my objects
  float delta_time = (SDL_GetTicks() - this->last_frame_time) / 1000.0f;

  // Store the milliseconds of the current frame
  this->last_frame_time = SDL_GetTicks();
  GameObject& ball = this->objects["ball"];
  GameObject& paddle = this->objects["paddle"];

  // Check for ball collision with the walls
  if (ball.x + ball.width > WINDOW_WIDTH || ball.x < 0)
    ball.vel_x = -ball.vel_x;
  if (ball.y < 0)
    ball.vel_y = -ball.vel_y;

  // Check for ball collision with the paddle
  if (ball.y + ball.height >= paddle.y && ball.x + ball.width > paddle.x && ball.x < paddle.x + paddle.width) {
    ball.vel_y = -ball.vel_y;
    this->score++;
  }

  // Check for game over
  if (ball.y + ball.height > WINDOW_HEIGHT)
    this->game_is_running = false;

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

void Game::render() {
  SDL_SetRenderDrawColor(this->renderer, 0, 0, 0, 255);
  SDL_RenderClear(this->renderer);

  this->render_objects();
  this->render_texts();

  SDL_RenderPresent(this->renderer);
}
