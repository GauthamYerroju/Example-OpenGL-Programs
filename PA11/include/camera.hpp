#ifndef CAMERA_H
#define CAMERA_H

#include "graphics_headers.hpp"

class Camera
{
  public:
    Camera();
    ~Camera();
    bool Initialize(int w, int h);
    glm::mat4 GetProjection();
    glm::mat4 GetView(bool zoom);
    glm::vec3 GetPosition();

  private:
    glm::mat4 projection;
    glm::mat4 view;
    glm::vec3 position;
    int width;
    int height;
};

#endif /* CAMERA_H */
