#include "object.h"

Object::Object()
{  
  /*
    # Blender File for a Cube
    o Cube
    v 1.000000 -1.000000 -1.000000
    v 1.000000 -1.000000 1.000000
    v -1.000000 -1.000000 1.000000
    v -1.000000 -1.000000 -1.000000
    v 1.000000 1.000000 -0.999999
    v 0.999999 1.000000 1.000001
    v -1.000000 1.000000 1.000000
    v -1.000000 1.000000 -1.000000
    s off
    f 2 3 4
    f 8 7 6
    f 1 5 6
    f 2 6 7
    f 7 8 4
    f 1 4 8
    f 1 2 4
    f 5 8 6
    f 2 1 6
    f 3 2 7
    f 3 7 4
    f 5 1 8
  */

  Vertices = {
    {{1.0f, -1.0f, -1.0f}, {0.0f, 0.0f, 0.0f}},
    {{1.0f, -1.0f, 1.0f}, {1.0f, 0.0f, 0.0f}},
    {{-1.0f, -1.0f, 1.0f}, {0.0f, 1.0f, 0.0f}},
    {{-1.0f, -1.0f, -1.0f}, {0.0f, 0.0f, 1.0f}},
    {{1.0f, 1.0f, -1.0f}, {1.0f, 1.0f, 0.0f}},
    {{1.0f, 1.0f, 1.0f}, {1.0f, 0.0f, 1.0f}},
    {{-1.0f, 1.0f, 1.0f}, {0.0f, 1.0f, 1.0f}},
    {{-1.0f, 1.0f, -1.0f}, {1.0f, 1.0f, 1.0f}}
  };

  Indices = {
    2, 3, 4,
    8, 7, 6,
    1, 5, 6,
    2, 6, 7,
    7, 8, 4,
    1, 4, 8,
    1, 2, 4,
    5, 8, 6,
    2, 1, 6,
    3, 2, 7,
    3, 7, 4,
    5, 1, 8
  };

  // The index works at a 0th index
  for(unsigned int i = 0; i < Indices.size(); i++)
  {
    Indices[i] = Indices[i] - 1;
  }

  angle_rotate = 0.0f;
  angle_orbit = 0.0f;
  orbit_radius = 5.0f;
  orbit_speed = 0.25f;
  rotate_speed = 1.0f;
  scaler = 1.0f;
  orbit_center = glm::mat4(1.0f);

  glGenBuffers(1, &VB);
  glBindBuffer(GL_ARRAY_BUFFER, VB);
  glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * Vertices.size(), &Vertices[0], GL_STATIC_DRAW);

  glGenBuffers(1, &IB);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IB);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * Indices.size(), &Indices[0], GL_STATIC_DRAW);
}

Object::~Object()
{
  Vertices.clear();
  Indices.clear();
}

void Object::Update(unsigned int dt, EventFlag e_flags)
{
  // If system not paused
  if( !e_flags.pause_all ){
    
    // If rotation not paused
    if( !e_flags.pause_rotate ){

      if( !e_flags.clockwise_rotate )
        // Set counter clockwise angle of rotation
        angle_rotate += (dt * M_PI/1000) * rotate_speed;

      else if( e_flags.clockwise_rotate )
        // Set clockwise angle of rotation
        angle_rotate -= (dt * M_PI/1000) * rotate_speed;
    }

    // If orbit not paused
    if( !e_flags.pause_orbit ){

      if( !e_flags.clockwise_orbit )
        // Set counter clockwise angle of orbit
        angle_orbit += (dt * M_PI/1000) * orbit_speed;

      else if( e_flags.clockwise_orbit )
        // Set clockwise angle of orbit
        angle_orbit -= (dt * M_PI/1000) * orbit_speed;
    }
  }

  translation = glm::translate(orbit_center, glm::vec3(sin(angle_orbit)*orbit_radius, 0.0f, cos(angle_orbit)*orbit_radius));
  rotation = glm::rotate(glm::mat4(1.0f), (angle_rotate), glm::vec3(0.0, 1.0, 0.0));
  scale = glm::scale(glm::mat4(1.0f), glm::vec3(1.0, 1.0, 1.0) * scaler);

  model = translation * rotation * scale;
  
}

void Object::Set_OrbitSpeed(float o_speed)
{
  orbit_speed = o_speed;
}

void Object::Set_RotateSpeed(float r_speed)
{
  rotate_speed = r_speed;
}

void Object::Set_OrbitRadius(float o_rad)
{
  orbit_radius = o_rad;
}

void Object::Set_Scale( float sclr )
{
  scaler = sclr;
}

void Object::Set_OrbitCenter(glm::mat4 o_center)
{
  orbit_center = o_center;
}

glm::mat4 Object::GetModel()
{
  return model;
}

glm::mat4 Object::GetPosition(){
  return translation;
}

void Object::Render()
{
  glEnableVertexAttribArray(0);
  glEnableVertexAttribArray(1);

  glBindBuffer(GL_ARRAY_BUFFER, VB);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex,color));

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IB);

  glDrawElements(GL_TRIANGLES, Indices.size(), GL_UNSIGNED_INT, 0);

  glDisableVertexAttribArray(0);
  glDisableVertexAttribArray(1);
}

