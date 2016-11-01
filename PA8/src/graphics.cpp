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

  m_width = width;
  m_height = height;
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

    if( label == "Box")
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

      if( !ball->Initialize(PhysicsObject::SPHERE_SHAPE, 1, btQuaternion(0, 0, 0, 1), btVector3(0, 0, -0.5) ) )
        printf("PhysicsObject failed to initialize\n");

      world.AddRigidBody(ball->GetRigidBody());
    }
    else if(label == "Paddle")
    {
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

}

void Graphics::HandleInput(SDL_Event *m_event)
{
  // On mouse click, pick a body
  if (!held && m_event->type == SDL_MOUSEBUTTONDOWN && m_event->button.button == SDL_BUTTON_LEFT)
  {
    // Pick rigid body
    std::cout << "Left Mouse clicked\n";
    held = true;

    glm::vec3 tmp = GetRayTo(m_event->button.x, m_event->button.y);
    std::cout << glm::to_string(tmp) << std::endl;
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

  // Get any errors from OpenGL
  auto error = glGetError();
  if ( error != GL_NO_ERROR )
  {
    string val = ErrorString( error );
    std::cout<< "Error initializing OpenGL! " << error << ", " << val << std::endl;
  }
}


glm::vec3 Graphics::GetRayTo(int x, int y)
{
	// Normalized Device Coordinates map 2D coordinate space to a range of [-1, 1]
	// More info here: https://www.youtube.com/watch?v=Ck1SH7oYRFM
	// and here: https://capnramses.github.io//opengl/raycasting.html
	// Ref: http://www.opengl-tutorial.org/miscellaneous/clicking-on-objects/picking-with-a-physics-library/

	// Get the start and end ray positions in NDC (easy to get in NDC)
	glm::vec4 rayStartNDC(
		( (float)x / (float)m_width - 0.5f ) * 2.0f, // [0, m_width] -> [-1,1]
		( (float)y / (float)m_height - 0.5f ) * 2.0f, // [0, m_height] -> [-1,1]
		-1.0, // The near plane maps to Z=-1 in NDC
		1.0f
	);

	glm::vec4 rayEndNDC(
		( (float)x / (float)m_width - 0.5f ) * 2.0f,
		( (float)y / (float)m_height - 0.5f ) * 2.0f,
		0.0,
		1.0f
	);

  // Convert GLint arrays to a glm::mat4
  glm::mat4 projectionMatrix = m_camera->GetProjection();
  glm::mat4 viewMatrix = m_camera->GetView();

	// Convert NDC positions to world space
	glm::mat4 tmp = glm::inverse(projectionMatrix * viewMatrix);

  glm::vec4 rayStartWorld = tmp * rayStartNDC;
  rayStartWorld /= rayStartWorld.w;

	glm::vec4 rayEndWorld = tmp * rayEndNDC;
  rayEndWorld /=rayEndWorld.w;

  // Calculate normalized vector indicating ray direction
  glm::vec3 rayDirection = glm::vec3(rayEndWorld - rayStartWorld);
  rayDirection = glm::normalize(rayDirection);

  return rayDirection;
}

// TODO: Do the actual ray casting to determine if an object is clicked.
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
