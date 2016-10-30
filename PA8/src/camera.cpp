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
  Position = glm::vec3(0.0, 0.0, 20.0); 	//Eye Position
  Front = glm::vec3(0.0, 0.0, 1.0); 		//Focus point
  WorldUp = glm::vec3(0.0, 1.0, 0.0); 		//Positive Y is up
  Yaw = -90.0f;
  Pitch = 0.0f;
  Zoom = 45.0f;
  MouseSensitivity = 0.0f;
  MovementSpeed = 0.0f;
  width = w;
  height = h;
  P_origin = Position;

  projection = glm::perspective( 45.0f, //the FoV typically 90 degrees is good which is what this is set to
                                 float(w)/float(h), //Aspect Ratio, so Circles stay Circular
                                 0.01f, //Distance to the near plane, normally a small value like this
                                 50000000.0f); //Distance to the far plane,

  UpdateVectors();
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

	//process mouse movement
	if (viewUpdate.type == MOUSE){
		viewUpdate.mouseX *= MouseSensitivity;
		viewUpdate.mouseY *= MouseSensitivity;
		Yaw += viewUpdate.mouseX;
		Pitch -= viewUpdate.mouseY;

		if (Pitch > 89.0f){
			Pitch = 89.0f;
		}

		if (Pitch < -89.0f){
			Pitch = -89.0f;
		}

		UpdateVectors();
	}

	//process scroll wheel
	if (viewUpdate.type == ZOOM){
		if (Zoom >= 44.5f && Zoom <= 45.0f){
			Zoom -= viewUpdate.scrollY;
		}

		if (Zoom <= 44.5f){
			Zoom = 44.5f;
		}

		if ( Zoom >= 45.0f){
			Zoom = 45.0f;
		}
	}

}

void Camera::ZoomOnPlanet(glm::vec3 Pos, int PosSclr){

	GLfloat scale = MovementSpeed * PosSclr;
	Position = Pos;
	Position -= Front * scale;
}

void Camera::ResetPosition()
{
	Position = P_origin;
}

glm::mat4 Camera::GetProjection()
{
  return glm::perspective( Zoom, //the FoV typically 90 degrees is good which is what this is set to
                                 width/height, //Aspect Ratio, so Circles stay Circular
                                 0.01f, //Distance to the near plane, normally a small value like this
                                 50000.0f); //Distance to the far plane
}

glm::mat4 Camera::GetView()
{
  return glm::lookAt(Position, Position + Front, Up);
}


