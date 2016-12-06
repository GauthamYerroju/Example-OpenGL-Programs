#include "input.hpp"

Input::Input()
{
  InitializeCodeList();
}

void Input::Update()
{
  for( auto it = keysDown.begin(); it != keysDown.end(); it++ ) it->second = false;
  for( auto it = keysUp.begin(); it != keysUp.end(); it++ ) it->second = false;

  while(SDL_PollEvent(&m_event) != 0)
  {
    if (m_event.type == SDL_KEYDOWN)
    {
      if (keysPressed[m_event.key.keysym.sym] != true)
        keysDown[m_event.key.keysym.sym] = true;
      keysPressed[m_event.key.keysym.sym] = true;
    }
    if (m_event.type == SDL_KEYUP)
    {
      if (keysPressed[m_event.key.keysym.sym] != false) {}
        keysUp[m_event.key.keysym.sym] = true;
      keysPressed[m_event.key.keysym.sym] = false;
    }
    quit = (m_event.type == SDL_QUIT);
  }
}

bool Input::KeyPressed(SDL_Keycode sym)
{
  return keysPressed[sym];
}

bool Input::KeyDown(SDL_Keycode sym)
{
  return keysDown[sym];
}

bool Input::KeyUp(SDL_Keycode sym)
{
  return keysUp[sym];
}

SDL_Keymod Input::GetModState()
{
  return SDL_GetModState();
}

bool Input::Quit()
{
  return quit;
}

void Input::InitializeCodeList()
{
  keysPressed[SDLK_UNKNOWN] = keysDown[SDLK_UNKNOWN] = keysUp[SDLK_UNKNOWN] = false;
  keysPressed[SDLK_BACKSPACE] = keysDown[SDLK_BACKSPACE] = keysUp[SDLK_BACKSPACE] = false;
  keysPressed[SDLK_TAB] = keysDown[SDLK_TAB] = keysUp[SDLK_TAB] = false;
  keysPressed[SDLK_RETURN] = keysDown[SDLK_RETURN] = keysUp[SDLK_RETURN] = false;
  keysPressed[SDLK_ESCAPE] = keysDown[SDLK_ESCAPE] = keysUp[SDLK_ESCAPE] = false;
  keysPressed[SDLK_SPACE] = keysDown[SDLK_SPACE] = keysUp[SDLK_SPACE] = false;
  keysPressed[SDLK_EXCLAIM] = keysDown[SDLK_EXCLAIM] = keysUp[SDLK_EXCLAIM] = false;
  keysPressed[SDLK_QUOTEDBL] = keysDown[SDLK_QUOTEDBL] = keysUp[SDLK_QUOTEDBL] = false;
  keysPressed[SDLK_HASH] = keysDown[SDLK_HASH] = keysUp[SDLK_HASH] = false;
  keysPressed[SDLK_DOLLAR] = keysDown[SDLK_DOLLAR] = keysUp[SDLK_DOLLAR] = false;
  keysPressed[SDLK_PERCENT] = keysDown[SDLK_PERCENT] = keysUp[SDLK_PERCENT] = false;
  keysPressed[SDLK_AMPERSAND] = keysDown[SDLK_AMPERSAND] = keysUp[SDLK_AMPERSAND] = false;
  keysPressed[SDLK_QUOTE] = keysDown[SDLK_QUOTE] = keysUp[SDLK_QUOTE] = false;
  keysPressed[SDLK_LEFTPAREN] = keysDown[SDLK_LEFTPAREN] = keysUp[SDLK_LEFTPAREN] = false;
  keysPressed[SDLK_RIGHTPAREN] = keysDown[SDLK_RIGHTPAREN] = keysUp[SDLK_RIGHTPAREN] = false;
  keysPressed[SDLK_ASTERISK] = keysDown[SDLK_ASTERISK] = keysUp[SDLK_ASTERISK] = false;
  keysPressed[SDLK_PLUS] = keysDown[SDLK_PLUS] = keysUp[SDLK_PLUS] = false;
  keysPressed[SDLK_COMMA] = keysDown[SDLK_COMMA] = keysUp[SDLK_COMMA] = false;
  keysPressed[SDLK_MINUS] = keysDown[SDLK_MINUS] = keysUp[SDLK_MINUS] = false;
  keysPressed[SDLK_PERIOD] = keysDown[SDLK_PERIOD] = keysUp[SDLK_PERIOD] = false;
  keysPressed[SDLK_SLASH] = keysDown[SDLK_SLASH] = keysUp[SDLK_SLASH] = false;
  keysPressed[SDLK_0] = keysDown[SDLK_0] = keysUp[SDLK_0] = false;
  keysPressed[SDLK_1] = keysDown[SDLK_1] = keysUp[SDLK_1] = false;
  keysPressed[SDLK_2] = keysDown[SDLK_2] = keysUp[SDLK_2] = false;
  keysPressed[SDLK_3] = keysDown[SDLK_3] = keysUp[SDLK_3] = false;
  keysPressed[SDLK_4] = keysDown[SDLK_4] = keysUp[SDLK_4] = false;
  keysPressed[SDLK_5] = keysDown[SDLK_5] = keysUp[SDLK_5] = false;
  keysPressed[SDLK_6] = keysDown[SDLK_6] = keysUp[SDLK_6] = false;
  keysPressed[SDLK_7] = keysDown[SDLK_7] = keysUp[SDLK_7] = false;
  keysPressed[SDLK_8] = keysDown[SDLK_8] = keysUp[SDLK_8] = false;
  keysPressed[SDLK_9] = keysDown[SDLK_9] = keysUp[SDLK_9] = false;
  keysPressed[SDLK_COLON] = keysDown[SDLK_COLON] = keysUp[SDLK_COLON] = false;
  keysPressed[SDLK_SEMICOLON] = keysDown[SDLK_SEMICOLON] = keysUp[SDLK_SEMICOLON] = false;
  keysPressed[SDLK_LESS] = keysDown[SDLK_LESS] = keysUp[SDLK_LESS] = false;
  keysPressed[SDLK_EQUALS] = keysDown[SDLK_EQUALS] = keysUp[SDLK_EQUALS] = false;
  keysPressed[SDLK_GREATER] = keysDown[SDLK_GREATER] = keysUp[SDLK_GREATER] = false;
  keysPressed[SDLK_QUESTION] = keysDown[SDLK_QUESTION] = keysUp[SDLK_QUESTION] = false;
  keysPressed[SDLK_AT] = keysDown[SDLK_AT] = keysUp[SDLK_AT] = false;
  keysPressed[SDLK_LEFTBRACKET] = keysDown[SDLK_LEFTBRACKET] = keysUp[SDLK_LEFTBRACKET] = false;
  keysPressed[SDLK_BACKSLASH] = keysDown[SDLK_BACKSLASH] = keysUp[SDLK_BACKSLASH] = false;
  keysPressed[SDLK_RIGHTBRACKET] = keysDown[SDLK_RIGHTBRACKET] = keysUp[SDLK_RIGHTBRACKET] = false;
  keysPressed[SDLK_CARET] = keysDown[SDLK_CARET] = keysUp[SDLK_CARET] = false;
  keysPressed[SDLK_UNDERSCORE] = keysDown[SDLK_UNDERSCORE] = keysUp[SDLK_UNDERSCORE] = false;
  keysPressed[SDLK_BACKQUOTE] = keysDown[SDLK_BACKQUOTE] = keysUp[SDLK_BACKQUOTE] = false;
  keysPressed[SDLK_a] = keysDown[SDLK_a] = keysUp[SDLK_a] = false;
  keysPressed[SDLK_b] = keysDown[SDLK_b] = keysUp[SDLK_b] = false;
  keysPressed[SDLK_c] = keysDown[SDLK_c] = keysUp[SDLK_c] = false;
  keysPressed[SDLK_d] = keysDown[SDLK_d] = keysUp[SDLK_d] = false;
  keysPressed[SDLK_e] = keysDown[SDLK_e] = keysUp[SDLK_e] = false;
  keysPressed[SDLK_f] = keysDown[SDLK_f] = keysUp[SDLK_f] = false;
  keysPressed[SDLK_g] = keysDown[SDLK_g] = keysUp[SDLK_g] = false;
  keysPressed[SDLK_h] = keysDown[SDLK_h] = keysUp[SDLK_h] = false;
  keysPressed[SDLK_i] = keysDown[SDLK_i] = keysUp[SDLK_i] = false;
  keysPressed[SDLK_j] = keysDown[SDLK_j] = keysUp[SDLK_j] = false;
  keysPressed[SDLK_k] = keysDown[SDLK_k] = keysUp[SDLK_k] = false;
  keysPressed[SDLK_l] = keysDown[SDLK_l] = keysUp[SDLK_l] = false;
  keysPressed[SDLK_m] = keysDown[SDLK_m] = keysUp[SDLK_m] = false;
  keysPressed[SDLK_n] = keysDown[SDLK_n] = keysUp[SDLK_n] = false;
  keysPressed[SDLK_o] = keysDown[SDLK_o] = keysUp[SDLK_o] = false;
  keysPressed[SDLK_p] = keysDown[SDLK_p] = keysUp[SDLK_p] = false;
  keysPressed[SDLK_q] = keysDown[SDLK_q] = keysUp[SDLK_q] = false;
  keysPressed[SDLK_r] = keysDown[SDLK_r] = keysUp[SDLK_r] = false;
  keysPressed[SDLK_s] = keysDown[SDLK_s] = keysUp[SDLK_s] = false;
  keysPressed[SDLK_t] = keysDown[SDLK_t] = keysUp[SDLK_t] = false;
  keysPressed[SDLK_u] = keysDown[SDLK_u] = keysUp[SDLK_u] = false;
  keysPressed[SDLK_v] = keysDown[SDLK_v] = keysUp[SDLK_v] = false;
  keysPressed[SDLK_w] = keysDown[SDLK_w] = keysUp[SDLK_w] = false;
  keysPressed[SDLK_x] = keysDown[SDLK_x] = keysUp[SDLK_x] = false;
  keysPressed[SDLK_y] = keysDown[SDLK_y] = keysUp[SDLK_y] = false;
  keysPressed[SDLK_z] = keysDown[SDLK_z] = keysUp[SDLK_z] = false;
  keysPressed[SDLK_DELETE] = keysDown[SDLK_DELETE] = keysUp[SDLK_DELETE] = false;
  keysPressed[SDLK_CAPSLOCK] = keysDown[SDLK_CAPSLOCK] = keysUp[SDLK_CAPSLOCK] = false;
  keysPressed[SDLK_F1] = keysDown[SDLK_F1] = keysUp[SDLK_F1] = false;
  keysPressed[SDLK_F2] = keysDown[SDLK_F2] = keysUp[SDLK_F2] = false;
  keysPressed[SDLK_F3] = keysDown[SDLK_F3] = keysUp[SDLK_F3] = false;
  keysPressed[SDLK_F4] = keysDown[SDLK_F4] = keysUp[SDLK_F4] = false;
  keysPressed[SDLK_F5] = keysDown[SDLK_F5] = keysUp[SDLK_F5] = false;
  keysPressed[SDLK_F6] = keysDown[SDLK_F6] = keysUp[SDLK_F6] = false;
  keysPressed[SDLK_F7] = keysDown[SDLK_F7] = keysUp[SDLK_F7] = false;
  keysPressed[SDLK_F8] = keysDown[SDLK_F8] = keysUp[SDLK_F8] = false;
  keysPressed[SDLK_F9] = keysDown[SDLK_F9] = keysUp[SDLK_F9] = false;
  keysPressed[SDLK_F10] = keysDown[SDLK_F10] = keysUp[SDLK_F10] = false;
  keysPressed[SDLK_F11] = keysDown[SDLK_F11] = keysUp[SDLK_F11] = false;
  keysPressed[SDLK_F12] = keysDown[SDLK_F12] = keysUp[SDLK_F12] = false;
  keysPressed[SDLK_PRINTSCREEN] = keysDown[SDLK_PRINTSCREEN] = keysUp[SDLK_PRINTSCREEN] = false;
  keysPressed[SDLK_SCROLLLOCK] = keysDown[SDLK_SCROLLLOCK] = keysUp[SDLK_SCROLLLOCK] = false;
  keysPressed[SDLK_PAUSE] = keysDown[SDLK_PAUSE] = keysUp[SDLK_PAUSE] = false;
  keysPressed[SDLK_INSERT] = keysDown[SDLK_INSERT] = keysUp[SDLK_INSERT] = false;
  keysPressed[SDLK_HOME] = keysDown[SDLK_HOME] = keysUp[SDLK_HOME] = false;
  keysPressed[SDLK_PAGEUP] = keysDown[SDLK_PAGEUP] = keysUp[SDLK_PAGEUP] = false;
  keysPressed[SDLK_END] = keysDown[SDLK_END] = keysUp[SDLK_END] = false;
  keysPressed[SDLK_PAGEDOWN] = keysDown[SDLK_PAGEDOWN] = keysUp[SDLK_PAGEDOWN] = false;
  keysPressed[SDLK_RIGHT] = keysDown[SDLK_RIGHT] = keysUp[SDLK_RIGHT] = false;
  keysPressed[SDLK_LEFT] = keysDown[SDLK_LEFT] = keysUp[SDLK_LEFT] = false;
  keysPressed[SDLK_DOWN] = keysDown[SDLK_DOWN] = keysUp[SDLK_DOWN] = false;
  keysPressed[SDLK_UP] = keysDown[SDLK_UP] = keysUp[SDLK_UP] = false;
  keysPressed[SDLK_NUMLOCKCLEAR] = keysDown[SDLK_NUMLOCKCLEAR] = keysUp[SDLK_NUMLOCKCLEAR] = false;
  keysPressed[SDLK_KP_DIVIDE] = keysDown[SDLK_KP_DIVIDE] = keysUp[SDLK_KP_DIVIDE] = false;
  keysPressed[SDLK_KP_MULTIPLY] = keysDown[SDLK_KP_MULTIPLY] = keysUp[SDLK_KP_MULTIPLY] = false;
  keysPressed[SDLK_KP_MINUS] = keysDown[SDLK_KP_MINUS] = keysUp[SDLK_KP_MINUS] = false;
  keysPressed[SDLK_KP_PLUS] = keysDown[SDLK_KP_PLUS] = keysUp[SDLK_KP_PLUS] = false;
  keysPressed[SDLK_KP_ENTER] = keysDown[SDLK_KP_ENTER] = keysUp[SDLK_KP_ENTER] = false;
  keysPressed[SDLK_KP_1] = keysDown[SDLK_KP_1] = keysUp[SDLK_KP_1] = false;
  keysPressed[SDLK_KP_2] = keysDown[SDLK_KP_2] = keysUp[SDLK_KP_2] = false;
  keysPressed[SDLK_KP_3] = keysDown[SDLK_KP_3] = keysUp[SDLK_KP_3] = false;
  keysPressed[SDLK_KP_4] = keysDown[SDLK_KP_4] = keysUp[SDLK_KP_4] = false;
  keysPressed[SDLK_KP_5] = keysDown[SDLK_KP_5] = keysUp[SDLK_KP_5] = false;
  keysPressed[SDLK_KP_6] = keysDown[SDLK_KP_6] = keysUp[SDLK_KP_6] = false;
  keysPressed[SDLK_KP_7] = keysDown[SDLK_KP_7] = keysUp[SDLK_KP_7] = false;
  keysPressed[SDLK_KP_8] = keysDown[SDLK_KP_8] = keysUp[SDLK_KP_8] = false;
  keysPressed[SDLK_KP_9] = keysDown[SDLK_KP_9] = keysUp[SDLK_KP_9] = false;
  keysPressed[SDLK_KP_0] = keysDown[SDLK_KP_0] = keysUp[SDLK_KP_0] = false;
  keysPressed[SDLK_KP_PERIOD] = keysDown[SDLK_KP_PERIOD] = keysUp[SDLK_KP_PERIOD] = false;
  keysPressed[SDLK_APPLICATION] = keysDown[SDLK_APPLICATION] = keysUp[SDLK_APPLICATION] = false;
  keysPressed[SDLK_POWER] = keysDown[SDLK_POWER] = keysUp[SDLK_POWER] = false;
  keysPressed[SDLK_KP_EQUALS] = keysDown[SDLK_KP_EQUALS] = keysUp[SDLK_KP_EQUALS] = false;
  keysPressed[SDLK_F13] = keysDown[SDLK_F13] = keysUp[SDLK_F13] = false;
  keysPressed[SDLK_F14] = keysDown[SDLK_F14] = keysUp[SDLK_F14] = false;
  keysPressed[SDLK_F15] = keysDown[SDLK_F15] = keysUp[SDLK_F15] = false;
  keysPressed[SDLK_F16] = keysDown[SDLK_F16] = keysUp[SDLK_F16] = false;
  keysPressed[SDLK_F17] = keysDown[SDLK_F17] = keysUp[SDLK_F17] = false;
  keysPressed[SDLK_F18] = keysDown[SDLK_F18] = keysUp[SDLK_F18] = false;
  keysPressed[SDLK_F19] = keysDown[SDLK_F19] = keysUp[SDLK_F19] = false;
  keysPressed[SDLK_F20] = keysDown[SDLK_F20] = keysUp[SDLK_F20] = false;
  keysPressed[SDLK_F21] = keysDown[SDLK_F21] = keysUp[SDLK_F21] = false;
  keysPressed[SDLK_F22] = keysDown[SDLK_F22] = keysUp[SDLK_F22] = false;
  keysPressed[SDLK_F23] = keysDown[SDLK_F23] = keysUp[SDLK_F23] = false;
  keysPressed[SDLK_F24] = keysDown[SDLK_F24] = keysUp[SDLK_F24] = false;
  keysPressed[SDLK_EXECUTE] = keysDown[SDLK_EXECUTE] = keysUp[SDLK_EXECUTE] = false;
  keysPressed[SDLK_HELP] = keysDown[SDLK_HELP] = keysUp[SDLK_HELP] = false;
  keysPressed[SDLK_MENU] = keysDown[SDLK_MENU] = keysUp[SDLK_MENU] = false;
  keysPressed[SDLK_SELECT] = keysDown[SDLK_SELECT] = keysUp[SDLK_SELECT] = false;
  keysPressed[SDLK_STOP] = keysDown[SDLK_STOP] = keysUp[SDLK_STOP] = false;
  keysPressed[SDLK_AGAIN] = keysDown[SDLK_AGAIN] = keysUp[SDLK_AGAIN] = false;
  keysPressed[SDLK_UNDO] = keysDown[SDLK_UNDO] = keysUp[SDLK_UNDO] = false;
  keysPressed[SDLK_CUT] = keysDown[SDLK_CUT] = keysUp[SDLK_CUT] = false;
  keysPressed[SDLK_COPY] = keysDown[SDLK_COPY] = keysUp[SDLK_COPY] = false;
  keysPressed[SDLK_PASTE] = keysDown[SDLK_PASTE] = keysUp[SDLK_PASTE] = false;
  keysPressed[SDLK_FIND] = keysDown[SDLK_FIND] = keysUp[SDLK_FIND] = false;
  keysPressed[SDLK_MUTE] = keysDown[SDLK_MUTE] = keysUp[SDLK_MUTE] = false;
  keysPressed[SDLK_VOLUMEUP] = keysDown[SDLK_VOLUMEUP] = keysUp[SDLK_VOLUMEUP] = false;
  keysPressed[SDLK_VOLUMEDOWN] = keysDown[SDLK_VOLUMEDOWN] = keysUp[SDLK_VOLUMEDOWN] = false;
  keysPressed[SDLK_KP_COMMA] = keysDown[SDLK_KP_COMMA] = keysUp[SDLK_KP_COMMA] = false;
  keysPressed[SDLK_KP_EQUALSAS400] = keysDown[SDLK_KP_EQUALSAS400] = keysUp[SDLK_KP_EQUALSAS400] = false;
  keysPressed[SDLK_ALTERASE] = keysDown[SDLK_ALTERASE] = keysUp[SDLK_ALTERASE] = false;
  keysPressed[SDLK_SYSREQ] = keysDown[SDLK_SYSREQ] = keysUp[SDLK_SYSREQ] = false;
  keysPressed[SDLK_CANCEL] = keysDown[SDLK_CANCEL] = keysUp[SDLK_CANCEL] = false;
  keysPressed[SDLK_CLEAR] = keysDown[SDLK_CLEAR] = keysUp[SDLK_CLEAR] = false;
  keysPressed[SDLK_PRIOR] = keysDown[SDLK_PRIOR] = keysUp[SDLK_PRIOR] = false;
  keysPressed[SDLK_RETURN2] = keysDown[SDLK_RETURN2] = keysUp[SDLK_RETURN2] = false;
  keysPressed[SDLK_SEPARATOR] = keysDown[SDLK_SEPARATOR] = keysUp[SDLK_SEPARATOR] = false;
  keysPressed[SDLK_OUT] = keysDown[SDLK_OUT] = keysUp[SDLK_OUT] = false;
  keysPressed[SDLK_OPER] = keysDown[SDLK_OPER] = keysUp[SDLK_OPER] = false;
  keysPressed[SDLK_CLEARAGAIN] = keysDown[SDLK_CLEARAGAIN] = keysUp[SDLK_CLEARAGAIN] = false;
  keysPressed[SDLK_CRSEL] = keysDown[SDLK_CRSEL] = keysUp[SDLK_CRSEL] = false;
  keysPressed[SDLK_EXSEL] = keysDown[SDLK_EXSEL] = keysUp[SDLK_EXSEL] = false;
  keysPressed[SDLK_KP_00] = keysDown[SDLK_KP_00] = keysUp[SDLK_KP_00] = false;
  keysPressed[SDLK_KP_000] = keysDown[SDLK_KP_000] = keysUp[SDLK_KP_000] = false;
  keysPressed[SDLK_THOUSANDSSEPARATOR] = keysDown[SDLK_THOUSANDSSEPARATOR] = keysUp[SDLK_THOUSANDSSEPARATOR] = false;
  keysPressed[SDLK_DECIMALSEPARATOR] = keysDown[SDLK_DECIMALSEPARATOR] = keysUp[SDLK_DECIMALSEPARATOR] = false;
  keysPressed[SDLK_CURRENCYUNIT] = keysDown[SDLK_CURRENCYUNIT] = keysUp[SDLK_CURRENCYUNIT] = false;
  keysPressed[SDLK_CURRENCYSUBUNIT] = keysDown[SDLK_CURRENCYSUBUNIT] = keysUp[SDLK_CURRENCYSUBUNIT] = false;
  keysPressed[SDLK_KP_LEFTPAREN] = keysDown[SDLK_KP_LEFTPAREN] = keysUp[SDLK_KP_LEFTPAREN] = false;
  keysPressed[SDLK_KP_RIGHTPAREN] = keysDown[SDLK_KP_RIGHTPAREN] = keysUp[SDLK_KP_RIGHTPAREN] = false;
  keysPressed[SDLK_KP_LEFTBRACE] = keysDown[SDLK_KP_LEFTBRACE] = keysUp[SDLK_KP_LEFTBRACE] = false;
  keysPressed[SDLK_KP_RIGHTBRACE] = keysDown[SDLK_KP_RIGHTBRACE] = keysUp[SDLK_KP_RIGHTBRACE] = false;
  keysPressed[SDLK_KP_TAB] = keysDown[SDLK_KP_TAB] = keysUp[SDLK_KP_TAB] = false;
  keysPressed[SDLK_KP_BACKSPACE] = keysDown[SDLK_KP_BACKSPACE] = keysUp[SDLK_KP_BACKSPACE] = false;
  keysPressed[SDLK_KP_A] = keysDown[SDLK_KP_A] = keysUp[SDLK_KP_A] = false;
  keysPressed[SDLK_KP_B] = keysDown[SDLK_KP_B] = keysUp[SDLK_KP_B] = false;
  keysPressed[SDLK_KP_C] = keysDown[SDLK_KP_C] = keysUp[SDLK_KP_C] = false;
  keysPressed[SDLK_KP_D] = keysDown[SDLK_KP_D] = keysUp[SDLK_KP_D] = false;
  keysPressed[SDLK_KP_E] = keysDown[SDLK_KP_E] = keysUp[SDLK_KP_E] = false;
  keysPressed[SDLK_KP_F] = keysDown[SDLK_KP_F] = keysUp[SDLK_KP_F] = false;
  keysPressed[SDLK_KP_XOR] = keysDown[SDLK_KP_XOR] = keysUp[SDLK_KP_XOR] = false;
  keysPressed[SDLK_KP_POWER] = keysDown[SDLK_KP_POWER] = keysUp[SDLK_KP_POWER] = false;
  keysPressed[SDLK_KP_PERCENT] = keysDown[SDLK_KP_PERCENT] = keysUp[SDLK_KP_PERCENT] = false;
  keysPressed[SDLK_KP_LESS] = keysDown[SDLK_KP_LESS] = keysUp[SDLK_KP_LESS] = false;
  keysPressed[SDLK_KP_GREATER] = keysDown[SDLK_KP_GREATER] = keysUp[SDLK_KP_GREATER] = false;
  keysPressed[SDLK_KP_AMPERSAND] = keysDown[SDLK_KP_AMPERSAND] = keysUp[SDLK_KP_AMPERSAND] = false;
  keysPressed[SDLK_KP_DBLAMPERSAND] = keysDown[SDLK_KP_DBLAMPERSAND] = keysUp[SDLK_KP_DBLAMPERSAND] = false;
  keysPressed[SDLK_KP_VERTICALBAR] = keysDown[SDLK_KP_VERTICALBAR] = keysUp[SDLK_KP_VERTICALBAR] = false;
  keysPressed[SDLK_KP_DBLVERTICALBAR] = keysDown[SDLK_KP_DBLVERTICALBAR] = keysUp[SDLK_KP_DBLVERTICALBAR] = false;
  keysPressed[SDLK_KP_COLON] = keysDown[SDLK_KP_COLON] = keysUp[SDLK_KP_COLON] = false;
  keysPressed[SDLK_KP_HASH] = keysDown[SDLK_KP_HASH] = keysUp[SDLK_KP_HASH] = false;
  keysPressed[SDLK_KP_SPACE] = keysDown[SDLK_KP_SPACE] = keysUp[SDLK_KP_SPACE] = false;
  keysPressed[SDLK_KP_AT] = keysDown[SDLK_KP_AT] = keysUp[SDLK_KP_AT] = false;
  keysPressed[SDLK_KP_EXCLAM] = keysDown[SDLK_KP_EXCLAM] = keysUp[SDLK_KP_EXCLAM] = false;
  keysPressed[SDLK_KP_MEMSTORE] = keysDown[SDLK_KP_MEMSTORE] = keysUp[SDLK_KP_MEMSTORE] = false;
  keysPressed[SDLK_KP_MEMRECALL] = keysDown[SDLK_KP_MEMRECALL] = keysUp[SDLK_KP_MEMRECALL] = false;
  keysPressed[SDLK_KP_MEMCLEAR] = keysDown[SDLK_KP_MEMCLEAR] = keysUp[SDLK_KP_MEMCLEAR] = false;
  keysPressed[SDLK_KP_MEMADD] = keysDown[SDLK_KP_MEMADD] = keysUp[SDLK_KP_MEMADD] = false;
  keysPressed[SDLK_KP_MEMSUBTRACT] = keysDown[SDLK_KP_MEMSUBTRACT] = keysUp[SDLK_KP_MEMSUBTRACT] = false;
  keysPressed[SDLK_KP_MEMMULTIPLY] = keysDown[SDLK_KP_MEMMULTIPLY] = keysUp[SDLK_KP_MEMMULTIPLY] = false;
  keysPressed[SDLK_KP_MEMDIVIDE] = keysDown[SDLK_KP_MEMDIVIDE] = keysUp[SDLK_KP_MEMDIVIDE] = false;
  keysPressed[SDLK_KP_PLUSMINUS] = keysDown[SDLK_KP_PLUSMINUS] = keysUp[SDLK_KP_PLUSMINUS] = false;
  keysPressed[SDLK_KP_CLEAR] = keysDown[SDLK_KP_CLEAR] = keysUp[SDLK_KP_CLEAR] = false;
  keysPressed[SDLK_KP_CLEARENTRY] = keysDown[SDLK_KP_CLEARENTRY] = keysUp[SDLK_KP_CLEARENTRY] = false;
  keysPressed[SDLK_KP_BINARY] = keysDown[SDLK_KP_BINARY] = keysUp[SDLK_KP_BINARY] = false;
  keysPressed[SDLK_KP_OCTAL] = keysDown[SDLK_KP_OCTAL] = keysUp[SDLK_KP_OCTAL] = false;
  keysPressed[SDLK_KP_DECIMAL] = keysDown[SDLK_KP_DECIMAL] = keysUp[SDLK_KP_DECIMAL] = false;
  keysPressed[SDLK_KP_HEXADECIMAL] = keysDown[SDLK_KP_HEXADECIMAL] = keysUp[SDLK_KP_HEXADECIMAL] = false;
  keysPressed[SDLK_LCTRL] = keysDown[SDLK_LCTRL] = keysUp[SDLK_LCTRL] = false;
  keysPressed[SDLK_LSHIFT] = keysDown[SDLK_LSHIFT] = keysUp[SDLK_LSHIFT] = false;
  keysPressed[SDLK_LALT] = keysDown[SDLK_LALT] = keysUp[SDLK_LALT] = false;
  keysPressed[SDLK_LGUI] = keysDown[SDLK_LGUI] = keysUp[SDLK_LGUI] = false;
  keysPressed[SDLK_RCTRL] = keysDown[SDLK_RCTRL] = keysUp[SDLK_RCTRL] = false;
  keysPressed[SDLK_RSHIFT] = keysDown[SDLK_RSHIFT] = keysUp[SDLK_RSHIFT] = false;
  keysPressed[SDLK_RALT] = keysDown[SDLK_RALT] = keysUp[SDLK_RALT] = false;
  keysPressed[SDLK_RGUI] = keysDown[SDLK_RGUI] = keysUp[SDLK_RGUI] = false;
  keysPressed[SDLK_MODE] = keysDown[SDLK_MODE] = keysUp[SDLK_MODE] = false;
  keysPressed[SDLK_AUDIONEXT] = keysDown[SDLK_AUDIONEXT] = keysUp[SDLK_AUDIONEXT] = false;
  keysPressed[SDLK_AUDIOPREV] = keysDown[SDLK_AUDIOPREV] = keysUp[SDLK_AUDIOPREV] = false;
  keysPressed[SDLK_AUDIOSTOP] = keysDown[SDLK_AUDIOSTOP] = keysUp[SDLK_AUDIOSTOP] = false;
  keysPressed[SDLK_AUDIOPLAY] = keysDown[SDLK_AUDIOPLAY] = keysUp[SDLK_AUDIOPLAY] = false;
  keysPressed[SDLK_AUDIOMUTE] = keysDown[SDLK_AUDIOMUTE] = keysUp[SDLK_AUDIOMUTE] = false;
  keysPressed[SDLK_MEDIASELECT] = keysDown[SDLK_MEDIASELECT] = keysUp[SDLK_MEDIASELECT] = false;
  keysPressed[SDLK_WWW] = keysDown[SDLK_WWW] = keysUp[SDLK_WWW] = false;
  keysPressed[SDLK_MAIL] = keysDown[SDLK_MAIL] = keysUp[SDLK_MAIL] = false;
  keysPressed[SDLK_CALCULATOR] = keysDown[SDLK_CALCULATOR] = keysUp[SDLK_CALCULATOR] = false;
  keysPressed[SDLK_COMPUTER] = keysDown[SDLK_COMPUTER] = keysUp[SDLK_COMPUTER] = false;
  keysPressed[SDLK_AC_SEARCH] = keysDown[SDLK_AC_SEARCH] = keysUp[SDLK_AC_SEARCH] = false;
  keysPressed[SDLK_AC_HOME] = keysDown[SDLK_AC_HOME] = keysUp[SDLK_AC_HOME] = false;
  keysPressed[SDLK_AC_BACK] = keysDown[SDLK_AC_BACK] = keysUp[SDLK_AC_BACK] = false;
  keysPressed[SDLK_AC_FORWARD] = keysDown[SDLK_AC_FORWARD] = keysUp[SDLK_AC_FORWARD] = false;
  keysPressed[SDLK_AC_STOP] = keysDown[SDLK_AC_STOP] = keysUp[SDLK_AC_STOP] = false;
  keysPressed[SDLK_AC_REFRESH] = keysDown[SDLK_AC_REFRESH] = keysUp[SDLK_AC_REFRESH] = false;
  keysPressed[SDLK_AC_BOOKMARKS] = keysDown[SDLK_AC_BOOKMARKS] = keysUp[SDLK_AC_BOOKMARKS] = false;
  keysPressed[SDLK_BRIGHTNESSDOWN] = keysDown[SDLK_BRIGHTNESSDOWN] = keysUp[SDLK_BRIGHTNESSDOWN] = false;
  keysPressed[SDLK_BRIGHTNESSUP] = keysDown[SDLK_BRIGHTNESSUP] = keysUp[SDLK_BRIGHTNESSUP] = false;
  keysPressed[SDLK_DISPLAYSWITCH] = keysDown[SDLK_DISPLAYSWITCH] = keysUp[SDLK_DISPLAYSWITCH] = false;
  keysPressed[SDLK_KBDILLUMTOGGLE] = keysDown[SDLK_KBDILLUMTOGGLE] = keysUp[SDLK_KBDILLUMTOGGLE] = false;
  keysPressed[SDLK_KBDILLUMDOWN] = keysDown[SDLK_KBDILLUMDOWN] = keysUp[SDLK_KBDILLUMDOWN] = false;
  keysPressed[SDLK_KBDILLUMUP] = keysDown[SDLK_KBDILLUMUP] = keysUp[SDLK_KBDILLUMUP] = false;
  keysPressed[SDLK_EJECT] = keysDown[SDLK_EJECT] = keysUp[SDLK_EJECT] = false;
  keysPressed[SDLK_SLEEP] = keysDown[SDLK_SLEEP] = keysUp[SDLK_SLEEP] = false;
}