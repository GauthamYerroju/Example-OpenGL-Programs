#ifndef CAMERA_H
#define CAMERA_H

#include "graphics_headers.hpp"

class Camera
{
  public:
    Camera();
    ~Camera();
    bool Initialize(int w, int h);
    void Update(bool zoom);
    glm::mat4 GetProjection();
    glm::mat4 GetView();
    glm::vec3 GetPosition();
    glm::vec3 GetFocusPoint();
    void SetPosition(glm::vec3 value);
    void SetFocusPoint(glm::vec3 value);

  private:
    glm::mat4 projection;
    glm::mat4 view;
    glm::vec3 position;
    glm::vec3 focusPoint;
    int width;
    int height;
};

#endif /* CAMERA_H */
