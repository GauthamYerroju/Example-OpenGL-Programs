#ifndef MESH_H
#define MESH_H

#include <vector>
#include <string>
#include <fstream>
#include <iterator>
#include "graphics_headers.hpp"

class Mesh
{
  public:
    Mesh();
    Mesh(Mesh *orig);
    ~Mesh();
    void Initialize();
    void Render();
    
    std::vector<Vertex> Vertices;
    std::vector<unsigned int> Indices;
    std::vector<GLuint> textures;

    GLuint VB;
    GLuint IB;
};

#endif  /* MESH_H */

