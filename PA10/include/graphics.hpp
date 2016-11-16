#ifndef GRAPHICS_H
#define GRAPHICS_H

#include <iostream>
using namespace std;

#include <SDL2/SDL.h>
#include "graphics_headers.hpp"
#include "camera.hpp"
#include "shader.hpp"
#include "object.hpp"
#include "physicsObject.hpp"
#include "physicsWorld.hpp"

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

    void Update(unsigned int dt, SDL_Event *m_event);
    void HandleInput(SDL_Event *m_event);
    void Render();

  private:
    std::string ErrorString(GLenum error);
    bool perFragmentLighting;

    Camera *m_camera;
    Shader *m_shader;

    GLint m_projectionMatrix;
    GLint m_viewMatrix;
    GLint m_modelMatrix;

    GLint m_LightPosition;
    GLint m_AmbientProduct;
    GLint m_DiffuseProduct;
    GLint m_SpecularProduct;
    GLint m_Shininess;
    GLint m_SpotLightDirection;
    GLint m_SpotCutOff;

    int spotLightAngle;

    float amb_Scalar;
    float diff_Scalar;
    float spec_Scalar;

    struct BumperContactResultCallback : public btCollisionWorld::ContactResultCallback
    {
        BumperContactResultCallback(bool* ht) : hit(ht) {}

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

    PhysicsObject                         *board;
    PhysicsObject                         *ball;
    PhysicsObject                         *paddle;
    PhysicsObject                         *lFlipper;
    PhysicsObject                         *rFlipper;
    PhysicsObject                         *bumper1;
    PhysicsObject                         *tBumper1;
    PhysicsObject                         *oBumper1;
    PhysicsObject                         *bumper2;
    PhysicsObject                         *tBumper2;
    PhysicsObject                         *oBumper2;
    PhysicsObject                         *bumper3;
    PhysicsObject                         *tBumper3;
    PhysicsObject                         *oBumper3;
    PhysicsWorld                          world;

    bool lFlipperMoveUp;
    bool lFlipperMoveDown;
    bool rFlipperMove;
    float lFlipperStep;
    float rFlipperStep;


    BumperContactResultCallback *callback1;
    BumperContactResultCallback *callback2;
    BumperContactResultCallback *callback3;

    bool bumperHit1;
    bool bumperHit2;
    bool bumperHit3;

    float launcherPower;


    std::string perVert_vShaderFile;
    std::string perVert_fShaderFile;
    std::string perFrag_vShaderFile;
    std::string perFrag_fShaderFile;

};

#endif /* GRAPHICS_H */
