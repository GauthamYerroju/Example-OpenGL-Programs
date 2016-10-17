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
<<<<<<< HEAD
  Position = glm::vec3(0.0, 20.0, 700.0); 	//Eye Position
  Front = glm::vec3(0.0, 0.0, -1.0); 				//Focus point
  WorldUp = glm::vec3(0.0, 1.0, 0.0); 			//Positive Y is up
	Yaw = -90.0f;
	Pitch = 0.0f;
	Zoom = 45.0f;
	MouseSensitivity = 0.25f;
	MovementSpeed = 3.0f;
=======
  //  if you will be having a moving camera the view matrix will need to more dynamic
  //  ...Like you should update it before you render more dynamic 
  //  for this project having them static will be fine
  view = glm::lookAt( glm::vec3(0.0, -400.0, 0.0), //Eye Position
                      glm::vec3(0.0, 0.0, 0.0), //Focus point
                      glm::vec3(0.0, 0.0, 1.0)); //Positive Y is up
>>>>>>> 029320f427889b6dbb567f9469574282bb1de634

  projection = glm::perspective( 45.0f, //the FoV typically 90 degrees is good which is what this is set to
                                 float(w)/float(h), //Aspect Ratio, so Circles stay Circular
                                 0.01f, //Distance to the near plane, normally a small value like this
<<<<<<< HEAD
                                 20000.0f); //Distance to the far plane, 
	UpdateVectors();
=======
                                 50000.0f); //Distance to the far plane, 
>>>>>>> 029320f427889b6dbb567f9469574282bb1de634
  return true;
}

void Camera::UpdateVectors()
{
	//Update the Front
	glm::vec3 tempFront;

	tempFront.x = cos(glm::radians(Yaw)) * cos(glm::radians(Pitch));
	tempFront.y = sin(glm::radians(Pitch));
	tempFront.z = sin(glm::radians(Yaw)) * cos(glm::radians(Pitch));
	Front = glm::normalize(tempFront);

	//Update the Right and Up vectors
	Right = glm::normalize(glm::cross(Front, WorldUp));
	Up = glm::normalize(glm::cross(Right, Front));	
}

void Camera::ProcessInput(ViewUpdate viewUpdate)
{
	//process keyboard input
	if (viewUpdate.type == KEY){
		GLfloat speed = MovementSpeed * viewUpdate.dt;
		if(viewUpdate.direction == FORWARD){
			Position += Front * speed;
		}
		if(viewUpdate.direction == BACKWARD){
			Position -= Front * speed;
		}
		if(viewUpdate.direction == LEFT){
			Position -= Right * speed;
		}
		if(viewUpdate.direction == RIGHT){
			Position += Right * speed;
		}
	}

	//process mouse input
	
}

glm::mat4 Camera::GetProjection()
{
  return projection;
}

glm::mat4 Camera::GetView()
{
  return glm::lookAt(Position, Position + Front, Up);
}


