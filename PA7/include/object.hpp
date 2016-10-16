#ifndef OBJECT_H
#define OBJECT_H

#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include "graphics_headers.hpp"
#include "mesh.hpp"

class Object
{
  public:
    Object(const char *objPath, const char *planet_name, const char *planet_orbiting);
    ~Object();

    void Render();
    glm::mat4 GetModel();
    glm::mat4 GetPosition();

    void Update(unsigned int dt, EventFlag e_flags);
    void Set_OrbitCenter(glm::mat4 o_center);
    void Set_OrbitRadius(glm::vec3 rad);
    void Set_RotateSpeed(float r_speed);
    void Set_Scale( float sclr );
    void Set_RadScale( float r_sclr );
    bool Model_Loader(const char *filePath);
    bool Texture_Loader(const char *filePath, Mesh *mesh);

    float getScaler();
    void Set_orbitOffset(float offset);

  private:
    glm::mat4 model;
    glm::mat4 translation;
    glm::mat4 rotation;
    glm::mat4 scale;

    glm::mat4 orbit_center;
    glm::vec3 orbit_radius;

    const char *planet;
    const char *orbit_planet;

    double et;
    double lt;
    int orbit_step;

    float angle_rotate;
    float orbit_speed;
    float rotate_speed;
    float rad_scaler;
    float scaler;

    std::vector<Mesh> meshes;

    float scaleToLog(float value);
};

#endif /* OBJECT_H */
