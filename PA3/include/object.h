#ifndef OBJECT_H
#define OBJECT_H

#include <vector>
#include "graphics_headers.h"

class Object
{
  public:
    Object();
    ~Object();
    void Update(unsigned int dt, EventFlag e_flags);
    void Render();
    void Set_OrbitSpeed(float o_speed);
    void Set_RotateSpeed(float r_speed);
    void Set_OrbitCenter(glm::mat4 o_center);

    glm::mat4 GetModel();
    glm::mat4 GetPosition();

  private:
    glm::mat4 model;
    glm::mat4 position;
    std::vector<Vertex> Vertices;
    std::vector<unsigned int> Indices;
    GLuint VB;
    GLuint IB;

    float angle_rotate;
    float angle_orbit;
    float orbit_radius;
    float orbit_speed;
    float rotate_speed;

    glm::mat4 orbit_center;
};

#endif /* OBJECT_H */
