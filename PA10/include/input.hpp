#ifndef INPUT_H
#define INPUT_H

// #include <sys/time.h>
// #include <assert.h>
// #include <vector>
#include <string>
#include <SDL2/SDL.h>

class Input
{
  public:
    Input();
    ~Input();
    void Update();
    bool Pressed(SDL_Keycode);
    bool KeyDown(SDL_Keycode);
    bool KeyUp(SDL_Keycode);
    SDL_Keymod GetModState();
    bool Mod(SDL_Keymod mods);
    bool Quit();

  private:
    SDL_Event m_event;
    bool keysPressed[400];
    SDL_Keymod modState;
    bool quit;
};

#endif // INPUT_H
