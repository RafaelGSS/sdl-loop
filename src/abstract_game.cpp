/* enum GameEvent { */
/*   GAMEOVER, */
/*   START, */
/*   PAUSE, */
/*   EXIT */
/* }; */
#include "./abstract_game.h"

void AbstractGame::run() {
  this->game_is_running = this->initialize();

  while(this->game_is_running) {
    this->process_input();
    this->update();
    this->render();
  }
};
