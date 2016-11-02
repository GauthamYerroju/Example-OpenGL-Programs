#include "graphics.hpp"

Graphics::Graphics()
{

}

Graphics::~Graphics()
{

}

bool Graphics::Initialize(int width, int height, char **argv)
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

  held = false;

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

  // Load objects from config file ( path in argv[3] )
  std::ifstream config_file(argv[3]);
  json config;
  config << config_file;
  for (json::iterator it = config.begin(); it != config.end(); ++it)
  {
    auto objectConfig = *it;

    // Load the model
    std::string label = objectConfig["name"];
    std::string modelFile = objectConfig["modelFile"];
    modelFile = "./models/" + modelFile;

    if( label == "Board")
    {
      board = new PhysicsObject( modelFile.c_str() );

      //if( !board->Initialize(PhysicsObject::BOX_SHAPE, 0, btQuaternion(0, 0, 0, 1), btVector3(0, 0, 0) ) )
      if( !board->Initialize(PhysicsObject::TRIANGLE_MESH, 0, btQuaternion(0, 0, 0, 1), btVector3(0, 0, 0) ) )
        printf("PhysicsObject failed to initialize\n");

      world.AddRigidBody(board->GetRigidBody());
    }
    else if( label == "Ball" )
    {
      ball = new PhysicsObject( modelFile.c_str() );

      if( !ball->Initialize(PhysicsObject::SPHERE_SHAPE, 1, btQuaternion(0.0, 0.0, 0.0, 1.0), btVector3(0.0, 0.13, 0.0) ) )
        printf("PhysicsObject failed to initialize\n");

      world.AddRigidBody(ball->GetRigidBody());
    }
    else if(label == "Paddle")
    {
      paddle = new PhysicsObject( modelFile.c_str() );

      if( !paddle->Initialize(PhysicsObject::BOX_SHAPE, 0, btQuaternion(0.0, 0.0, 0.0, 1.0), btVector3(0.0, 1.0, 4.84) ) )
        printf("PhysicsObject failed to initialize\n");

      world.AddRigidBody(paddle->GetRigidBody());

    }
    else if( label == "Bumper")
    {
      bumper = new PhysicsObject( modelFile.c_str() );
      if( !bumper->Initialize(PhysicsObject::TRIANGLE_MESH, 0, btQuaternion(0.0, 0.0, 0.0, 1.0), btVector3(0.0, 0.0, 0.0) ) )
        printf("PhysicsObject failed to initialize\n");

      world.AddRigidBody(bumper->GetRigidBody());
    }

  }

  // Set up the shaders
  m_shader = new Shader();
  if(!m_shader->Initialize())
  {
    printf("Shader Failed to Initialize\n");
    return false;
  }

  // Add the vertex shader
  if(!m_shader->AddShader(GL_VERTEX_SHADER, argv[1]))
  {
    printf("Vertex Shader failed to Initialize\n");
    return false;
  }

  // Add the fragment shader
  if(!m_shader->AddShader(GL_FRAGMENT_SHADER, argv[2]))
  {
    printf("Fragment Shader failed to Initialize\n");
    return false;
  }

  // Connect the program
  if(!m_shader->Finalize())
  {
    printf("Program to Finalize\n");
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

  //enable depth testing
  glEnable(GL_DEPTH_TEST);
  glDepthFunc(GL_LESS);

  return true;
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
  // On mouse click, pick a body
  if (!held && m_event->type == SDL_MOUSEBUTTONDOWN && m_event->button.button == SDL_BUTTON_LEFT)
  {
    // Pick rigid body
    std::cout << "Left Mouse clicked\n";
    held = true;

    // Perform ray test
    glm::vec3 rayDir = m_camera->RayCast(m_event->button.x, m_event->button.y);
    rayDir *= 1.0f;
    glm::vec3 camPos = m_camera->GetPosition();
    btVector3 rayFrom = btVector3(camPos.x, camPos.y, camPos.z);

    glm::vec3 tmp = camPos + rayDir;
    btVector3 rayTo = btVector3(tmp.x, tmp.y, tmp.z);


    RayTest(rayFrom, rayTo);
  }

  // On mouse release, release the rigid body if was picked earlier
  if (held && m_event->type == SDL_MOUSEBUTTONUP && m_event->button.button == SDL_BUTTON_LEFT)
  {
    // Release rigid body
    std::cout << "Left Mouse released\n";
    held = false;
  }

  // On mouse motion, if a rigid body is held, move it around
  if (held && m_event->type == SDL_MOUSEMOTION)
  {
    // Move rigid body
    std::cout << "Mouse is being dragged\n";
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

void Graphics::RayTest(btVector3 rayFrom, btVector3 rayTo)
{
  // Perform ray test and get closest rigid body
  btCollisionWorld::ClosestRayResultCallback rayCallback(rayFrom, rayTo);
  // world.RayTestGetClosest(rayFrom, rayTo, rayCallback);
  world.GetWorld()->rayTest(rayFrom, rayTo, rayCallback);

  if (rayCallback.hasHit())
  {
    std::cout << "Hit!\n";
    btVector3 pickPos = rayCallback.m_hitPointWorld;
    btRigidBody* body = (btRigidBody*)btRigidBody::upcast(rayCallback.m_collisionObject);
    if (body)
    {
      if (!(body->isStaticObject() || body->isKinematicObject()))
      {
        std::cout << "Picked a body\n";
        btRigidBody *m_pickedBody = body;
        // m_savedState = m_pickedBody->getActivationState();
        m_pickedBody->setActivationState(DISABLE_DEACTIVATION);

        btVector3 localPivot = body->getCenterOfMassTransform().inverse() * pickPos;
        btPoint2PointConstraint* p2p = new btPoint2PointConstraint(*body, localPivot);
        world.GetWorld()->addConstraint(p2p, true);
        // m_pickedConstraint = p2p;
        btScalar mousePickClamping = 30.f;
        p2p->m_setting.m_impulseClamp = mousePickClamping;
        p2p->m_setting.m_tau = 0.001f;
      } else {
        std::cout << "Not a dynamic body\n";
      }
    } else {
      std::cout << "No body\n";
    }

    // m_oldPickingPos = rayTo;
    // m_hitPos = pickPos;
    // m_oldPickingDist = (pickPos - rayFrom).length();
  } else {
    std::cout << "Not hit\n";
  }
}

// TODO: Code to handle picking up the object, moving it around and dropping it.

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
