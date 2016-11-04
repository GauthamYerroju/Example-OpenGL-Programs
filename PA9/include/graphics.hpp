#ifndef GRAPHICS_H
#define GRAPHICS_H

#include <iostream>
using namespace std;

#include <SDL2/SDL.h>
#include "graphics_headers.hpp"
#include "camera.hpp"
#include "shader.hpp"
#include "object.hpp"
#include "physicsObject.hpp"
#include "physicsWorld.hpp"

// For the config file
#include "json.hpp"
using json = nlohmann::json;

class Graphics
{
  public:
    Graphics();
    ~Graphics();
    bool Initialize(int width, int height, char *configFile);
    bool LoadConfig( char *configFile );
    bool SetShader();
    void SetPerFragLighting();
    void SetPerVertLighting();
    void Update(unsigned int dt, SDL_Event *m_event);
    void HandleInput(SDL_Event *m_event);
    void Render();


  private:
    std::string ErrorString(GLenum error);
    bool impulseFlag;
    bool perFragmentLighting;

    Camera *m_camera;
    Shader *m_shader;

    GLint m_projectionMatrix;
    GLint m_viewMatrix;
    GLint m_modelMatrix;

    PhysicsObject                         *board;
    PhysicsObject                         *ball;
    PhysicsObject                         *paddle;
    PhysicsObject                         *bumper;
    PhysicsObject                         *dome;
    PhysicsWorld                          world;

    std::string perVert_vShaderFile;
    std::string perVert_fShaderFile;
    std::string perFrag_vShaderFile;
    std::string perFrag_fShaderFile;

};

#endif /* GRAPHICS_H */
