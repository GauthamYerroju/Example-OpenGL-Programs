#ifndef CAMERA_H
#define CAMERA_H

#include "graphics_headers.hpp"

class Camera
{
  public:
    Camera();
    ~Camera();
    bool Initialize(int w, int h);
		void Update(ViewUpdate viewUpdate);
    glm::mat4 GetProjection();
    glm::mat4 GetView();
  
  private:
    glm::mat4 projection;
    glm::mat4 view;
		glm::vec3 cameraLocation;
		glm::vec3 cameraFocus;
		glm::vec3 cameraUp;
};

#endif /* CAMERA_H */
