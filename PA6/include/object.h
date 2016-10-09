#ifndef OBJECT_H
#define OBJECT_H

#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include "graphics_headers.h"

class Object
{
  public:
    Object(const char *objPath);
    ~Object();

    void Render();
    glm::mat4 GetModel();

    void Update(unsigned int dt, EventFlag e_flags);
    void Set_RotateSpeed(float r_speed); 
    void Set_Scale( float sclr );
    bool Model_Loader(const char *filePath);
    bool Texture_Loader(const char *filePath, GLenum GL_TEXTUREi);

  private:
    glm::mat4 model;
    glm::mat4 rotation;
    glm::mat4 scale;

    float angle_rotate;
    float rotate_speed;
    float scaler;

    std::vector<Vertex> Vertices;
    std::vector<unsigned int> Indices;
    GLuint VB;
    GLuint IB;

    std::vector<Texture> textures;
    Magick::Image m_pImage;
    Magick::Blob m_Blob;
   
};

#endif /* OBJECT_H */
