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

  // Load objects from config file ( path in argv[3] )
  std::ifstream config_file(argv[3]);
  json config;
  config << config_file;
  for (json::iterator it = config.begin(); it != config.end(); ++it)
  {
    auto objectConfig = *it;

    // Load the modelm
    std::string modelFile = objectConfig["modelFile"];
    modelFile = "./models/" + modelFile;

    std::string label = objectConfig["name"];
    // std::string parent = objectConfig["parent"];
    // float spinSpeed = objectConfig["spinSpeed"];
    // float scaler = objectConfig["radius"];
    // float radScaler = 200000;

    Object * obj = new Object( modelFile.c_str(), label.c_str(), NULL );
    objects.push_back( obj );
  }

  // Assign parents
  // for( unsigned int i = 0; i < objects.size(); i++ )
  // {
  //   if (objects[i]->Get_ParentName() != "null")
  //   {
  //     for( unsigned int j = 0; j < objects.size(); j++ )
  //     {
  //       if (objects[i]->Get_ParentName() == objects[j]->Get_Name())
  //       {
  //         objects[i]->Set_Parent( objects[j] );
  //         break;
  //       }
  //     }
  //   }
  // }

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

void Graphics::Update(unsigned int dt, EventFlag e_flags, ViewUpdate viewUpdate)
{
  // Planet position vector
  glm::vec3 tempPos(0.0f);

  // if(viewUpdate.zoom)
  // {
  //   // Zoom on planet indicated by pIndx
  //   int pIndx = viewUpdate.planetIndx;
  //   tempPos.x = objects[pIndx]->GetModel()[3][0];
  //   tempPos.y = objects[pIndx]->GetModel()[3][1];
  //   tempPos.z = objects[pIndx]->GetModel()[3][2];
  //   // Update camera to zoom on planet position offset by scaler
  //   m_camera->ZoomOnPlanet(tempPos, viewUpdate.pViewScaler);
  // }

  if(viewUpdate.resetPos){
    m_camera->ResetPosition();
  }

	// Update the camera
	if (viewUpdate.processed == false){

		m_camera->ProcessInput(viewUpdate);
	}

  // Update the objects
  for( unsigned int i = 0; i < objects.size(); i++ )
  {
    objects[i]->Update(dt, e_flags);
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
  for( unsigned int i = 0; i < objects.size(); i++ )
  {
    glUniformMatrix4fv(m_modelMatrix, 1, GL_FALSE, glm::value_ptr(objects[i]->GetModel()));
    objects[i]->Render();
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
