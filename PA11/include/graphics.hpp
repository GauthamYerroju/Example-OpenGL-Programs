#ifndef GRAPHICS_H
#define GRAPHICS_H

#include <iostream>
using namespace std;

#include <SDL2/SDL.h>
#include "graphics_headers.hpp"
#include "camera.hpp"
#include "shader.hpp"
#include "object.hpp"
#include "gameTrack.hpp"
#include "physicsObject.hpp"
#include "physicsWorld.hpp"
#include "input.hpp"

// For the config file
#include "json.hpp"
using json = nlohmann::json;

class Graphics
{
  public:
    Graphics();
    ~Graphics();
    bool Initialize(int width, int height, char *configFile);
    bool LoadConfig( char *configFile );

    bool SetShader();
    void SetPerFragLighting();
    void SetPerVertLighting();

    void SetAmbientScalar(float a_slr);
    void SetDiffuseScalar(float d_slr);
    void SetSpecularScalar(float s_slr);
    void SetSpotLightAngle(int angle);
    float getAmbientScalar();
    float getDiffuseScalar();
    float getSpecularScalar();
    int getSpotLightAngle();

    void Update(unsigned int dt, Input *m_input);
    void HandleInput(Input *m_input);
    void Render();

  private:
    std::string ErrorString(GLenum error);
    bool perFragmentLighting;
    bool zoom;

    Camera *m_camera;
    Shader *m_shader;
    
    GLint m_projectionMatrix;
    GLint m_viewMatrix;
    GLint m_modelMatrix;

    Lights lights;

    int spotLightAngle;

    float amb_Scalar;
    float diff_Scalar;
    float spec_Scalar;

    void printToConsole();
    void resetShip();

    GameTrack                             *track;
    PhysicsObject                         *ship;
    Object                                *skyBox;
    PhysicsWorld                          world;

    struct CollisionCallback : public btCollisionWorld::ContactResultCallback
    {
        CollisionCallback(bool* ht) : hit(ht) {}
        btScalar addSingleResult(   btManifoldPoint& cp,
            const btCollisionObjectWrapper* colObj0Wrap,
            int partId0,
            int index0,
            const btCollisionObjectWrapper* colObj1Wrap,
            int partId1,
            int index1)
        {
            *hit= true;
        }

        bool* hit;
    };

    // BumperContactResultCallback *callback1;
    // BumperContactResultCallback *callback2;
    // BumperContactResultCallback *callback3;

    int score;
    int lives;
    bool jumping;

    std::string perVert_vShaderFile;
    std::string perVert_fShaderFile;
    std::string perFrag_vShaderFile;
    std::string perFrag_fShaderFile;

};

#endif /* GRAPHICS_H */
