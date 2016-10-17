#ifndef CAMERA_H
#define CAMERA_H

#include "graphics_headers.hpp"

class Camera
{
  public:
    Camera();
    ~Camera();
    bool Initialize(int w, int h);
		void UpdateVectors();
		void ProcessInput(ViewUpdate viewUpdate);
    glm::mat4 GetProjection();
    glm::mat4 GetView();
  
  private:
		//vp matrices
    glm::mat4 projection;
    glm::mat4 view;

		//camera position and view vectors
		glm::vec3 Position;
		glm::vec3 Front;
		glm::vec3 Up;
		glm::vec3 Right;
		glm::vec3 WorldUp;

		//view rotation
		GLfloat Yaw;
		GLfloat Pitch;

		//view and movement variables
		GLfloat MovementSpeed;
		GLfloat MouseSensitivity;
		GLfloat Zoom;
};

#endif /* CAMERA_H */
