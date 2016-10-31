#ifndef OBJECT_H
#define OBJECT_H

#include <vector>
#include <fstream>
#include <sstream>

#include "graphics_headers.hpp"
#include "mesh.hpp"

class Object
{
  public:

    Object(const char *objPath );
    ~Object();

    void Render();
    glm::mat4 GetModel();
    glm::mat4 GetPosition();

    void Update(); 
    void Set_Origin( glm::mat4 orig ); 
    void Set_TranslationVec( glm::vec3 tVec );
    void Set_RotationAngle( float rotAngle );
    void Set_Scaler( float sclr );
    bool Model_Loader(const char *filePath);
    bool Texture_Loader(const char *filePath, Mesh *mesh);

  protected:

    glm::mat4 model;
    glm::mat4 translation;
    glm::mat4 rotation;
    glm::mat4 scale;

    glm::mat4 origin;
    glm::vec3 translationVec;
    float rotationAngle;
    float scaler;

    std::vector<Mesh> meshes;

};

#endif /* OBJECT_H */
