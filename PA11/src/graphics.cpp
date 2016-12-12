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
  zoom = false;

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

  score = 0;
  lives = 3;
  jumping = false;
  currentLevelIndex = 0;
  explosion = false;
  expl_slr = 0.01;

  printToConsole();

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
    else if(label == "Track")
    {
      printf("track init\n");
      json levels = objectConfig["levels"];
      if (levels.size() == 0)
      { 
        printf("No level to load.\n");
        return false;
      }

      track = new GameTrack(
        btTransform( btQuaternion(0, 0, 0, 1), btVector3(0.0, 0.0, 0.0) ) // World tranform
      );

      if (!track->InitializeFromJson(levels[currentLevelIndex])) {
        printf("GameTrack failed to initialize\n");
      }

      track->addToWorld(&world);

    }
    else if(label == "Ship")
    {

      modelFile = objectConfig["modelFile"];
      ship = new PhysicsObject( modelFile.c_str() );
      shipBody = new btCompoundShape();

      shipBottom = new btBoxShape( btVector3(2.5, 0.1, 5.0) );
      shipTop = new btBoxShape( btVector3(3.0, 1.405, 6.0) );
      shipBody->addChildShape(btTransform( btQuaternion(0.0, 0.0, 0.0, 1), btVector3(0.0, -2.0, -2.0) ), shipBottom);
      shipBody->addChildShape(btTransform( btQuaternion(0.0, 0.0, 0.0, 1), btVector3(0.0, 0.0, 0.0) ), shipTop);

      if( !ship->InitializeWithCompoundShape(shipBody,
                                             btTransform( btQuaternion(0.0, 1.0, 0.0, 0), btVector3(0.0, 2.0, -2.0) ),  //WorldTranformation
                                             1,  //Mass
                                             0.4,  //Restitution
                                             0.0   //Friction
                                             )
      )
      printf("PhysicsObject failed to initialize\n");

      ship->GetRigidBody()->setActivationState(DISABLE_DEACTIVATION);
      ship->GetRigidBody()->setDamping(0.5, 30);

      world.AddRigidBody(ship->GetRigidBody());
    }
    else if(label == "Skybox")
    {
      modelFile = objectConfig["modelFile"];
      skyBox = new Object(modelFile.c_str());

      skyBox->Set_Scaler(1000);
      skyBox->Update();
    }
    else if(label == "Cloud")
    {
      modelFile = objectConfig["modelFile"];
      cloud = new Object(modelFile.c_str());

      cloud->Update();
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


  return true;
}

void Graphics::SetPerFragLighting()
{
  perFragmentLighting = true;
  printToConsole();
}

void Graphics::SetPerVertLighting()
{
  perFragmentLighting = false;
  printToConsole();
}

void Graphics::SetAmbientScalar(float a_slr)
{
  if (a_slr < 0.0)
    amb_Scalar = 0.0;
  else if (a_slr > 2.0)
    amb_Scalar = 2.0;
  else
    amb_Scalar = a_slr;
  printToConsole();
}

void Graphics::SetDiffuseScalar(float d_slr)
{
  if (d_slr < 0.0)
    diff_Scalar = 0.0;
  else if (d_slr > 3.0)
    diff_Scalar = 3.0;
  else
    diff_Scalar = d_slr;
  printToConsole();
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
  printToConsole();
}

void Graphics::SetSpotLightAngle(int angle)
{
  if (angle < 0)
    spotLightAngle = 0;
  else if (angle > 90)
    spotLightAngle = 12;
  else
    spotLightAngle = angle;
  printToConsole();
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

int Graphics::getSpotLightAngle()
{
  return spotLightAngle;
}

void Graphics::Update(unsigned int dt, Input *m_input)
{
  // Handle input
  HandleInput(m_input);

  // Step the physics world
  world.Update(dt);

  // Update the physics objects
  track->Update();
  ship->Update();

  // Update collision flags
  bool groundHitTest;
  bool obstacleHitTest;
  CollisionCallback *collTest;


  collTest = new CollisionCallback(&groundHitTest, 0);
  world.GetWorld()->contactPairTest(ship->GetRigidBody(), track->GetBase()->GetRigidBody(), *collTest);
  delete collTest;

  collTest = new CollisionCallback(&groundHitTest, 0);
  world.GetWorld()->contactPairTest(ship->GetRigidBody(), track->GetObstacles()->GetRigidBody(), *collTest);
  delete collTest;

  collTest = new CollisionCallback(&obstacleHitTest, 1);
  world.GetWorld()->contactPairTest(ship->GetRigidBody(), track->GetObstacles()->GetRigidBody(), *collTest);
  delete collTest;

  if (groundHitTest)
    jumping = false;

  if(explosion)
  {
    if(expl_slr < 50)
    {
      cloud->Set_Scaler(expl_slr);
      expl_slr++;
      cloud->Update();
    }
    else
    {
      cloud->Set_Scaler(1.0);
      expl_slr = 0.01;
      explosion = false;
      resetShip();
    }
  }

  if (obstacleHitTest)
  {
    printf("Hit an obstacle at speed: %f\n", ship->GetRigidBody()->getLinearVelocity().getZ());
    if(ship->GetRigidBody()->getLinearVelocity().getZ() < -40)
    {
      
      cloud->Set_TranslationVec(glm::vec3(ship->GetRigidBody()->getCenterOfMassPosition().getX(),
                                          ship->GetRigidBody()->getCenterOfMassPosition().getY(),
                                          ship->GetRigidBody()->getCenterOfMassPosition().getZ()));
      cloud->Update();
      explosion = true;

      
    }
  }

    
  if(ship->GetRigidBody()->getCenterOfMassPosition().getY() < -20 )
    resetShip();
  // callback2 = new BumperContactResultCallback(&bumperHit2);
  // world.GetWorld()->contactPairTest(ship->GetRigidBody(), oBumper2->GetRigidBody(), *callback2);
  // callback3 = new BumperContactResultCallback(&bumperHit3);
  // world.GetWorld()->contactPairTest(ship->GetRigidBody(), oBumper3->GetRigidBody(), *callback3);

  glm::vec3 shipPosition(
    ship->GetRigidBody()->getCenterOfMassPosition().getX(),
    ship->GetRigidBody()->getCenterOfMassPosition().getY(),
    ship->GetRigidBody()->getCenterOfMassPosition().getZ()
  );

  BoundingBox shipBox(
    shipPosition,
    glm::vec3(6.0, 4.2, 14.0) // Ship size
  );
  bool inTunnelBool = track->inTunnel(shipBox);
  if (inTunnelBool) {
    m_camera->SetPosition(glm::vec3(
      shipPosition.x,
      shipPosition.y + 2.0,
      shipPosition.z + 2.75
    ), true);

    m_camera->SetFocusPoint(glm::vec3(
      shipPosition.x,
      shipPosition.y,
      shipPosition.z - 10.0
    ), true);
  }
  else
  {
    m_camera->SetPosition(glm::vec3(
      shipPosition.x,
      shipPosition.y + 16.0,
      shipPosition.z + 40.0
    ), false);

    m_camera->SetFocusPoint(glm::vec3(
      shipPosition.x,
      shipPosition.y,
      shipPosition.z
    ), false);
  }

  skyBox->Set_TranslationVec(shipPosition);
  skyBox->Update();

  m_camera->Update(zoom);
}

void Graphics::HandleInput(Input *m_input)
{
  // NOTE!!! This is just placeholder code,
  //  should be replaced by flags for movement, like pinship flippers

  // Movement: left and right (set velocity)
  btVector3 shipVelocity = ship->GetRigidBody()->getLinearVelocity();
  // Key Down
  if (m_input->KeyDown(SDLK_LEFT))
    ship->GetRigidBody()->setLinearVelocity(btVector3(-25, shipVelocity.y(), shipVelocity.z()));
  if (m_input->KeyDown(SDLK_RIGHT))
    ship->GetRigidBody()->setLinearVelocity(btVector3(25, shipVelocity.y(), shipVelocity.z()));
  // Key Up
  if (m_input->KeyUp(SDLK_LEFT))
    ship->GetRigidBody()->setLinearVelocity(btVector3(0, shipVelocity.y(), shipVelocity.z()));
  if (m_input->KeyUp(SDLK_RIGHT))
    ship->GetRigidBody()->setLinearVelocity(btVector3(0, shipVelocity.y(), shipVelocity.z()));

  // Movement: up and down (set velocity)
  // Key Down
  if (m_input->KeyPressed(SDLK_UP))
  {
    if (shipVelocity.z() > -100.0)
      ship->GetRigidBody()->applyCentralForce(btVector3(0, 0, -100));
  }
  if (m_input->KeyPressed(SDLK_DOWN))
    ship->GetRigidBody()->applyDamping(0.1);

  // Jump
  if (!jumping && m_input->KeyDown(SDLK_b))
  {
    ship->GetRigidBody()->applyCentralImpulse( btVector3(0, 40, 0) );
    jumping = true;
  }

  // Reset the ship
  if (m_input->KeyDown(SDLK_r))
  {
    if (lives == 0 || (m_input->GetModState() & KMOD_SHIFT)) {
      lives = 3;
      score = 0;
      resetShip();
      printToConsole();
    }
  }
}

void Graphics::printToConsole()
{
  return;
  // clear();
  // printf("\n============ Pinball ============\n");
  // printf("  Launch power: ");
  // for(size_t i = 0; i < round(launcherPower); i++)
  //   printf("|");
  // printf("\n  Score: %i\n", score);
  // if (lives <= 0)
  // {
  //   printf("Game over! Press R to reset the game.");
  // }
  // else
  // {
  //   printf("  Balls left: ");
  //   for(size_t i = 0; i < lives; i++)
  //     printf("* ");
  // }
  // printf("\n\n------------- Status ------------\n");
  // printf("  Shader: ");
  // if (perFragmentLighting)
  //   printf("Per-Fragment\n");
  // else
  //   printf("Per-Vertex\n");
  // printf("  Ambient scalar:  %.2f\n", amb_Scalar);
  // printf("  Specular scalar: %.2f\n", spec_Scalar);
  // printf("  Diffuse scalar:  %.2f\n", diff_Scalar);
  // printf("  Spotlight size:  %d degrees\n", spotLightAngle);
  // printf("\n------------ Controls -----------\n");
  // printf("  Pinball Controls:\n");
  // printf("    Down:    Activate plunger (hold for more power)\n");
  // printf("    Left:    Use left and right flippers\n");
  // printf("    Right:   Use left and right flippers\n");
  // printf("    R:       Reset game after game over\n");
  // printf("    Shift+R: Force-reset game\n");
  // printf("    Z:       Change zoom\n");
  // printf("  Lighting Controls:\n");
  // printf("    1:       Change to per-fragment lighting\n");
  // printf("    2:       Change to per-vertex lighting\n");
  // printf("    +:       Increase selected scalar\n");
  // printf("    -:       Decrease selected scalar\n");
  // printf("    A:       Select ambient scalar\n");
  // printf("    S:       Select specular scalar\n");
  // printf("    D:       Select diffuse scalar\n");
  // printf("    C:       Select spotlight radius scalar\n");
}

void Graphics::resetShip()
{
  btTransform transform;
  btVector3 zeroVector(0,0,0);

  ship->GetRigidBody()->clearForces();
  ship->GetRigidBody()->setLinearVelocity(zeroVector);
  ship->GetRigidBody()->setAngularVelocity(zeroVector);

  transform = ship->GetRigidBody()->getCenterOfMassTransform();
  transform.setOrigin( btVector3(0.0, 5.0, -2.0) );
  transform.setRotation(btQuaternion(0.0, 1.0, 0.0, 0));
  ship->GetRigidBody()->setCenterOfMassTransform(transform);
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
  glUniformMatrix4fv(m_viewMatrix, 1, GL_FALSE, glm::value_ptr(m_camera->GetView()));

  // Ship coordinates in view space
  const glm::mat4 mv = m_camera->GetView()*ship->GetModel();

  // Lights
  Lights::DirLight _dirLight;
  Lights::PointLight _pointLight;
  Lights::SpotLight _spotLight;

  //Directional Light
  _dirLight.direction = glm::vec3(0.0);
  _dirLight.ambient = glm::vec3(0.0);
  _dirLight.diffuse = glm::vec3(0.0);
  _dirLight.specular = glm::vec3(0.0);

  lights.dirLights.push_back(_dirLight);

  //Point Light
  _pointLight.position = glm::vec3(mv[3][0], mv[3][1], mv[3][2]);
  _pointLight.ambient = glm::vec3(amb_Scalar * 0.4, amb_Scalar * 0.4, amb_Scalar * 0.4);
  _pointLight.diffuse = glm::vec3(diff_Scalar * 1.0, diff_Scalar * 1.0, diff_Scalar * 1.0);
  _pointLight.specular = glm::vec3(spec_Scalar * 1.0, spec_Scalar * 1.0, spec_Scalar * 1.0);

  // Attenuation (constant = 1.0, linear and quadratic = 0.0 -> No Attenuation)
  _pointLight.constant = 1.0;
  _pointLight.linear = 0.0;
  _pointLight.quadratic = 0.0;

  lights.pointLights.push_back(_pointLight);

    //Spot Light #1 (Top light)
  _spotLight.position = glm::vec3(mv[3][0], mv[3][1] + 2.0, mv[3][2]);
  _spotLight.direction = glm::vec3(mv[3][0], mv[3][1], mv[3][2]) - _spotLight.position;
  _spotLight.cutOffAngle = 50;
  _spotLight.ambient = glm::vec3(0.4, 0.4, 0.4);
  _spotLight.diffuse = glm::vec3(1.0, 1.0, 1.0);
  _spotLight.specular = glm::vec3(1.0, 1.0, 1.0);

  // Attenuation (constant = 1.0, linear and quadratic = 0.0 -> No Attenuation)
  _spotLight.constant = 1.0;
  _spotLight.linear = 0.0;
  _spotLight.quadratic = 0.0;

  lights.spotLights.push_back(_spotLight);

  // Spot Light #2 (Forward Light)
  _spotLight.position = glm::vec3(mv[3][0], mv[3][1], mv[3][2]);
  _spotLight.direction = glm::vec3(0,0,-1);
  _spotLight.cutOffAngle = 45;
  _spotLight.ambient = glm::vec3(0.4, 0.4, 0.4);
  _spotLight.diffuse = glm::vec3(1.0, 1.0, 1.0);
  _spotLight.specular = glm::vec3(1.0, 1.0, 1.0);

  // Attenuation
  _spotLight.constant = 1.0;
  _spotLight.linear = 0.07;
  _spotLight.quadratic = 0.012;

  lights.spotLights.push_back(_spotLight);


  // Directional Lights
  glUniform3fv(m_shader->GetUniformLocation("dirLights[0].direction"), 1, glm::value_ptr(lights.dirLights[0].direction));
  glUniform3fv(m_shader->GetUniformLocation("dirLights[0].ambient"), 1, glm::value_ptr(lights.dirLights[0].ambient));
  glUniform3fv(m_shader->GetUniformLocation("dirLights[0].diffuse"), 1, glm::value_ptr(lights.dirLights[0].diffuse));
  glUniform3fv(m_shader->GetUniformLocation("dirLights[0].specular"), 1, glm::value_ptr(lights.dirLights[0].specular));

  // Point Lights
  glUniform3fv(m_shader->GetUniformLocation("pointLights[0].position"), 1, glm::value_ptr(lights.pointLights[0].position));
  glUniform3fv(m_shader->GetUniformLocation("pointLights[0].ambient"), 1, glm::value_ptr(lights.pointLights[0].ambient));
  glUniform3fv(m_shader->GetUniformLocation("pointLights[0].diffuse"), 1, glm::value_ptr(lights.pointLights[0].diffuse));
  glUniform3fv(m_shader->GetUniformLocation("pointLights[0].specular"), 1, glm::value_ptr(lights.pointLights[0].specular));
  glUniform1fv(m_shader->GetUniformLocation("pointLights[0].constant"), 1, reinterpret_cast<GLfloat *>(&lights.pointLights[0].constant));
  glUniform1fv(m_shader->GetUniformLocation("pointLights[0].linear"), 1,  reinterpret_cast<GLfloat *>(&lights.pointLights[0].linear));
  glUniform1fv(m_shader->GetUniformLocation("pointLights[0].quadratic"), 1, reinterpret_cast<GLfloat *>(&lights.pointLights[0].quadratic));

  // Spot Lights
  glUniform3fv(m_shader->GetUniformLocation("spotLights[0].position"), 1, glm::value_ptr(lights.spotLights[0].position));
  glUniform3fv(m_shader->GetUniformLocation("spotLights[0].direction"), 1, glm::value_ptr(lights.spotLights[0].direction));
  glUniform1fv(m_shader->GetUniformLocation("spotLights[0].cutOffAngle"), 1, reinterpret_cast<GLfloat *>(&lights.spotLights[0].cutOffAngle));
  glUniform3fv(m_shader->GetUniformLocation("spotLights[0].ambient"), 1, glm::value_ptr(lights.spotLights[0].ambient));
  glUniform3fv(m_shader->GetUniformLocation("spotLights[0].diffuse"), 1, glm::value_ptr(lights.spotLights[0].diffuse));
  glUniform3fv(m_shader->GetUniformLocation("spotLights[0].specular"), 1, glm::value_ptr(lights.spotLights[0].specular));
  glUniform1fv(m_shader->GetUniformLocation("spotLights[0].constant"), 1, reinterpret_cast<GLfloat *>(&lights.spotLights[0].constant));
  glUniform1fv(m_shader->GetUniformLocation("spotLights[0].linear"), 1, reinterpret_cast<GLfloat *>(&lights.spotLights[0].linear));
  glUniform1fv(m_shader->GetUniformLocation("spotLights[0].quadratic"), 1, reinterpret_cast<GLfloat *>(&lights.spotLights[0].quadratic));

  glUniform3fv(m_shader->GetUniformLocation("spotLights[1].position"), 1, glm::value_ptr(lights.spotLights[1].position));
  glUniform3fv(m_shader->GetUniformLocation("spotLights[1].direction"), 1, glm::value_ptr(lights.spotLights[1].direction));
  glUniform1fv(m_shader->GetUniformLocation("spotLights[1].cutOffAngle"), 1, reinterpret_cast<GLfloat *>(&lights.spotLights[1].cutOffAngle));
  glUniform3fv(m_shader->GetUniformLocation("spotLights[1].ambient"), 1, glm::value_ptr(lights.spotLights[1].ambient));
  glUniform3fv(m_shader->GetUniformLocation("spotLights[1].diffuse"), 1, glm::value_ptr(lights.spotLights[1].diffuse));
  glUniform3fv(m_shader->GetUniformLocation("spotLights[1].specular"), 1, glm::value_ptr(lights.spotLights[1].specular));
  glUniform1fv(m_shader->GetUniformLocation("spotLights[1].constant"), 1, reinterpret_cast<GLfloat *>(&lights.spotLights[1].constant));
  glUniform1fv(m_shader->GetUniformLocation("spotLights[1].linear"), 1, reinterpret_cast<GLfloat *>(&lights.spotLights[1].linear));
  glUniform1fv(m_shader->GetUniformLocation("spotLights[1].quadratic"), 1, reinterpret_cast<GLfloat *>(&lights.spotLights[1].quadratic));

  // Material
  glUniform1f( m_shader->GetUniformLocation("shininess"), 50.0f ); // higher more concentrated specular reflection


  if(explosion)
  {
    glUniformMatrix4fv(m_modelMatrix, 1, GL_FALSE, glm::value_ptr(cloud->GetModel()));
    cloud->Render();
  }
  // Render the objects
  glUniformMatrix4fv(m_modelMatrix, 1, GL_FALSE, glm::value_ptr(track->GetBase()->GetModel()));
  track->GetBase()->Render();

  glUniformMatrix4fv(m_modelMatrix, 1, GL_FALSE, glm::value_ptr(track->GetObstacles()->GetModel()));
  track->GetObstacles()->Render();

  for(auto & trackObj : track->GetObjects())
  {
    glUniformMatrix4fv(m_modelMatrix, 1, GL_FALSE, glm::value_ptr(trackObj.GetModel()));
    trackObj.Render();
  }

  if(!explosion)
  {
    glUniformMatrix4fv(m_modelMatrix, 1, GL_FALSE, glm::value_ptr(ship->GetModel()));
    ship->Render();
  }
  

  glUniformMatrix4fv(m_modelMatrix, 1, GL_FALSE, glm::value_ptr(skyBox->GetModel()));
  skyBox->Render();

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
