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
  // 1. Model file (string, path)
  // 2. Texture file (string, path)
  // 3. Orbit radius (float, in au)
  // 4. Orbit speed (float, radians)
  // 5. Spin speed (float, radians)
  // 6. Axial tilt (vec3 for degrees in 3 axes)
  // 7. Orbital tilt (vec3 for degrees in 3 axes)
  
  public:
    Object(const char *objPath);
    ~Object();

    // Basics
    void Render();
    void Update(unsigned int dt, EventFlag e_flags);

    // Utils
    bool Model_Loader(const char *filePath);
    bool Texture_Loader(const char *filePath, Mesh *mesh);

    // Attrib Accessors
    void Set_ScaleFactor(float value);
    void Set_SpeedFactor(float value);
    
    void Set_Radius(float value);
    void Set_SpinSpeed(float value);
    void Set_OrbitSpeed(float value);
    void Set_OrbitRadius(float value);
    void Set_AxialTilt(glm::vec3 value);
    void Set_OrbitalTilt(glm::vec3 value);

    glm::mat4 GetModel();

  private:
    // Model matrices
    std::vector<Mesh> meshes; // Model is a list of meshes, which contain their own textures

    glm::mat4 model;
    glm::mat4 rotation;
    glm::mat4 scale;
    glm::mat4 translation;

    // Global modifiers
    float scaleFactor; // Convert to scale matrix and multiply with model (before orbit translation)
    float speedFactor; // Multiply with orbitSpeed and spinSpeed every frame

    // Cross-frame metrics
    float radius; // Should be a ratio (float)
    float spinSpeed; // Should be in radians
    float orbitSpeed; // Should be in radians
    float orbitRadius; // Should be in length units (au?)
    glm::vec3 axialTilt; // Axial tilt along x, y and z axes in radians
    glm::vec3 orbitalTilt; // Axial tilt along x, y and z axes in radians

    // Per-frame metrics
    float currentSpinAngle;
    float currentOrbitAngle;

    // For internal use, so private
    float GetSpinSpeed();
};

#endif /* OBJECT_H */
