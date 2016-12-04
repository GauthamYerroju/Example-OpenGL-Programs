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
      modelFile = objectConfig["modelFile"];
      track = new GameTrack( modelFile.c_str() );

      if (!track->Initialize(
        btTransform( btQuaternion(0, 0, 0, 1), btVector3(0.0, 0.0, 0.0) ) // World tranform
      )) {
        printf("GameTrack failed to initialize\n");
      }

      world.AddRigidBody(track->GetRigidBody());
    }
    else if(label == "Ball")
    {
      modelFile = objectConfig["modelFile"];
      ball = new PhysicsObject( modelFile.c_str() );

      if( !ball->Initialize(PhysicsObject::BOX_SHAPE,  //CollisionShape
                              1,  //Mass
                              btTransform( btQuaternion(0.0, 1.59, 0.0, 0), btVector3(0.0, 5.0, -4.0) ),  //WorldTranformation
                              0.8,  //Restitution
                              0.0   //Friction
                              ))
        printf("PhysicsObject failed to initialize\n");

      ball->GetRigidBody()->setActivationState(DISABLE_DEACTIVATION);
      ball->GetRigidBody()->setDamping(0.5, 30);

      world.AddRigidBody(ball->GetRigidBody());
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
  ball->Update();

  // callback1 = new BumperContactResultCallback(&bumperHit1);
  // world.GetWorld()->contactPairTest(ball->GetRigidBody(), oBumper1->GetRigidBody(), *callback1);
  // callback2 = new BumperContactResultCallback(&bumperHit2);
  // world.GetWorld()->contactPairTest(ball->GetRigidBody(), oBumper2->GetRigidBody(), *callback2);
  // callback3 = new BumperContactResultCallback(&bumperHit3);
  // world.GetWorld()->contactPairTest(ball->GetRigidBody(), oBumper3->GetRigidBody(), *callback3);

  m_camera->SetPosition(glm::vec3(
    ball->GetRigidBody()->getCenterOfMassPosition().getX(),
    ball->GetRigidBody()->getCenterOfMassPosition().getY() + 16.0,
    ball->GetRigidBody()->getCenterOfMassPosition().getZ() + 40.0
  ));

  m_camera->SetFocusPoint(glm::vec3(
    ball->GetRigidBody()->getCenterOfMassPosition().getX(),
    ball->GetRigidBody()->getCenterOfMassPosition().getY(),
    ball->GetRigidBody()->getCenterOfMassPosition().getZ()
  ));

  m_camera->Update(zoom);
}

void Graphics::HandleInput(Input *m_input)
{
  // NOTE!!! This is just placeholder code,
  //  should be replaced by flags for movement, like pinball flippers

  // Movement: left and right (set velocity)
  btVector3 shipVelocity = ball->GetRigidBody()->getLinearVelocity();
  // Key Down
  if (m_input->KeyDown(SDLK_LEFT))
  {
    ball->GetRigidBody()->setLinearVelocity(btVector3(-10, shipVelocity.y(), shipVelocity.z()));
  }
  if (m_input->KeyDown(SDLK_RIGHT))
    ball->GetRigidBody()->setLinearVelocity(btVector3(10, shipVelocity.y(), shipVelocity.z()));
  // Key Up
  if (m_input->KeyUp(SDLK_LEFT))
    ball->GetRigidBody()->setLinearVelocity(btVector3(0, shipVelocity.y(), shipVelocity.z()));
  else if (m_input->KeyUp(SDLK_RIGHT))
    ball->GetRigidBody()->setLinearVelocity(btVector3(0, shipVelocity.y(), shipVelocity.z()));

  // Movement: up and down (set velocity)
  // Key Down
  else if (m_input->KeyPressed(SDLK_UP))
  {
    if (shipVelocity.z() > -25.0)
      ball->GetRigidBody()->applyCentralForce(btVector3(shipVelocity.x(), shipVelocity.y(), -25));
  }
  else if (m_input->KeyPressed(SDLK_DOWN))
    ball->GetRigidBody()->applyDamping(0.1);

  // Change zoom
  if (m_input->KeyDown(SDLK_z))
  {
    if(ball->GetRigidBody()->getCenterOfMassPosition().getY() < 5.51 &&
              ball->GetRigidBody()->getCenterOfMassPosition().getY() > 5.49)
    {
      ball->GetRigidBody()->applyCentralImpulse( btVector3( 0, 30, 0));
    }
  }

  // Reset the ball
  if (m_input->KeyDown(SDLK_r))
  {
    if (lives == 0 || (m_input->GetModState() & KMOD_SHIFT)) {
      lives = 3;
      score = 0;
      resetBall();
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

void Graphics::resetBall()
{
  btTransform transform;
  btVector3 zeroVector(0,0,0);

  ball->GetRigidBody()->clearForces();
  ball->GetRigidBody()->setLinearVelocity(zeroVector);
  ball->GetRigidBody()->setAngularVelocity(zeroVector);

  transform = ball->GetRigidBody()->getCenterOfMassTransform();
  transform.setOrigin( btVector3(0.0, 5.0, -2.0) );
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
  glUniformMatrix4fv(m_viewMatrix, 1, GL_FALSE, glm::value_ptr(m_camera->GetView()));

  // Intensity r,g,b,a
  const glm::vec4 ambient = glm::vec4(0.4*amb_Scalar, 0.4*amb_Scalar, 0.4*amb_Scalar, 1.0);
  const glm::vec4 diffuse = glm::vec4(1.0*diff_Scalar, 1.0*diff_Scalar, 1.0*diff_Scalar, 1.0);
  const glm::vec4 specular = glm::vec4(1.0*spec_Scalar, 1.0*spec_Scalar, 1.0*spec_Scalar, 1.0);

  const glm::vec4 light_pos(
    0.0f, // ball->GetRigidBody()->getCenterOfMassPosition().getX(),
    3.0f, // ball->GetRigidBody()->getCenterOfMassPosition().getY(),
    0.0f, // ball->GetRigidBody()->getCenterOfMassPosition().getZ(),
    1.0f);
  const glm::mat4 mv = m_camera->GetView()*ball->GetModel();
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

  // Render the objects
  glUniformMatrix4fv(m_modelMatrix, 1, GL_FALSE, glm::value_ptr(track->GetModel()));
  track->Render();

  glUniformMatrix4fv(m_modelMatrix, 1, GL_FALSE, glm::value_ptr(ball->GetModel()));
  ball->Render();

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
