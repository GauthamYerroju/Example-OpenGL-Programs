#include "graphics.hpp"
#define clear() printf("\033[H\033[J")

Graphics::Graphics()
{

}

Graphics::~Graphics()
{

}

bool Graphics::Initialize(int width, int height, char *configFile)
{
  // Used for the linux OS
  #if !defined(__APPLE__) && !defined(MACOSX)
    // cout << glewGetString(GLEW_VERSION) << endl;
    glewExperimental = GL_TRUE;

    auto status = glewInit();

    // This is here to grab the error that comes from glew init.
    // This error is an GL_INVALID_ENUM that has no effects on the performance
    glGetError();

    //Check for error
    if (status != GLEW_OK)
    {
      std::cerr << "GLEW Error: " << glewGetErrorString(status) << "\n";
      return false;
    }
  #endif

  lives = 3;

  // For OpenGL 3
  GLuint vao;
  glGenVertexArrays(1, &vao);
  glBindVertexArray(vao);

  // Init Camera
  m_camera = new Camera();
  if(!m_camera->Initialize(width, height))
  {
    printf("Camera Failed to Initialize\n");
    return false;
  }

  // Create Physics World
  world.Initialize();

  // cnfg
  if(!LoadConfig( configFile ))
  {
    printf("Graphics: Config File failed to load\n");
    return false;
  }

  // Set default lighting to per fragment
  bool perFragmentLighting = true;

  //shaders
  if(!SetShader())
  {
    printf("Shader Setup failed\n");
    return false;
  }

  //enable depth testing
  glEnable(GL_DEPTH_TEST);
  glDepthFunc(GL_LESS);

  amb_Scalar = 1.0;
  diff_Scalar = 1.0;
  spec_Scalar = 1.0;

  spotLightAngle = 10;

  lFlipperMoveUp = false;
  lFlipperMoveDown = false;
  lFlipperStep = 0.0;


  bumperHit1 = false;
  bumperHit2 = false;
  bumperHit3 = false;
  zoom = false;

  launcherPower = 0.0f;
  score = 0;

  return true;
}


bool Graphics::LoadConfig( char *configFile )
{
  std::ifstream config_fStream(configFile);

  if ( config_fStream.fail() )
  {
    std::cerr << "Error with config file: " << std::strerror(errno) << std::endl;
    return false;
  }

  json config;
  config << config_fStream;

  // Load Configuration
  for (json::iterator it = config.begin(); it != config.end(); ++it)
  {
    auto objectConfig = *it;

    // Load the model
    std::string label = objectConfig["name"];
    std::string modelFile;

    if(label == "perVertexLighting")
    {
      perVert_vShaderFile = objectConfig["vertexShader"];
      perVert_fShaderFile = objectConfig["fragmentShader"];
    }
    else if(label == "perFragmentLighting")
    {
      perFrag_vShaderFile = objectConfig["vertexShader"];
      perFrag_fShaderFile = objectConfig["fragmentShader"];
    }
    else if(label == "Segment")
    {
      modelFile = objectConfig["modelFile"];

      SevenSegment *dig;
      float x_offset = 0;


      for(int dig_count = 0; dig_count < 3; dig_count++)
      {
        dig = new SevenSegment( new Object(modelFile.c_str()),
                                new Object(modelFile.c_str()),
                                new Object(modelFile.c_str()),
                                new Object(modelFile.c_str()),
                                new Object(modelFile.c_str()),
                                new Object(modelFile.c_str()),
                                new Object(modelFile.c_str()) 
                              );

        // y is up and -z is forward
        dig->A->Set_TranslationVec(glm::vec3(-1.8 + x_offset, 2.72, -8.0));
        dig->A->Set_RotationAngle(glm::radians(98.421), glm::vec3(0.357, 0.863, 0.357));
        dig->A->Update();

        dig->B->Set_TranslationVec(glm::vec3(-1.3 + x_offset, 2.37, -7.65));
        dig->B->Set_RotationAngle(glm::radians(45.0), glm::vec3(1.0, 0.0, 0.0));
        dig->B->Update();

        dig->C->Set_TranslationVec(glm::vec3(-1.3 + x_offset, 1.72, -7.0));
        dig->C->Set_RotationAngle(glm::radians(45.0), glm::vec3(1.0, 0.0, 0.0));
        dig->C->Update();

        dig->D->Set_TranslationVec(glm::vec3(-1.8 + x_offset, 1.38, -6.66));
        dig->D->Set_RotationAngle(glm::radians(98.421), glm::vec3(0.357, 0.863, 0.357));
        dig->D->Update();

        dig->E->Set_TranslationVec(glm::vec3(-2.3 + x_offset, 1.72, -7.0));
        dig->E->Set_RotationAngle(glm::radians(45.0), glm::vec3(1.0, 0.0, 0.0));
        dig->E->Update();

        dig->F->Set_TranslationVec(glm::vec3(-2.3 + x_offset, 2.37, -7.65));
        dig->F->Set_RotationAngle(glm::radians(45.0), glm::vec3(1.0, 0.0, 0.0));
        dig->F->Update();

        dig->G->Set_TranslationVec(glm::vec3(-1.8+ x_offset, 2.05, -7.33));
        dig->G->Set_RotationAngle(glm::radians(98.421), glm::vec3(0.357, 0.863, 0.357));
        dig->G->Update();

        x_offset += 1.8;


        digit.push_back(dig);
      }
    }
    else if(label == "Frame")
    {
      modelFile = objectConfig["modelFile"];
      frame = new Object(modelFile.c_str());

      frame->Update();
    }
    else if(label == "Board")
    {
      modelFile = objectConfig["modelFile"];
      board = new PhysicsObject( modelFile.c_str() );

      if( !board->Initialize(PhysicsObject::TRIANGLE_MESH,  //CollisionShape
                              0,  //Mass
                              btTransform( btQuaternion(0, 0, 0, 1), btVector3(0.0, 0.0, -2.58196) ),  //WorldTranformation
                              0.8,  //Restitution
                              1.5   //Friction
                              ))
        printf("PhysicsObject failed to initialize\n");

      world.AddRigidBody(board->GetRigidBody());
    }
    else if(label == "Ball")
    {
      modelFile = objectConfig["modelFile"];
      ball = new PhysicsObject( modelFile.c_str() );

      if( !ball->Initialize(PhysicsObject::SPHERE_SHAPE,  //CollisionShape
                              1,  //Mass
                              btTransform( btQuaternion(0.0, 0.0, 0.0, 1.0), btVector3(3.03204, 0.2217404, 6.36945) ),  //WorldTranformation
                              0.8,  //Restitution
                              1.5   //Friction
                              ))
        printf("PhysicsObject failed to initialize\n");

      ball->GetRigidBody()->setActivationState(DISABLE_DEACTIVATION);

      ball->GetRigidBody()->setLinearFactor(btVector3(1, 0, 1));

      world.AddRigidBody(ball->GetRigidBody());
    }
    else if(label == "Paddle")
    {
      modelFile = objectConfig["modelFile"];
      paddle = new PhysicsObject( modelFile.c_str() );

      if( !paddle->Initialize(PhysicsObject::CYLINDER_SHAPE,  //CollisionShape
                              0,  //Mass
                              btTransform( btQuaternion(0.0, 0.0, 0.0, 1.0), btVector3(3.03191, 0.2289447, 6.91188) ),  //WorldTranformation
                              0.8,  //Restitution
                              1.5   //Friction
                              ))
        printf("PhysicsObject failed to initialize\n");


      // Constrain linear motion along y axis
      paddle->GetRigidBody()->setLinearFactor(btVector3(1, 0, 1));

      paddle->GetRigidBody()->setCollisionFlags(paddle->GetRigidBody()->getCollisionFlags() | btCollisionObject::CF_KINEMATIC_OBJECT );

      world.AddRigidBody(paddle->GetRigidBody());
    }
    else if(label == "LeftFlipper")
    {
      modelFile = objectConfig["modelFile"];
      lFlipper = new PhysicsObject( modelFile.c_str() );

      if( !lFlipper->Initialize(PhysicsObject::TRIANGLE_MESH,  //CollisionShape
                              0,  //Mass
                              btTransform( btQuaternion(0.0, 0.0, 0.0, 1.0), btVector3(-1.17565, 1.41711, 5.99798) ),  //WorldTranformation
                              0.8,  //Restitution
                              1.5   //Friction
                              ))
        printf("PhysicsObject failed to initialize\n");

      // Constrain linear motion along z axis and disable angular motion
      //lFlipper->GetRigidBody()->setLinearFactor(btVector3(0, 0, 1));
      //lFlipper->GetRigidBody()->setAngularFactor(btVector3(0, 0, 0));

      lFlipper->GetRigidBody()->setCollisionFlags(lFlipper->GetRigidBody()->getCollisionFlags() | btCollisionObject::CF_KINEMATIC_OBJECT );
      
      world.AddRigidBody(lFlipper->GetRigidBody());
    }
    else if(label == "RightFlipper")
    {
      modelFile = objectConfig["modelFile"];
      rFlipper = new PhysicsObject( modelFile.c_str() );

      if( !rFlipper->Initialize(PhysicsObject::TRIANGLE_MESH,  //CollisionShape
                              0,  //Mass
                              btTransform( btQuaternion(0.0, 0.0, 0.0, 1.0), btVector3(1.15313, 1.41711, 5.99798) ),  //WorldTranformation
                              0.8,  //Restitution
                              1.5   //Friction
                              ))
        printf("PhysicsObject failed to initialize\n");

      rFlipper->GetRigidBody()->setCollisionFlags(rFlipper->GetRigidBody()->getCollisionFlags() | btCollisionObject::CF_KINEMATIC_OBJECT );
      
      world.AddRigidBody(rFlipper->GetRigidBody());
    }
    else if(label == "Bumper1")
    {
      modelFile = objectConfig["modelFile"];
      bumper1 = new PhysicsObject( modelFile.c_str() );

      if( !bumper1->Initialize(PhysicsObject::TRIANGLE_MESH,  //CollisionShape
                              0,  //Mass
                              btTransform( btQuaternion(0.0, 0.0, 0.0, 1.0), btVector3(0.0, 0.5004097, -3.52873) ),  //WorldTranformation
                              0.8,  //Restitution
                              1.5   //Friction
                              ))
        printf("PhysicsObject failed to initialize\n");

      world.AddRigidBody(bumper1->GetRigidBody()); 
    }
    else if(label == "TopBumper1")
    {
      modelFile = objectConfig["modelFile"];
      tBumper1 = new PhysicsObject( modelFile.c_str() );

      if( !tBumper1->Initialize(PhysicsObject::TRIANGLE_MESH,  //CollisionShape
                              0,  //Mass
                              btTransform( btQuaternion(0.0, 0.0, 0.0, 1.0), btVector3(0.0, 0.3634913, -3.52873) ),  //WorldTranformation
                              0.8,  //Restitution
                              1.5   //Friction
                              ))
        printf("PhysicsObject failed to initialize\n");

      world.AddRigidBody(tBumper1->GetRigidBody());
    }
    else if(label == "OuterBumper1")
    {
      modelFile = objectConfig["modelFile"];
      oBumper1 = new PhysicsObject( modelFile.c_str() );

      if( !oBumper1->Initialize(PhysicsObject::TRIANGLE_MESH,  //CollisionShape
                              0,  //Mass
                              btTransform( btQuaternion(0.0, 0.0, 0.0, 1.0), btVector3(0.0, 0.2642085, -3.52873) ),  //WorldTranformation
                              0.8,  //Restitution
                              1.5   //Friction
                              ))
        printf("PhysicsObject failed to initialize\n");

      world.AddRigidBody(oBumper1->GetRigidBody());
    }
    else if(label == "Bumper2")
    {
      modelFile = objectConfig["modelFile"];
      bumper2 = new PhysicsObject( modelFile.c_str() );

      if( !bumper2->Initialize(PhysicsObject::TRIANGLE_MESH,  //CollisionShape
                              0,  //Mass
                              btTransform( btQuaternion(0.0, 0.0, 0.0, 1.0), btVector3(-1.4, 0.5004097, -1.82873) ),  //WorldTranformation
                              0.8,  //Restitution
                              1.5   //Friction
                              ))
        printf("PhysicsObject failed to initialize\n");

      world.AddRigidBody(bumper2->GetRigidBody());
    }
    else if(label == "TopBumper2")
    {
      modelFile = objectConfig["modelFile"];
      tBumper2 = new PhysicsObject( modelFile.c_str() );

      if( !tBumper2->Initialize(PhysicsObject::TRIANGLE_MESH,  //CollisionShape
                              0,  //Mass
                              btTransform( btQuaternion(0.0, 0.0, 0.0, 1.0), btVector3(-1.4, 0.3634913, -1.82873) ),  //WorldTranformation
                              0.8,  //Restitution
                              1.5   //Friction
                              ))
        printf("PhysicsObject failed to initialize\n");

      world.AddRigidBody(tBumper2->GetRigidBody());
    }
    else if(label == "OuterBumper2")
    {
      modelFile = objectConfig["modelFile"];
      oBumper2 = new PhysicsObject( modelFile.c_str() );

      if( !oBumper2->Initialize(PhysicsObject::TRIANGLE_MESH,  //CollisionShape
                              0,  //Mass
                              btTransform( btQuaternion(0.0, 0.0, 0.0, 1.0), btVector3(-1.4, 0.2642085, -1.82873) ),  //WorldTranformation
                              0.8,  //Restitution
                              1.5   //Friction
                              ))
        printf("PhysicsObject failed to initialize\n");

      world.AddRigidBody(oBumper2->GetRigidBody());
    }
    else if(label == "Bumper3")
    {
      modelFile = objectConfig["modelFile"];
      bumper3 = new PhysicsObject( modelFile.c_str() );

      if( !bumper3->Initialize(PhysicsObject::TRIANGLE_MESH,  //CollisionShape
                              0,  //Mass
                              btTransform( btQuaternion(0.0, 0.0, 0.0, 1.0), btVector3(1.4, 0.5004097, -1.82873) ),  //WorldTranformation
                              0.8,  //Restitution
                              1.5   //Friction
                              ))
        printf("PhysicsObject failed to initialize\n");

      world.AddRigidBody(bumper3->GetRigidBody());
    }
    else if(label == "TopBumper3")
    {
      modelFile = objectConfig["modelFile"];
      tBumper3 = new PhysicsObject( modelFile.c_str() );

      if( !tBumper3->Initialize(PhysicsObject::TRIANGLE_MESH,  //CollisionShape
                              0,  //Mass
                              btTransform( btQuaternion(0.0, 0.0, 0.0, 1.0), btVector3(1.4, 0.3634913, -1.82873) ),  //WorldTranformation
                              0.8,  //Restitution
                              1.5   //Friction
                              ))
        printf("PhysicsObject failed to initialize\n");

      world.AddRigidBody(tBumper3->GetRigidBody());
    }
    else if(label == "OuterBumper3")
    {
      modelFile = objectConfig["modelFile"];
      oBumper3 = new PhysicsObject( modelFile.c_str() );

      if( !oBumper3->Initialize(PhysicsObject::TRIANGLE_MESH,  //CollisionShape
                              0,  //Mass
                              btTransform( btQuaternion(0.0, 0.0, 0.0, 1.0), btVector3(1.4, 0.2642085, -1.82873) ),  //WorldTranformation
                              0.8,  //Restitution
                              1.5   //Friction
                              ))
        printf("PhysicsObject failed to initialize\n");

      world.AddRigidBody(oBumper3->GetRigidBody());
    }
    else
    {
      printf("CONFIG: 'name: %s' no action taken\n", label.c_str() );
    }

  }

  return true;
}


bool Graphics::SetShader()
{
  // Set up the shaders
  m_shader = new Shader();
  if(!m_shader->Initialize())
  {
    printf("Shader Failed to Initialize\n");
    return false;
  }

  // Add the shaders
  if( perFragmentLighting )
  {
    // Add the vertex shader
    if(!m_shader->AddShader(GL_VERTEX_SHADER, perFrag_vShaderFile.c_str()))
    {
      printf("perFragmentLighting: Vertex Shader failed to Initialize\n");
      return false;
    }

    // Add the fragment shader
    if(!m_shader->AddShader(GL_FRAGMENT_SHADER, perFrag_fShaderFile.c_str()))
    {
      printf("perFragmentLighting: Fragment Shader failed to Initialize\n");
      return false;
    }
  }
  else
  {
    // Add the vertex shader
    if(!m_shader->AddShader(GL_VERTEX_SHADER, perVert_vShaderFile.c_str()))
    {
      printf("perVertexLighting: Vertex Shader failed to Initialize\n");
      return false;
    }

    // Add the fragment shader
    if(!m_shader->AddShader(GL_FRAGMENT_SHADER, perVert_fShaderFile.c_str()))
    {
      printf("perVertexLighting: Fragment Shader failed to Initialize\n");
      return false;
    }

  }

  // Connect the program
  if(!m_shader->Finalize())
  {
    printf("Program failed to Finalize\n");
    return false;
  }

  // Locate the projection matrix in the shader
  m_projectionMatrix = m_shader->GetUniformLocation("projectionMatrix");
  if (m_projectionMatrix == INVALID_UNIFORM_LOCATION)
  {
    printf("m_projectionMatrix not found\n");
    return false;
  }

  // Locate the view matrix in the shader
  m_viewMatrix = m_shader->GetUniformLocation("viewMatrix");
  if (m_viewMatrix == INVALID_UNIFORM_LOCATION)
  {
    printf("m_viewMatrix not found\n");
    return false;
  }

  // Locate the model matrix in the shader
  m_modelMatrix = m_shader->GetUniformLocation("modelMatrix");
  if (m_modelMatrix == INVALID_UNIFORM_LOCATION)
  {
    printf("m_modelMatrix not found\n");
    return false;
  }

  // Locate the light position vector
  m_LightPosition = m_shader->GetUniformLocation("LightPosition");
  // Locate the ambient product vector
  m_AmbientProduct = m_shader->GetUniformLocation("AmbientProduct");
  // Locate the diffuse product vector
  m_DiffuseProduct = m_shader->GetUniformLocation("DiffuseProduct");
  // Locate the specular product vector
  m_SpecularProduct = m_shader->GetUniformLocation("SpecularProduct");
  // Locate the shininess float
  m_Shininess = m_shader->GetUniformLocation("Shininess");

  m_SpotLightDirection = m_shader->GetUniformLocation("SpotLightDirection");
  m_SpotCutOff = m_shader->GetUniformLocation("SpotLightCutOffAngle");


  return true;
}

void Graphics::SetPerFragLighting()
{
  perFragmentLighting = true;
}


void Graphics::SetPerVertLighting()
{
  perFragmentLighting = false;
}

void Graphics::SetAmbientScalar(float a_slr)
{
  if (a_slr < 0.0)
    amb_Scalar = 0.0;
  else if (a_slr > 2.0)
    amb_Scalar = 2.0;
  else
    amb_Scalar = a_slr;
}


void Graphics::SetDiffuseScalar(float d_slr)
{
  if (d_slr < 0.0)
    diff_Scalar = 0.0;
  else if (d_slr > 3.0)
    diff_Scalar = 3.0;
  else
    diff_Scalar = d_slr;
}


void Graphics::SetSpecularScalar(float s_slr)
{
  spec_Scalar = s_slr;
  if (s_slr < 0.0)
    spec_Scalar = 0.0;
  else if (s_slr > 2.0)
    spec_Scalar = 2.0;
  else
    spec_Scalar = s_slr;
}

float Graphics::getAmbientScalar()
{
  return amb_Scalar;
}


float Graphics::getDiffuseScalar()
{
  return diff_Scalar;
}


float Graphics::getSpecularScalar()
{
  return spec_Scalar;
}

void Graphics::SetSpotLightAngle(int angle)
{
  if (angle < 0)
    spotLightAngle = 0;
  else if (angle > 90)
    spotLightAngle = 12;
  else
    spotLightAngle = angle;
}

int Graphics::getSpotLightAngle()
{
  return spotLightAngle;
}

void Graphics::Update(unsigned int dt, SDL_Event *m_event)
{
  // Handle input
  HandleInput(m_event);

  if(bumperHit1)
  {
    bumperHit1 = false;
  }

  if(bumperHit2)
  {
    bumperHit2 = false;
  }

  if(bumperHit3)
  {
    bumperHit3 = false;
  }

  // Step the physics world
  world.Update(dt);

  // Update the physics objects
  board->Update();
  ball->Update();
  paddle->Update();
  lFlipper->Update();
  rFlipper->Update();
  bumper1->Update();
  tBumper1->Update();
  oBumper1->Update();
  bumper2->Update();
  tBumper2->Update();
  oBumper2->Update();
  bumper3->Update();
  tBumper3->Update();
  oBumper3->Update();


  callback1 = new BumperContactResultCallback(&bumperHit1);
  world.GetWorld()->contactPairTest(ball->GetRigidBody(), oBumper1->GetRigidBody(), *callback1);
  callback2 = new BumperContactResultCallback(&bumperHit2);
  world.GetWorld()->contactPairTest(ball->GetRigidBody(), oBumper2->GetRigidBody(), *callback2);
  callback3 = new BumperContactResultCallback(&bumperHit3);
  world.GetWorld()->contactPairTest(ball->GetRigidBody(), oBumper3->GetRigidBody(), *callback3);

  if(bumperHit1 || bumperHit2 || bumperHit3)
  {
    score += 10;
    printToConsole();
  }

  if (ball->GetRigidBody()->getCenterOfMassPosition().getX() < 3.0
    && ball->GetRigidBody()->getCenterOfMassPosition().getZ() > 6.5)
  {
    lives -= 1;

    if (lives <= 0) {
      lives = 0;
    }
    else
    {
      resetBall();
    }
    printToConsole();
  }
}

void Graphics::HandleInput(SDL_Event *m_event)
{
  if (m_event->type == SDL_MOUSEBUTTONDOWN && m_event->button.button == SDL_BUTTON_LEFT)
  {
    // Possibly could have paddle for left and right mouse here as well
  }

  

  if (m_event->type == SDL_KEYDOWN && m_event->key.keysym.sym == SDLK_F10)
  {
    if(zoom)
    {
      zoom = false;
    }
    else
    {
      zoom = true;
    }
  }
  if (m_event->type == SDL_KEYDOWN && m_event->key.keysym.sym == SDLK_DOWN)
  {
    if (launcherPower < 15.0)
    {
      //launcherPower += 0.25;
      launcherPower += 0.50;
    }

    if (launcherPower > 15.0)
    {
      launcherPower = 15.0;
    }
  }

  if (m_event->type == SDL_KEYDOWN && m_event->key.keysym.sym == SDLK_UP)
  {
    resetBall();
  }

  if (m_event->type == SDL_KEYUP)
  {
    if (m_event->key.keysym.sym == SDLK_DOWN)
    {
      paddle->GetRigidBody()->setLinearVelocity(btVector3(0, 0, -1.0 * launcherPower));
      launcherPower = 0.0;
    }
  }

  //Left Flipper
  if (m_event->type == SDL_KEYDOWN && m_event->key.keysym.sym == SDLK_LEFT)
  {
    btTransform transf;

    if (lFlipperStep < 0.5)
    {
      lFlipperStep += 0.05;
      lFlipper->GetRigidBody()->setLinearVelocity(btVector3(0,0,-15));
    }

    else
    {
      lFlipper->GetRigidBody()->setLinearVelocity(btVector3(0,0,0));
    }

    transf.setOrigin( btVector3(-1.17565, 1.41711, 5.99798) );
    transf.setRotation(btQuaternion(0,lFlipperStep,0,1));
    lFlipper->GetRigidBody()->getMotionState()->setWorldTransform(transf);
    lFlipper->GetRigidBody()->setWorldTransform(transf);
  }

  if (m_event->type == SDL_KEYUP && m_event->key.keysym.sym == SDLK_LEFT)
  {
    btTransform transf;

    lFlipperStep = 0.0;
    lFlipper->GetRigidBody()->setLinearVelocity(btVector3(0,0,0));
    transf.setOrigin( btVector3(-1.17565, 1.41711, 5.99798) );
    transf.setRotation(btQuaternion(0,lFlipperStep,0,1));
    lFlipper->GetRigidBody()->getMotionState()->setWorldTransform(transf);
    lFlipper->GetRigidBody()->setWorldTransform(transf);
  }

  //Right Flipper
  if (m_event->type == SDL_KEYDOWN && m_event->key.keysym.sym == SDLK_RIGHT)
  {
    btTransform transf;

    if (rFlipperStep > -0.5)
    {
      rFlipperStep -= 0.05;
      rFlipper->GetRigidBody()->setLinearVelocity(btVector3(0,0,-10));
    }

    else
    {
      rFlipper->GetRigidBody()->setLinearVelocity(btVector3(0,0,0));
    }

    transf.setOrigin( btVector3(1.15313, 1.41711, 5.99798) );
    transf.setRotation(btQuaternion(0,rFlipperStep,0,1));
    rFlipper->GetRigidBody()->getMotionState()->setWorldTransform(transf);
    rFlipper->GetRigidBody()->setWorldTransform(transf);
  }

  if (m_event->type == SDL_KEYUP && m_event->key.keysym.sym == SDLK_RIGHT)
  {
    btTransform transf;

    rFlipperStep = 0.0;
    rFlipper->GetRigidBody()->setLinearVelocity(btVector3(0,0,0));
    transf.setOrigin( btVector3(1.15313, 1.41711, 5.99798) );
    transf.setRotation(btQuaternion(0,rFlipperStep,0,1));
    rFlipper->GetRigidBody()->getMotionState()->setWorldTransform(transf);
    rFlipper->GetRigidBody()->setWorldTransform(transf);
  }


}

void Graphics::printToConsole()
{
  clear();
  printf("Score: %i\n", score);
  printf("Lives: %i\n", lives);
  if (lives <= 0)
    printf("Game over!\n");
}

void Graphics::resetBall()
{
  btTransform transform;
  btVector3 zeroVector(0,0,0);

  ball->GetRigidBody()->clearForces();
  ball->GetRigidBody()->setLinearVelocity(zeroVector);
  ball->GetRigidBody()->setAngularVelocity(zeroVector);

  transform = ball->GetRigidBody()->getCenterOfMassTransform();
  transform.setOrigin( btVector3(3.03204, 0.2217404, 5.36945) );
  ball->GetRigidBody()->setCenterOfMassTransform(transform);
}

void Graphics::Render()
{
  //clear the screen
  glClearColor(0.0, 0.0, 0.0, 1.0);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  // Start the correct program
  m_shader->Enable();

  // Send in the projection and view to the shader
  glUniformMatrix4fv(m_projectionMatrix, 1, GL_FALSE, glm::value_ptr(m_camera->GetProjection()));
  glUniformMatrix4fv(m_viewMatrix, 1, GL_FALSE, glm::value_ptr(m_camera->GetView(zoom)));

  // Intensity r,g,b,a
  const glm::vec4 ambient = glm::vec4(0.4*amb_Scalar, 0.4*amb_Scalar, 0.4*amb_Scalar, 1.0);
  const glm::vec4 diffuse = glm::vec4(1.0*diff_Scalar, 1.0*diff_Scalar, 1.0*diff_Scalar, 1.0);
  const glm::vec4 specular = glm::vec4(1.0*spec_Scalar, 1.0*spec_Scalar, 1.0*spec_Scalar, 1.0);

  const glm::vec4 light_pos( 0.0f, -5.0f, 0.0f, 1.0f);
  const glm::mat4 mv = m_camera->GetView(zoom)*ball->GetModel();
  const glm::vec4 spotDIR = mv[3] - light_pos;

   // Set material properties.
  glUniform4fv( m_AmbientProduct, 1, glm::value_ptr(ambient) );
  glUniform4fv( m_DiffuseProduct, 1, glm::value_ptr(diffuse) );
  glUniform4fv( m_SpecularProduct, 1, glm::value_ptr(specular) );
  glUniform1f( m_Shininess, 50.0f ); // higher more concentrated specular reflection

  // Set the light position
  glUniform4fv( m_LightPosition, 1, glm::value_ptr(light_pos) );

  // Set spotlight
  glUniform4fv( m_SpotLightDirection, 1, glm::value_ptr(spotDIR));
  glUniform1f( m_SpotCutOff, spotLightAngle ); // angle in degrees

//red 255,0,0

  // Render the object
  glUniformMatrix4fv(m_modelMatrix, 1, GL_FALSE, glm::value_ptr(frame->GetModel()));
  frame->Render();

  glUniformMatrix4fv(m_modelMatrix, 1, GL_FALSE, glm::value_ptr(board->GetModel()));
  board->Render();

  glUniformMatrix4fv(m_modelMatrix, 1, GL_FALSE, glm::value_ptr(ball->GetModel()));
  ball->Render();

  glUniformMatrix4fv(m_modelMatrix, 1, GL_FALSE, glm::value_ptr(paddle->GetModel()));
  paddle->Render();

  glUniformMatrix4fv(m_modelMatrix, 1, GL_FALSE, glm::value_ptr(lFlipper->GetModel()));
  lFlipper->Render();

  glUniformMatrix4fv(m_modelMatrix, 1, GL_FALSE, glm::value_ptr(rFlipper->GetModel()));
  rFlipper->Render();

  glUniformMatrix4fv(m_modelMatrix, 1, GL_FALSE, glm::value_ptr(bumper1->GetModel()));
  bumper1->Render();

  glUniformMatrix4fv(m_modelMatrix, 1, GL_FALSE, glm::value_ptr(bumper2->GetModel()));
  bumper2->Render();

  glUniformMatrix4fv(m_modelMatrix, 1, GL_FALSE, glm::value_ptr(bumper3->GetModel()));
  bumper3->Render();

  glUniformMatrix4fv(m_modelMatrix, 1, GL_FALSE, glm::value_ptr(oBumper1->GetModel()));
  oBumper1->Render();

  glUniformMatrix4fv(m_modelMatrix, 1, GL_FALSE, glm::value_ptr(oBumper2->GetModel()));
  oBumper2->Render();

  glUniformMatrix4fv(m_modelMatrix, 1, GL_FALSE, glm::value_ptr(oBumper3->GetModel()));
  oBumper3->Render();


  //RED GLOW Bumper 1
  if(bumperHit1)
  {
    glUniform4fv( m_AmbientProduct, 1, glm::value_ptr(glm::vec4(255,0,0,1)) );
    glUniform4fv( m_DiffuseProduct, 1, glm::value_ptr(glm::vec4(255,0,0,1)) );
    glUniform4fv( m_SpecularProduct, 1, glm::value_ptr(glm::vec4(255,0,0,1)) );
  }

  glUniformMatrix4fv(m_modelMatrix, 1, GL_FALSE, glm::value_ptr(tBumper1->GetModel()));
  tBumper1->Render();

  glUniform4fv( m_AmbientProduct, 1, glm::value_ptr(ambient) );
  glUniform4fv( m_DiffuseProduct, 1, glm::value_ptr(diffuse) );
  glUniform4fv( m_SpecularProduct, 1, glm::value_ptr(specular) );

  // RED GLOW Bumper 2
  if(bumperHit2)
  {
    glUniform4fv( m_AmbientProduct, 1, glm::value_ptr(glm::vec4(255,0,0,1)) );
    glUniform4fv( m_DiffuseProduct, 1, glm::value_ptr(glm::vec4(255,0,0,1)) );
    glUniform4fv( m_SpecularProduct, 1, glm::value_ptr(glm::vec4(255,0,0,1)) );
  }

  glUniformMatrix4fv(m_modelMatrix, 1, GL_FALSE, glm::value_ptr(tBumper2->GetModel()));
  tBumper2->Render();

  glUniform4fv( m_AmbientProduct, 1, glm::value_ptr(ambient) );
  glUniform4fv( m_DiffuseProduct, 1, glm::value_ptr(diffuse) );
  glUniform4fv( m_SpecularProduct, 1, glm::value_ptr(specular) );

  // RED GLOW Bumper 3
  if(bumperHit3)
  {
    glUniform4fv( m_AmbientProduct, 1, glm::value_ptr(glm::vec4(255,0,0,1)) );
    glUniform4fv( m_DiffuseProduct, 1, glm::value_ptr(glm::vec4(255,0,0,1)) );
    glUniform4fv( m_SpecularProduct, 1, glm::value_ptr(glm::vec4(255,0,0,1)) );
  }

  glUniformMatrix4fv(m_modelMatrix, 1, GL_FALSE, glm::value_ptr(tBumper3->GetModel()));
  tBumper3->Render();

  glUniform4fv( m_AmbientProduct, 1, glm::value_ptr(ambient) );
  glUniform4fv( m_DiffuseProduct, 1, glm::value_ptr(diffuse) );
  glUniform4fv( m_SpecularProduct, 1, glm::value_ptr(specular) );


  //Render Seven Segment Score
  int placeValue = 100;
  int number = 0;
  for(int dig_count = 0; dig_count < 3; dig_count++)
  {
    number = (score/placeValue) % 10;
    placeValue /= 10;

    //Set Segment Color
    glUniform4fv( m_AmbientProduct, 1, glm::value_ptr(glm::vec4(255,255,255,1)) );
    glUniform4fv( m_DiffuseProduct, 1, glm::value_ptr(glm::vec4(255,255,255,1)) );
    glUniform4fv( m_SpecularProduct, 1, glm::value_ptr(glm::vec4(255,255,255,1)) );

    switch(number)
    {
      case 0:
        glUniformMatrix4fv(m_modelMatrix, 1, GL_FALSE, glm::value_ptr(digit[dig_count]->A->GetModel()));
        digit[dig_count]->A->Render();

        glUniformMatrix4fv(m_modelMatrix, 1, GL_FALSE, glm::value_ptr(digit[dig_count]->B->GetModel()));
        digit[dig_count]->B->Render();

        glUniformMatrix4fv(m_modelMatrix, 1, GL_FALSE, glm::value_ptr(digit[dig_count]->C->GetModel()));
        digit[dig_count]->C->Render();

        glUniformMatrix4fv(m_modelMatrix, 1, GL_FALSE, glm::value_ptr(digit[dig_count]->D->GetModel()));
        digit[dig_count]->D->Render();

        glUniformMatrix4fv(m_modelMatrix, 1, GL_FALSE, glm::value_ptr(digit[dig_count]->E->GetModel()));
        digit[dig_count]->E->Render();

        glUniformMatrix4fv(m_modelMatrix, 1, GL_FALSE, glm::value_ptr(digit[dig_count]->F->GetModel()));
        digit[dig_count]->F->Render();

        break;

      case 1:
        glUniformMatrix4fv(m_modelMatrix, 1, GL_FALSE, glm::value_ptr(digit[dig_count]->B->GetModel()));
        digit[dig_count]->B->Render();

        glUniformMatrix4fv(m_modelMatrix, 1, GL_FALSE, glm::value_ptr(digit[dig_count]->C->GetModel()));
        digit[dig_count]->C->Render();

        break;

      case 2:
        glUniformMatrix4fv(m_modelMatrix, 1, GL_FALSE, glm::value_ptr(digit[dig_count]->A->GetModel()));
        digit[dig_count]->A->Render();

        glUniformMatrix4fv(m_modelMatrix, 1, GL_FALSE, glm::value_ptr(digit[dig_count]->B->GetModel()));
        digit[dig_count]->B->Render();

        glUniformMatrix4fv(m_modelMatrix, 1, GL_FALSE, glm::value_ptr(digit[dig_count]->D->GetModel()));
        digit[dig_count]->D->Render();

        glUniformMatrix4fv(m_modelMatrix, 1, GL_FALSE, glm::value_ptr(digit[dig_count]->E->GetModel()));
        digit[dig_count]->E->Render();

        glUniformMatrix4fv(m_modelMatrix, 1, GL_FALSE, glm::value_ptr(digit[dig_count]->G->GetModel()));
        digit[dig_count]->G->Render();

        break;

      case 3:
        glUniformMatrix4fv(m_modelMatrix, 1, GL_FALSE, glm::value_ptr(digit[dig_count]->A->GetModel()));
        digit[dig_count]->A->Render();

        glUniformMatrix4fv(m_modelMatrix, 1, GL_FALSE, glm::value_ptr(digit[dig_count]->B->GetModel()));
        digit[dig_count]->B->Render();

        glUniformMatrix4fv(m_modelMatrix, 1, GL_FALSE, glm::value_ptr(digit[dig_count]->C->GetModel()));
        digit[dig_count]->C->Render();

        glUniformMatrix4fv(m_modelMatrix, 1, GL_FALSE, glm::value_ptr(digit[dig_count]->D->GetModel()));
        digit[dig_count]->D->Render();

        glUniformMatrix4fv(m_modelMatrix, 1, GL_FALSE, glm::value_ptr(digit[dig_count]->G->GetModel()));
        digit[dig_count]->G->Render();

        break;

      case 4:
        glUniformMatrix4fv(m_modelMatrix, 1, GL_FALSE, glm::value_ptr(digit[dig_count]->B->GetModel()));
        digit[dig_count]->B->Render();

        glUniformMatrix4fv(m_modelMatrix, 1, GL_FALSE, glm::value_ptr(digit[dig_count]->C->GetModel()));
        digit[dig_count]->C->Render();

        glUniformMatrix4fv(m_modelMatrix, 1, GL_FALSE, glm::value_ptr(digit[dig_count]->F->GetModel()));
        digit[dig_count]->F->Render();

        glUniformMatrix4fv(m_modelMatrix, 1, GL_FALSE, glm::value_ptr(digit[dig_count]->G->GetModel()));
        digit[dig_count]->G->Render();

        break;

      case 5:
        glUniformMatrix4fv(m_modelMatrix, 1, GL_FALSE, glm::value_ptr(digit[dig_count]->A->GetModel()));
        digit[dig_count]->A->Render();

        glUniformMatrix4fv(m_modelMatrix, 1, GL_FALSE, glm::value_ptr(digit[dig_count]->C->GetModel()));
        digit[dig_count]->C->Render();

        glUniformMatrix4fv(m_modelMatrix, 1, GL_FALSE, glm::value_ptr(digit[dig_count]->D->GetModel()));
        digit[dig_count]->D->Render();

        glUniformMatrix4fv(m_modelMatrix, 1, GL_FALSE, glm::value_ptr(digit[dig_count]->F->GetModel()));
        digit[dig_count]->F->Render();

        glUniformMatrix4fv(m_modelMatrix, 1, GL_FALSE, glm::value_ptr(digit[dig_count]->G->GetModel()));
        digit[dig_count]->G->Render();

        break;

      case 6:
        glUniformMatrix4fv(m_modelMatrix, 1, GL_FALSE, glm::value_ptr(digit[dig_count]->A->GetModel()));
        digit[dig_count]->A->Render();

        glUniformMatrix4fv(m_modelMatrix, 1, GL_FALSE, glm::value_ptr(digit[dig_count]->C->GetModel()));
        digit[dig_count]->C->Render();

        glUniformMatrix4fv(m_modelMatrix, 1, GL_FALSE, glm::value_ptr(digit[dig_count]->D->GetModel()));
        digit[dig_count]->D->Render();

        glUniformMatrix4fv(m_modelMatrix, 1, GL_FALSE, glm::value_ptr(digit[dig_count]->E->GetModel()));
        digit[dig_count]->E->Render();

        glUniformMatrix4fv(m_modelMatrix, 1, GL_FALSE, glm::value_ptr(digit[dig_count]->F->GetModel()));
        digit[dig_count]->F->Render();

        glUniformMatrix4fv(m_modelMatrix, 1, GL_FALSE, glm::value_ptr(digit[dig_count]->G->GetModel()));
        digit[dig_count]->G->Render();

        break;

      case 7:
        glUniformMatrix4fv(m_modelMatrix, 1, GL_FALSE, glm::value_ptr(digit[dig_count]->A->GetModel()));
        digit[dig_count]->A->Render();

        glUniformMatrix4fv(m_modelMatrix, 1, GL_FALSE, glm::value_ptr(digit[dig_count]->B->GetModel()));
        digit[dig_count]->B->Render();

        glUniformMatrix4fv(m_modelMatrix, 1, GL_FALSE, glm::value_ptr(digit[dig_count]->C->GetModel()));
        digit[dig_count]->C->Render();

        break;

      case 8:
        glUniformMatrix4fv(m_modelMatrix, 1, GL_FALSE, glm::value_ptr(digit[dig_count]->A->GetModel()));
        digit[dig_count]->A->Render();

        glUniformMatrix4fv(m_modelMatrix, 1, GL_FALSE, glm::value_ptr(digit[dig_count]->B->GetModel()));
        digit[dig_count]->B->Render();

        glUniformMatrix4fv(m_modelMatrix, 1, GL_FALSE, glm::value_ptr(digit[dig_count]->C->GetModel()));
        digit[dig_count]->C->Render();

        glUniformMatrix4fv(m_modelMatrix, 1, GL_FALSE, glm::value_ptr(digit[dig_count]->D->GetModel()));
        digit[dig_count]->D->Render();

        glUniformMatrix4fv(m_modelMatrix, 1, GL_FALSE, glm::value_ptr(digit[dig_count]->E->GetModel()));
        digit[dig_count]->E->Render();

        glUniformMatrix4fv(m_modelMatrix, 1, GL_FALSE, glm::value_ptr(digit[dig_count]->F->GetModel()));
        digit[dig_count]->F->Render();

        glUniformMatrix4fv(m_modelMatrix, 1, GL_FALSE, glm::value_ptr(digit[dig_count]->G->GetModel()));
        digit[dig_count]->G->Render();

        break;

      case 9:
        glUniformMatrix4fv(m_modelMatrix, 1, GL_FALSE, glm::value_ptr(digit[dig_count]->A->GetModel()));
        digit[dig_count]->A->Render();

        glUniformMatrix4fv(m_modelMatrix, 1, GL_FALSE, glm::value_ptr(digit[dig_count]->B->GetModel()));
        digit[dig_count]->B->Render();

        glUniformMatrix4fv(m_modelMatrix, 1, GL_FALSE, glm::value_ptr(digit[dig_count]->C->GetModel()));
        digit[dig_count]->C->Render();

        glUniformMatrix4fv(m_modelMatrix, 1, GL_FALSE, glm::value_ptr(digit[dig_count]->F->GetModel()));
        digit[dig_count]->F->Render();

        glUniformMatrix4fv(m_modelMatrix, 1, GL_FALSE, glm::value_ptr(digit[dig_count]->G->GetModel()));
        digit[dig_count]->G->Render();
        break;
      default:
        break;
    }


  }





  // Get any errors from OpenGL
  auto error = glGetError();
  if ( error != GL_NO_ERROR )
  {
    string val = ErrorString( error );
    std::cout<< "Error initializing OpenGL! " << error << ", " << val << std::endl;
  }
}

std::string Graphics::ErrorString(GLenum error)
{
  if(error == GL_INVALID_ENUM)
  {
    return "GL_INVALID_ENUM: An unacceptable value is specified for an enumerated argument.";
  }

  else if(error == GL_INVALID_VALUE)
  {
    return "GL_INVALID_VALUE: A numeric argument is out of range.";
  }

  else if(error == GL_INVALID_OPERATION)
  {
    return "GL_INVALID_OPERATION: The specified operation is not allowed in the current state.";
  }

  else if(error == GL_INVALID_FRAMEBUFFER_OPERATION)
  {
    return "GL_INVALID_FRAMEBUFFER_OPERATION: The framebuffer object is not complete.";
  }

  else if(error == GL_OUT_OF_MEMORY)
  {
    return "GL_OUT_OF_MEMORY: There is not enough memory left to execute the command.";
  }
  else
  {
    return "None";
  }
}
