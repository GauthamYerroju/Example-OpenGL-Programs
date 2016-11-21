#ifndef INPUT_H
#define INPUT_H

// #include <sys/time.h>
// #include <assert.h>
// #include <vector>
#include <SDL2/SDL.h>
#include <map>

class Input
{
  public:
    Input();
    void InitializeCodeList();
    void Update();
    bool KeyPressed(SDL_Keycode);
    bool KeyDown(SDL_Keycode);
    bool KeyUp(SDL_Keycode);
    SDL_Keymod GetModState();
    bool Quit();

  private:
    SDL_Event m_event;
    std::map< SDL_Keycode, bool > keysPressed;
    std::map< SDL_Keycode, bool > keysDown;
    std::map< SDL_Keycode, bool > keysUp;
    bool quit;
};

#endif // INPUT_H
