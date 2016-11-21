#include "input.hpp"

void Input::Update()
{
  while(SDL_PollEvent(&m_event) != 0)
  {
    if (m_event.type == SDL_KEYDOWN)
    {
      keysPressed[m_event.key.keysym.sym] = true;
      keysDown[m_event.key.keysym.sym] = true;
    }
    if (m_event.type == SDL_KEYUP)
    {
      keysPressed[m_event.key.keysym.sym] = false;
      keysUp[m_event.key.keysym.sym] = true;
    }
    quit = (m_event.type == SDL_QUIT);
  }
}

bool Input::KeyPressed(SDL_Keycode sym)
{
  return (keysPressed.count(sym) > 0) && keysPressed[sym];
}

bool Input::KeyDown(SDL_Keycode sym)
{
  return (keysDown.count(sym) > 0) && keysDown[sym];
}

bool Input::KeyUp(SDL_Keycode sym)
{
  return (keysUp.count(sym) > 0) && keysUp[sym];
}

SDL_Keymod Input::GetModState()
{
  return SDL_GetModState();
}

bool Input::Quit()
{
  return quit;
}
