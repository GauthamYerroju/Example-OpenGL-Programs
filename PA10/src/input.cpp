#include "input.hpp"

Input::Input()
{
  for(size_t i = 0; i < 400; i++)
  {
    keysPressed[i] = false;
    keysDown[i] = false;
    keysUp[i] = false;
  }
  modState = KMOD_NONE;
}

Input::~Input()
{
  // Clear pointers
}

void Input::Update()
{
  // Reset key down and key up flags
  for(size_t i = 0; i < 400; i++)
  {
    keysDown[i] = false;
    keysUp[i] = false;
  }
  // Collect all events and set flags
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
    modState = (SDL_Keymod) ( (int)modState | (int)m_event.key.keysym.mod );
    quit = (m_event.type == SDL_QUIT);
  }
}

bool Input::KeyPressed(SDL_Keycode sym)
{
  return keysPressed[sym];
}

bool Input::KeyDown(SDL_Keycode)
{
  return keysDown[sym];
}

bool Input::KeyUp(SDL_Keycode)
{
  return keysUp[sym];
}

SDL_Keymod Input::GetModState()
{
  return modState;
}

bool Input::Mod(SDL_Keymod mods)
{
  return modState & mods != KMOD_NONE;
}

bool Input::Quit()()
{
  return quit;
}
// unsigned int Input::getDT()
// {
//   long long TimeNowMillis = GetCurrentTimeMillis();
//   assert(TimeNowMillis >= m_currentTimeMillis);
//   unsigned int DeltaTimeMillis = (unsigned int)(TimeNowMillis - m_currentTimeMillis);
//   m_currentTimeMillis = TimeNowMillis;
//   return DeltaTimeMillis;
// }

// long long Input::GetCurrentTimeMillis()
// {
//   timeval t;
//   gettimeofday(&t, NULL);
//   long long ret = t.tv_sec * 1000 + t.tv_usec / 1000;
//   return ret;
// }
