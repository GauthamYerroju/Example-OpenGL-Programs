#ifndef ENGINE_H
#define ENGINE_H

#include <sys/time.h>
#include <assert.h>
#include <vector>

#include "graphics_headers.hpp"
#include "window.hpp"
#include "graphics.hpp"
#include "input.hpp"

class Engine
{
  public:
    Engine(string name, int width, int height);
    Engine(string name);
    ~Engine();
    bool Initialize(char *configFile);
    void Run();
    void Keyboard();
    void Mouse();
    unsigned int getDT();
    long long GetCurrentTimeMillis();

  private:
    // Window related variables
    Window *m_window;
    string m_WINDOW_NAME;
    int m_WINDOW_WIDTH;
    int m_WINDOW_HEIGHT;
    bool m_FULLSCREEN;
    Input *m_input;

    Graphics *m_graphics;
    unsigned int m_DT;
    long long m_currentTimeMillis;
    bool m_running;

    bool selectAmbient;
    bool selectDiffuse;
    bool selectSpecular;
    bool selectSpotLight;

    short unsigned int fps_msCount = 0;
    short unsigned int fps_frameCount = 0;
};

#endif // ENGINE_H
