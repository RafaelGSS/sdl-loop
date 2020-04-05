#ifndef ABSTRACT_GAME_H
#define ABSTRACT_GAME_H

class AbstractGame {
  protected:
    bool game_is_running = false;

  public:
    virtual bool initialize() = 0;
    virtual void process_input() = 0;
    virtual void update() = 0;
    virtual void render() = 0;

    virtual void run();
    /* virtual void subscribe(const GameEvent& event) = 0; */
};
#endif
