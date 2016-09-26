#include "box.h"

Box::Box(const char *objPath)
{  
  
  if(!Model_Loader(objPath)){
    std::cout << "FAILED TO LOAD OBJECT" << std::endl;
  }

  // The index works at a 0th index
  for(unsigned int i = 0; i < Indices.size(); i++)
  {
    Indices[i] = Indices[i] - 1;
  }

  glGenBuffers(1, &VB);
  glBindBuffer(GL_ARRAY_BUFFER, VB);
  glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * Vertices.size(), &Vertices[0], GL_STATIC_DRAW);

  glGenBuffers(1, &IB);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IB);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * Indices.size(), &Indices[0], GL_STATIC_DRAW);

  angle_rotate = 0.0f;
  rotate_speed = 1.0f;
  scaler = 1.0f;
}

Box::~Box()
{

}

void Box::Update(unsigned int dt, EventFlag e_flags)
{
  // If system not paused
  if( !e_flags.pause_all ){
    if( !e_flags.clockwise_rotate )
    // Set counter clockwise angle of rotation
      angle_rotate += (dt * M_PI/1000) * rotate_speed;
    else if( e_flags.clockwise_rotate )
    // Set clockwise angle of rotation
      angle_rotate -= (dt * M_PI/1000) * rotate_speed;
    }

  rotation = glm::rotate(glm::mat4(1.0f), (angle_rotate), glm::vec3(0.0, 1.0, 0.0));
  scale = glm::scale(glm::mat4(1.0f), glm::vec3(1.0, 1.0, 1.0) * scaler);

  model = rotation * scale;
}


void Box::Set_RotateSpeed(float r_speed)
{
  rotate_speed = r_speed;
}


void Box::Set_Scale( float sclr )
{
  scaler = sclr;
}


bool Box::Model_Loader(const char *filePath){
  std::ifstream fin_obj(filePath);
  std::string line_buffer;
  std::istringstream ss;
  std::string token;
  glm::vec3 vert;
  int face_val;
  float color = 0.0f;

  // Check File
  if( !fin_obj.is_open() ){
    perror(filePath);
    return false;
  }

  //Parse file
  while(std::getline(fin_obj, line_buffer)){

    ss.str(line_buffer);
    ss >> token;
   
    // Vertex
    if(token == "v"){

      ss >> vert.x >> vert.y >> vert.z;
      Vertices.push_back(Vertex(vert,glm::vec3(std::fmod(color,1.0f))));
      color += 0.02f;

    }
    // Face
    else if(token == "f"){

      while(ss.rdbuf()->in_avail() != 0){

        // Get vertex index and store in Indicies
        ss >> face_val;
        Indices.push_back((face_val));

        if(ss.peek() == '/'){
          // Skip texture and normal indicies
          ss >> token;
        }
        
      }
        
    }

    ss.clear();
  }

  // Close file
  fin_obj.close();

  return true;
}