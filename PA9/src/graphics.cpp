#include "graphics.hpp"

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

  // Create Physics World
  world.Initialize();

  impulseFlag = false;

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

  for (json::iterator it = config.begin(); it != config.end(); ++it)
  {
    auto objectConfig = *it;

    // Load the model
    std::string label = objectConfig["name"];
    std::string modelFile;
    //modelFile = "./models/" + modelFile;
    
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
    else if(label == "Board")
    {
      modelFile = objectConfig["modelFile"];
      board = new PhysicsObject( modelFile.c_str() );

      //if( !board->Initialize(PhysicsObject::BOX_SHAPE, 0, btQuaternion(0, 0, 0, 1), btVector3(0, 0, 0) ) )
      if( !board->Initialize(PhysicsObject::TRIANGLE_MESH, 0, btQuaternion(0, 0, 0, 1), btVector3(0.0, 0.0, 0.0) ) )
        printf("PhysicsObject failed to initialize\n");

      world.AddRigidBody(board->GetRigidBody());
    }
    else if(label == "Dome")
    {
      dome = new PhysicsObject();
      if( !dome->Initialize(PhysicsObject::STATIC_PLANE_SHAPE, 0, btQuaternion(0.0, 0.0, 0.0, 1.0), btVector3(0.0, 2.2, 0.0) ) )
        printf("PhysicsObject failed to initialize\n");

      world.AddRigidBody(dome->GetRigidBody());
    }
    else if(label == "Ball")
    {
      modelFile = objectConfig["modelFile"];
      ball = new PhysicsObject( modelFile.c_str() );

      if( !ball->Initialize(PhysicsObject::SPHERE_SHAPE, 1, btQuaternion(0.0, 0.0, 0.0, 1.0), btVector3(0.0, 1.5, 0.0) ) )
        printf("PhysicsObject failed to initialize\n");

      world.AddRigidBody(ball->GetRigidBody());
    }
    else if(label == "Paddle")
    {
      modelFile = objectConfig["modelFile"];
      paddle = new PhysicsObject( modelFile.c_str() );

      if( !paddle->Initialize(PhysicsObject::BOX_SHAPE, 10, btQuaternion(0.0, 0.0, 0.0, 1.0), btVector3(0.0, 1.0, 4.84) ) )
        printf("PhysicsObject failed to initialize\n");

      // Constrain linear motion along z axis and disable angular motion
      paddle->GetRigidBody()->setLinearFactor(btVector3(0, 0, 1));
      paddle->GetRigidBody()->setAngularFactor(btVector3(0, 0, 0));

      world.AddRigidBody(paddle->GetRigidBody());
    }
    else if(label == "Bumper")
    {
      modelFile = objectConfig["modelFile"];
      bumper = new PhysicsObject( modelFile.c_str() );
      if( !bumper->Initialize(PhysicsObject::TRIANGLE_MESH, 0, btQuaternion(0.0, 0.0, 0.0, 1.0), btVector3(0.0, 0.0, 0.0) ) )
        printf("PhysicsObject failed to initialize\n");

      world.AddRigidBody(bumper->GetRigidBody());
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

  //m_SpotLightDirection = m_shader->GetUniformLocation("SpotLightDirection");
  //m_SpotCutOff = m_shader->GetUniformLocation("SpotCutOff");
  

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

void Graphics::Update(unsigned int dt, SDL_Event *m_event)
{
  // Handle input
  HandleInput(m_event);

  // Step the physics world
  world.Update(dt);

  // Update the physics objects
  board->Update();
  ball->Update();
  paddle->Update();
  bumper->Update();

}

void Graphics::HandleInput(SDL_Event *m_event)
{
  if (!impulseFlag && m_event->type == SDL_MOUSEBUTTONDOWN && m_event->button.button == SDL_BUTTON_LEFT)
  {
    // Hit the paddle
    paddle->GetRigidBody()->applyCentralImpulse( btVector3(0, 0, -75) );

    impulseFlag = true;
  }
  if (m_event->type == SDL_MOUSEBUTTONUP && m_event->button.button == SDL_BUTTON_LEFT)
  {
    // Reset the impulse flag
    impulseFlag = false;
  }
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

  const glm::vec4 white(1);
  const glm::vec4 black(0);
  // Ambient Intesnity: r, g, b, a
  const glm::vec4 ambient( 0.4f, 0.4f, 0.4f, 1.0);
  const glm::vec4 light_pos( 0.0f, 10.0f, 0.0f, 1.0f);

  // Set the light position
  glUniform4fv( m_LightPosition, 1, glm::value_ptr(light_pos) );
  //glUniform4fv( g_uniformLightColor, 1, glm::value_ptr(white) );
  glUniform4fv( m_AmbientProduct, 1, glm::value_ptr(ambient) );

  // Set material properties.
  //glUniform4fv( g_uniformMaterialEmissive, 1, glm::value_ptr(black) );
  glUniform4fv( m_DiffuseProduct, 1, glm::value_ptr(white) );
  glUniform4fv( m_SpecularProduct, 1, glm::value_ptr(white) );
  glUniform1f( m_Shininess, 100.0f ); // higher more concentrated specular reflection

  //glm::mat4 mv = m_camera->GetView()*ball->GetModel();
  //glm::vec4 spotDIR = mv[3] - light_pos;
  //glUniform4fv( m_SpotLightDirection, 1, glm::value_ptr(spotDIR));
  //glUniform1f( m_SpotCutOff, cos(M_PI/4) );


  // Render the object
  glUniformMatrix4fv(m_modelMatrix, 1, GL_FALSE, glm::value_ptr(board->GetModel()));
  board->Render();
  glUniformMatrix4fv(m_modelMatrix, 1, GL_FALSE, glm::value_ptr(ball->GetModel()));
  ball->Render();
  glUniformMatrix4fv(m_modelMatrix, 1, GL_FALSE, glm::value_ptr(paddle->GetModel()));
  paddle->Render();
  glUniformMatrix4fv(m_modelMatrix, 1, GL_FALSE, glm::value_ptr(bumper->GetModel()));
  bumper->Render();

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
