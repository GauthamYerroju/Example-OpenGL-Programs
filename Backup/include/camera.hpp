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
		glm::vec3 cameraLocation;		  		//location of camera
		glm::vec3 cameraFocus;				//point the camera is looking at
		glm::vec3 cameraDirection; 		//vector from the focal point through the camera
		glm::vec3 up;									//upward direction of the world space
		glm::vec3 cameraRight;				//the x-axis vector of the camera
		glm::vec3 cameraUp;						//the y-axis vector of the camera
};

#endif /* CAMERA_H */
