#ifndef GRAPHICS_H
#define GRAPHICS_H

#include <iostream>
using namespace std;

#include "graphics_headers.hpp"
#include "camera.hpp"
#include "shader.hpp"
#include "object.hpp"

class Graphics
{
  public:
    Graphics();
    ~Graphics();
    bool Initialize(int width, int height, char **argv);
    void Update(unsigned int dt, vector<EventFlag> e_flags);
    void Render();

  private:
    std::string ErrorString(GLenum error);

    Camera *m_camera;
    Shader *m_shader;

    GLint m_projectionMatrix;
    GLint m_viewMatrix;
    GLint m_modelMatrix;

    Object *sun;
    Object *mercury;
    Object *venus;
    Object *earth;
    Object *moon;

};

#endif /* GRAPHICS_H */
