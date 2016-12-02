#ifndef OBJECT_2_H
#define OBJECT_2_H

#include <vector>
#include <fstream>
#include <sstream>

#include "graphics_headers.hpp"
#include "mesh.hpp"

class Object2
{
  public:
    Object2();
    Object2(const char *objPath );
    ~Object2();

    // bool Model_Loader(const char *filePath);
    bool loadLevel();
    Mesh Model_Loader(const char *filePath, int lane, float scale, float cubeLength, int lastLength);
    bool Texture_Loader(const char *filePath, Mesh *mesh);

    void Update();
    void Render();

    void Set_Origin( glm::mat4 orig ); 
    void Set_TranslationVec( glm::vec3 tVec );
    void Set_RotationAngle( float rotAngle, glm::vec3 rotAxis );
    void Set_Scaler( float sclr );

    glm::mat4 GetModel();
    glm::mat4 GetPosition();

  protected:

    glm::mat4 model;
    glm::mat4 translation;
    glm::mat4 rotation;
    glm::mat4 scale;

    glm::mat4 origin;
    glm::vec3 translationVec;
    glm::vec3 rotationAxis;
    float rotationAngle;
    float scaler;

    std::vector<Mesh> meshes;

};

#endif /* OBJECT_2_H */
