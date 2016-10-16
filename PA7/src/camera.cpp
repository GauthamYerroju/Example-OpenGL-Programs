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
  //--Init the view and projection matrices
  cameraLocation = glm::vec3(0.0, 20.0, -700.0); 	//Eye Position
  cameraFocus = glm::vec3(0.0, 0.0, 0.0); 				//Focus point
  cameraUp = glm::vec3(0.0, 1.0, 0.0); 						//Positive Y is up

	view = glm::lookAt(cameraLocation, cameraFocus, cameraUp);
  projection = glm::perspective( 45.0f, //the FoV typically 90 degrees is good which is what this is set to
                                 float(w)/float(h), //Aspect Ratio, so Circles stay Circular
                                 0.01f, //Distance to the near plane, normally a small value like this
                                 20000.0f); //Distance to the far plane, 
  return true;
}

void Camera::Update(ViewUpdate viewUpdate)
{
	if (viewUpdate.type == FOCUS){
		cameraFocus += viewUpdate.value;
		view = glm::lookAt(cameraLocation, cameraFocus, cameraUp);
	}

	if (viewUpdate.type == EYE){
		cameraLocation += viewUpdate.value;
		view = glm::lookAt(cameraLocation, cameraFocus, cameraUp);
	}
}

glm::mat4 Camera::GetProjection()
{
  return projection;
}

glm::mat4 Camera::GetView()
{
  return view;
}


