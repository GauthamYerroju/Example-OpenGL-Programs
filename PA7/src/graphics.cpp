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

  //Load the spk file for planet distances
  furnsh_c ( "spk/de421.bsp" ); 

  // Create the object
  sun = new Object(argv[3], "sun", NULL);
  mercury = new Object(argv[3], "mercury", "sun"); //parent is the sun
  venus = new Object(argv[3], "venus", "sun"); //parent is the sun
  earth = new Object(argv[3], "earth", "sun"); //parent is the sun
  moon = new Object(argv[3], "moon", "earth"); //parent is the earth

  // Initialize box attributes
  sun->Set_RotateSpeed(0.040866367);
  mercury->Set_RotateSpeed(0.005682787);
  venus->Set_RotateSpeed(0.004114803);
  earth->Set_RotateSpeed(1.0);
  moon->Set_RotateSpeed(0.036600542);



  sun->Set_Scale( log(109.078080903) );
  mercury->Set_Scale( log(0.383/2) );
  venus->Set_Scale( log(0.949/2) );
  earth->Set_Scale( log(1) ); 
  moon->Set_Scale( log(0.2724/2) );
  
  /*
  mercury->Set_RadScale( log(0.000156789) );
  venus->Set_RadScale( log(0.000156789) );
  earth->Set_RadScale( log(0.000156789) );
  moon->Set_RadScale( log(0.000156789) );
*/
  
  mercury->Set_RadScale( (0.000156789) );
  venus->Set_RadScale( (0.000156789) );
  earth->Set_RadScale( (0.000156789) );
  moon->Set_RadScale( (0.000156789) );


 // sun->Set_Scale( 398.048947948f * 0.05f );
  //mercury->Set_Scale( 1.39606064f * 3.05f );
  //mercury->Set_RadScale(0.000002222);
  //venus->Set_Scale( 3.462688112f * 3.05f );
  //venus->Set_RadScale(0.000002222);
  //earth->Set_Scale( 3.645205876f * 3.05f );
  //earth->Set_RadScale(0.000002222);
  //moon->Set_Scale( 0.993834972 * 3.05f );
  //moon->Set_RadScale(0.000002222*20);
  // unit scale 0.000572156
  
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

void Graphics::Update(unsigned int dt, vector<EventFlag> e_flags)
{

  // Update the object
  sun->Update(dt, e_flags[0]);  //parent first
  mercury->Set_OrbitCenter(sun->GetPosition()); //child second; also parent for moon
  mercury->Update(dt, e_flags[0]);
  venus->Set_OrbitCenter(sun->GetPosition()); //child second; also parent for moon
  venus->Update(dt, e_flags[0]);
  earth->Set_OrbitCenter(sun->GetPosition()); //child second; also parent for moon
  earth->Update(dt, e_flags[0]);
  moon->Set_OrbitCenter(earth->GetPosition()); //child of earth
  moon->Update(dt, e_flags[0]);


}

void Graphics::Render()
{
  //clear the screen
  //glClearColor(0.0, 0.0, 0.2, 1.0);
  glClearColor(0.0, 0.0, 0.0, 1.0);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  // Start the correct program
  m_shader->Enable();

  // Send in the projection and view to the shader
  glUniformMatrix4fv(m_projectionMatrix, 1, GL_FALSE, glm::value_ptr(m_camera->GetProjection())); 
  glUniformMatrix4fv(m_viewMatrix, 1, GL_FALSE, glm::value_ptr(m_camera->GetView()));

  // Render the object
  glUniformMatrix4fv(m_modelMatrix, 1, GL_FALSE, glm::value_ptr(sun->GetModel()));
  sun->Render(); 
  
  glUniformMatrix4fv(m_modelMatrix, 1, GL_FALSE, glm::value_ptr(mercury->GetModel()));
  mercury->Render();

  glUniformMatrix4fv(m_modelMatrix, 1, GL_FALSE, glm::value_ptr(venus->GetModel()));
  venus->Render();

  glUniformMatrix4fv(m_modelMatrix, 1, GL_FALSE, glm::value_ptr(earth->GetModel()));
  earth->Render();

  glUniformMatrix4fv(m_modelMatrix, 1, GL_FALSE, glm::value_ptr(moon->GetModel()));
  moon->Render();

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