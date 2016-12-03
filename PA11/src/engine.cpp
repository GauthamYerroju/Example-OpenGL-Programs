
#include "engine.hpp"

Engine::Engine(string name, int width, int height)
{
  m_WINDOW_NAME = name;
  m_WINDOW_WIDTH = width;
  m_WINDOW_HEIGHT = height;
  m_FULLSCREEN = false;
  fps_frameCount = 0;
  fps_msCount = 0;
}

Engine::Engine(string name)
{
  m_WINDOW_NAME = name;
  m_WINDOW_HEIGHT = 0;
  m_WINDOW_WIDTH = 0;
  m_FULLSCREEN = true;
  fps_frameCount = 0;
  fps_msCount = 0;
}

Engine::~Engine()
{
  delete m_window;
  delete m_graphics;
  delete m_input;
  m_window = NULL;
  m_graphics = NULL;
}

bool Engine::Initialize(char *configFile)
{
  // Start a window
  m_window = new Window();
  if(!m_window->Initialize(m_WINDOW_NAME, &m_WINDOW_WIDTH, &m_WINDOW_HEIGHT))
  {
    printf("The window failed to initialize.\n");
    return false;
  }

  // Create input manager
  m_input = new Input();

  // Start the graphics
  m_graphics = new Graphics();
  if(!m_graphics->Initialize(m_WINDOW_WIDTH, m_WINDOW_HEIGHT, configFile))
  {
    printf("The graphics failed to initialize.\n");
    return false;
  }

  // Set the time
  m_currentTimeMillis = GetCurrentTimeMillis();


  bool selectAmbient = false;
  bool selectDiffuse = false;
  bool selectSpecular = false;
  bool selectSpotLight = false;

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

    // Update FPS if one second has passed
    fps_msCount += m_DT;
    if (fps_msCount >= 1000)
    {
      SDL_SetWindowTitle(m_window->getWindow(), (m_WINDOW_NAME + " (FPS: "+ to_string(fps_frameCount) +")").c_str() );
      fps_msCount = 0;
      fps_frameCount = 0;
    }

    // Check the keyboard input
    m_input->Update();
    Keyboard();
    // Mouse();

    // Update and render the graphics
    m_graphics->Update(m_DT, m_input);
    m_graphics->Render();

    // Swap to the Window
    m_window->Swap();

    fps_frameCount++;
  }
}

void Engine::Keyboard()
{
  if ( m_input->Quit() || m_input->KeyDown(SDLK_ESCAPE))
  {
    m_running = false;
  }
  else
  {
    float sclStepDown = 0.1;//0.80;
    float sclStepUp = 0.1;//1.25;
    
    // Switch between per-vertex and per-fragment shaders
    if (m_input->KeyDown(SDLK_1))
    {
      //'1' pressed, use per fragment lighting
      m_graphics->SetPerFragLighting();
      if (!m_graphics->SetShader())
        printf("Failed to switch shader for per fragment lighting\n");
    }
    else if (m_input->KeyDown(SDLK_2))
    {
      //'2' pressed, use per vertex lighting
      m_graphics->SetPerVertLighting();
      if (!m_graphics->SetShader())
        printf("Failed to switch shader for per vertex lighting\n");
    }

    // Set scalars
    if (m_input->KeyDown(SDLK_a))
    {
      selectAmbient = true;
      selectDiffuse = false;
      selectSpecular = false;
      selectSpotLight = false;
    }
    else if (m_input->KeyDown(SDLK_d))
    {
      selectAmbient = false;
      selectDiffuse = true;
      selectSpecular = false;
      selectSpotLight = false;
    }
    else if (m_input->KeyDown(SDLK_s))
    {
      selectAmbient = false;
      selectDiffuse = false;
      selectSpecular = true;
      selectSpotLight = false;
    }
    else if (m_input->KeyDown(SDLK_c))
    {
      selectAmbient = false;
      selectDiffuse = false;
      selectSpecular = false;
      selectSpotLight = true;
    }

    // Increase or decrease currently selected scalar
    if (m_input->KeyDown(SDLK_EQUALS))
    {
      if(selectAmbient)
        m_graphics->SetAmbientScalar(m_graphics->getAmbientScalar() + sclStepUp);
      else if(selectDiffuse)
        m_graphics->SetDiffuseScalar(m_graphics->getDiffuseScalar() + sclStepUp);
      else if(selectSpecular)
        m_graphics->SetSpecularScalar(m_graphics->getSpecularScalar() + sclStepUp);
      else if (selectSpotLight)
        m_graphics->SetSpotLightAngle(m_graphics->getSpotLightAngle() + 1);
    }
    else if (m_input->KeyDown(SDLK_MINUS))
    {
      if(selectAmbient)
        m_graphics->SetAmbientScalar(m_graphics->getAmbientScalar() - sclStepDown);
      else if(selectDiffuse)
        m_graphics->SetDiffuseScalar(m_graphics->getDiffuseScalar() - sclStepDown);
      else if(selectSpecular)
        m_graphics->SetSpecularScalar(m_graphics->getSpecularScalar() - sclStepDown);
      else if (selectSpotLight)
        m_graphics->SetSpotLightAngle(m_graphics->getSpotLightAngle() - 1);
    }
  }
}

void Engine::Mouse(){
  // if (m_input->type == SDL_MOUSEMOTION){
  // }
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
