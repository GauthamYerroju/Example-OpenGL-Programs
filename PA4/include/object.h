#ifndef OBJECT_H
#define OBJECT_H

#include <vector>
#include "graphics_headers.h"

class Object
{
  public:
    Object();
    ~Object();

    void Render();
    glm::mat4 GetModel();

    virtual void Update(unsigned int dt, EventFlag e_flags) =0;
    virtual void Set_RotateSpeed(float r_speed) = 0;
    virtual void Set_Scale( float sclr ) = 0;
    virtual bool Model_Loader(const char *filePath) = 0;


  protected:
    glm::mat4 model;

    std::vector<Vertex> Vertices;
    std::vector<unsigned int> Indices;
    GLuint VB;
    GLuint IB;
   
};

#endif /* OBJECT_H */
