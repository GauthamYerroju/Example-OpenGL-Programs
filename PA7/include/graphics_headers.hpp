#ifndef GRAPHICS_HEADERS_H
#define GRAPHICS_HEADERS_H

#include <iostream>

#define GL_DO_NOT_WARN_IF_MULTI_GL_VERSION_HEADERS_INCLUDED

#if defined(__APPLE__) || defined(MACOSX)
  #include <OpenGL/gl3.h>
  #include <OpenGL/GLU.h>
#else //linux as default
  #include <GL/glew.h>
  //#include <GL/glu.h>
#endif

// GLM for matricies
#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include "glm/gtx/string_cast.hpp"

// ASSIMP for model loader
#include <assimp/Importer.hpp> //includes the importer, which is used to read our obj file 
#include <assimp/scene.h> //includes the aiScene object
#include <assimp/postprocess.h> //includes the postprocessing variables for the importer
#include <assimp/color4.h> //includes the aiColor4 object, which is used to handle the colors from the mesh objects 

// Magick++ for image loading
#include <Magick++.h>

//CSPICE
#include <SpiceUsr.h>

#define INVALID_UNIFORM_LOCATION 0x7fffffff

enum UpdateType {KEY, MOUSE};
enum Direction {FORWARD, BACKWARD, LEFT, RIGHT};
struct Vertex
{
  glm::vec3 vertex;
  glm::vec2 uv_Coords;

  Vertex(glm::vec3 v, glm::vec2 uv): vertex(v), uv_Coords(uv) {}
};

struct EventFlag
{
	bool clockwise_rotate;
  bool pause_all;

  EventFlag()
  {
    clockwise_rotate = false;
    pause_all = false;
	}
};

struct ViewUpdate
{
	bool processed;
	unsigned int dt;
	UpdateType type;
	Direction direction;

};
#endif /* GRAPHICS_HEADERS_H */
