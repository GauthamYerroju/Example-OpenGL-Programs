
#include "engine.hpp"

Engine::Engine(string name, int width, int height)
{
  m_WINDOW_NAME = name;
  m_WINDOW_WIDTH = width;
  m_WINDOW_HEIGHT = height;
  m_FULLSCREEN = false;
	viewUpdate.processed = true;
}

Engine::Engine(string name)
{
  m_WINDOW_NAME = name;
  m_WINDOW_HEIGHT = 0;
  m_WINDOW_WIDTH = 0;
  m_FULLSCREEN = true;
	viewUpdate.processed = true;
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
    m_graphics->Update(m_DT, eventFlags, viewUpdate);
		viewUpdate.processed = true;
    viewUpdate.resetPos = false;
    eventFlags.incrSpeed = false;
    eventFlags.dcrSpeed = false;
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
		viewUpdate.dt = 10;

    // handle key down events here
    switch(m_event.key.keysym.sym){
      case SDLK_ESCAPE:
        m_running = false;
        break;

      case SDLK_EQUALS:
        //'=' pressed, increase speed
        eventFlags.incrSpeed = true;
        break;

      case SDLK_MINUS:
        //'-' pressed, decrease speed
        eventFlags.dcrSpeed = true;
        break;

      case SDLK_PAUSE:
        //Pause or resume program
        if( eventFlags.pause_all == false )
        {
          eventFlags.pause_all = true;
        }

        else if( eventFlags.pause_all == true )
        { 
          eventFlags.pause_all = false;
        }
        break;

			case SDLK_w:
				//'W' pressed, move camera forward
				viewUpdate.processed = false;
				viewUpdate.type = KEY;			
				viewUpdate.direction = FORWARD;
        viewUpdate.zoom = false;
        viewUpdate.resetPos = false;
				break;

			case SDLK_a:
				//'A' pressed, move camera left
				viewUpdate.processed = false;
				viewUpdate.type = KEY;
				viewUpdate.direction = LEFT;
        viewUpdate.zoom = false;
        viewUpdate.resetPos = false;
				break;

			case SDLK_s:
				//'S' pressed, move camera backward
				viewUpdate.processed = false;
				viewUpdate.type = KEY;
				viewUpdate.direction = BACKWARD;
        viewUpdate.zoom = false;
        viewUpdate.resetPos = false;
				break;

			case SDLK_d:
				//'D' pressed, move camera right
				viewUpdate.processed = false;
				viewUpdate.type = KEY;
				viewUpdate.direction = RIGHT;
        viewUpdate.zoom = false;
        viewUpdate.resetPos = false;
				break;

      case SDLK_0:
        //'D' pressed, move camera right
        viewUpdate.zoom = true;
        viewUpdate.resetPos = false;
        viewUpdate.planetIndx = 1;
        viewUpdate.pViewScaler = 10;
        break;
      case SDLK_1:
        //'D' pressed, move camera right
        viewUpdate.zoom = true;
        viewUpdate.resetPos = false;
        viewUpdate.planetIndx = 2;
        viewUpdate.pViewScaler = 20;
        break;
      case SDLK_2:
        //'D' pressed, move camera right
        viewUpdate.zoom = true;
        viewUpdate.resetPos = false;
        viewUpdate.planetIndx = 3;
        viewUpdate.pViewScaler = 20;
        break;
      case SDLK_3:
        //'D' pressed, move camera right
        viewUpdate.zoom = true;
        viewUpdate.resetPos = false;
        viewUpdate.planetIndx = 4;
        viewUpdate.pViewScaler = 10;
        break;
       case SDLK_4:
        //'D' pressed, move camera right
        viewUpdate.zoom = true;
        viewUpdate.resetPos = false;
        viewUpdate.planetIndx = 5;
        viewUpdate.pViewScaler = 15;
        break;
      case SDLK_5:
        //'D' pressed, move camera right
        viewUpdate.zoom = true;
        viewUpdate.resetPos = false;
        viewUpdate.planetIndx = 6;
        viewUpdate.pViewScaler = 125;
        break;
      case SDLK_6:
        //'D' pressed, move camera right
        viewUpdate.zoom = true;
        viewUpdate.resetPos = false;
        viewUpdate.planetIndx = 7;
        viewUpdate.pViewScaler = 125;
        break;
      case SDLK_7:
        //'D' pressed, move camera right
        viewUpdate.zoom = true;
        viewUpdate.resetPos = false;
        viewUpdate.planetIndx = 8;
        viewUpdate.pViewScaler = 75;
        break;
      case SDLK_8:
        //'D' pressed, move camera right
        viewUpdate.zoom = true;
        viewUpdate.resetPos = false;
        viewUpdate.planetIndx = 9;
        viewUpdate.pViewScaler = 55;
        break;                 
      case SDLK_9:
        //'D' pressed, move camera right
        viewUpdate.zoom = true;
        viewUpdate.resetPos = false;
        viewUpdate.planetIndx = 10;
        viewUpdate.pViewScaler = 5;
        break;
      case SDLK_BACKSPACE:
        //'D' pressed, move camera right
        viewUpdate.zoom = false;
        viewUpdate.resetPos = true;
        break;

      default:
        break;
    }
  }
}

void Engine::Mouse(){

	//handle mouse look here
  if (m_event.type == SDL_MOUSEMOTION){

  }
  
	//handle mouse look here
  if (m_event.type == SDL_MOUSEMOTION){
		viewUpdate.type = MOUSE;
		viewUpdate.processed = false;
		viewUpdate.mouseX = m_event.motion.xrel;
		viewUpdate.mouseY = m_event.motion.yrel;
  }

	//handle zoom
	if (m_event.type == SDL_MOUSEWHEEL){
		viewUpdate.type = ZOOM;	
		viewUpdate.processed = false;
		viewUpdate.scrollY = m_event.wheel.y *.1;
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
