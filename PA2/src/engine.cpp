
#include "engine.h"

Engine::Engine(string name, int width, int height)
{
  m_WINDOW_NAME = name;
  m_WINDOW_WIDTH = width;
  m_WINDOW_HEIGHT = height;
  m_FULLSCREEN = false;
}

Engine::Engine(string name)
{
  m_WINDOW_NAME = name;
  m_WINDOW_HEIGHT = 0;
  m_WINDOW_WIDTH = 0;
  m_FULLSCREEN = true;
}

Engine::~Engine()
{
  delete m_window;
  delete m_graphics;
  m_window = NULL;
  m_graphics = NULL;
}

bool Engine::Initialize(char **argv)
{
  // Start a window
  m_window = new Window();
  if(!m_window->Initialize(m_WINDOW_NAME, &m_WINDOW_WIDTH, &m_WINDOW_HEIGHT))
  {
    printf("The window failed to initialize.\n");
    return false;
  }

  // Start the graphics
  m_graphics = new Graphics();
  if(!m_graphics->Initialize(m_WINDOW_WIDTH, m_WINDOW_HEIGHT, argv))
  {
    printf("The graphics failed to initialize.\n");
    return false;
  }

  // Set the time
  m_currentTimeMillis = GetCurrentTimeMillis();

  // No errors
  return true;
}

void Engine::Run()
{
  m_running = true;

  while(m_running)
  {
    // Update the DT
    m_DT = getDT();

    // Check the keyboard input
    while(SDL_PollEvent(&m_event) != 0)
    {
      Keyboard();
      Mouse();
    }

    // Update and render the graphics
    m_graphics->Update(m_DT, eventFlags);
    m_graphics->Render();

    // Swap to the Window
    m_window->Swap();
  }
}

void Engine::Keyboard()
{
  if(m_event.type == SDL_QUIT)
  {
    m_running = false;
  }
  else if (m_event.type == SDL_KEYDOWN)
  {
    // handle key down events here
    switch(m_event.key.keysym.sym){
      case SDLK_ESCAPE:
        m_running = false;
        break;
      case SDLK_1:
        //1 pressed, change cube rotation direction
        if( eventFlags.clockwise_rotate == false )
          eventFlags.clockwise_rotate = true;
        else if( eventFlags.clockwise_rotate == true )
          eventFlags.clockwise_rotate = false;
        break;
      case SDLK_2:
        //2 pressed, change cube orbit direction
        if( eventFlags.clockwise_orbit== false )
          eventFlags.clockwise_orbit = true;
        else if( eventFlags.clockwise_orbit == true )
          eventFlags.clockwise_orbit = false;
        break;
      case SDLK_3:
        //3 pressed, toggle (start/stop) cube rotation
        if( eventFlags.pause_rotate == false )
          eventFlags.pause_rotate = true;
        else if( eventFlags.pause_rotate == true)
          eventFlags.pause_rotate = false;
        break;
      case SDLK_4:
        //4 pressed, toggle (start/stop) cube orbit
        if( eventFlags.pause_orbit == false )
          eventFlags.pause_orbit = true;
        else if( eventFlags.pause_orbit == true )
          eventFlags.pause_orbit = false;
        break;
      case SDLK_PAUSE:
        //Pause or resume program
        if( eventFlags.pause_all == false )
          eventFlags.pause_all = true;
        else if( eventFlags.pause_all == true )
          eventFlags.pause_all = false;
        break;
      default:
        break;
    }
  }
}

void Engine::Mouse(){

  if (m_event.type == SDL_MOUSEBUTTONDOWN)
  {
    // handle mouse button down events here
    switch(m_event.button.button){
      case SDL_BUTTON_LEFT:
        //Left click, change cube rotation direction
        if( eventFlags.clockwise_rotate == false )
          eventFlags.clockwise_rotate = true;
        else if( eventFlags.clockwise_rotate == true )
          eventFlags.clockwise_rotate = false;
        break;
      case SDL_BUTTON_RIGHT:
        //Right click, change cube orbit direction
        if( eventFlags.clockwise_orbit== false )
          eventFlags.clockwise_orbit = true;
        else if( eventFlags.clockwise_orbit == true )
          eventFlags.clockwise_orbit = false;
        break;
      default:
        break;
    }
  }
}

unsigned int Engine::getDT()
{
  long long TimeNowMillis = GetCurrentTimeMillis();
  assert(TimeNowMillis >= m_currentTimeMillis);
  unsigned int DeltaTimeMillis = (unsigned int)(TimeNowMillis - m_currentTimeMillis);
  m_currentTimeMillis = TimeNowMillis;
  return DeltaTimeMillis;
}

long long Engine::GetCurrentTimeMillis()
{
  timeval t;
  gettimeofday(&t, NULL);
  long long ret = t.tv_sec * 1000 + t.tv_usec / 1000;
  return ret;
}
