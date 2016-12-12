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
    void SetPosition(glm::vec3 value, bool tween=false);
    void SetFocusPoint(glm::vec3 value, bool tween=false);

  private:
    glm::mat4 projection;
    glm::mat4 view;
    glm::vec3 position;
    glm::vec3 focusPoint;
    glm::vec3 targetPosition;
    glm::vec3 targetFocusPoint;
    int width;
    int height;

    bool vec3Equal(glm::vec3, glm::vec3);
};

#endif /* CAMERA_H */
