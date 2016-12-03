#include "camera.hpp"
#include <iostream>
Camera::Camera()
{

}

Camera::~Camera()
{

}

bool Camera::Initialize(int w, int h)
{
  width = w;
  height = h;
  position = glm::vec3(0.0, 25.0/1.5, 16/1.5);
  //--Init the view and projection matrices
  view = glm::lookAt( position, //Eye Position
                      glm::vec3(0.0, 0.0, 0.0), //Focus point
                      glm::vec3(0.0, 0.0, -1.0)); //Positive Y is up

  projection = glm::perspective( 45.0f, //the FoV typically 90 degrees is good which is what this is set to
                                 float(w)/float(h), //Aspect Ratio, so Circles stay Circular
                                 0.01f, //Distance to the near plane, normally a small value like this
                                 100.0f); //Distance to the far plane,
  return true;
}

glm::mat4 Camera::GetProjection()
{
  return projection;
}

glm::mat4 Camera::GetView(bool zoom)
{
  if (zoom)
  {
    position = glm::vec3(0.0, 25.0/1.5, 16/1.5);
  }

  else
  {
    position = glm::vec3(0.0, 15.0/1.5, 16/1.5);
  }

  view = glm::lookAt( position, //Eye Position
                      glm::vec3(0.0, 0.0, 0.0), //Focus point
                      glm::vec3(0.0, 0.0, -1.0)); //Positive Y is up

  return view;
}
