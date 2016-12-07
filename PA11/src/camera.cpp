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
  focusPoint = glm::vec3(0.0, 0.0, 0.0);
  //--Init the view and projection matrices
  view = glm::lookAt( position, //Eye Position
                      focusPoint, //Focus point
                      glm::vec3(0.0, 0.0, -1.0)); //Positive Y is up

  projection = glm::perspective( 45.0f, //the FoV typically 90 degrees is good which is what this is set to
                                 float(w)/float(h), //Aspect Ratio, so Circles stay Circular
                                 0.01f, //Distance to the near plane, normally a small value like this
                                 10000.0f); //Distance to the far plane,
  return true;
}

glm::mat4 Camera::GetProjection()
{
  return projection;
}

void Camera::Update(bool zoom)
{
  view = glm::lookAt(
    (zoom ? position + (focusPoint - position)*0.5f : position), // Eye Position
    focusPoint, //Focus point
    glm::vec3(0.0, 0.0, -1.0)); //Positive Y is up
}

glm::mat4 Camera::GetView()
{
  return view;
}

glm::vec3 Camera::GetPosition()
{
  return position;
}

void Camera::SetPosition(glm::vec3 value)
{
  position = value;
}

glm::vec3 Camera::GetFocusPoint()
{
  return focusPoint;
}

void Camera::SetFocusPoint(glm::vec3 value)
{
  focusPoint = value;
}
