#include "object.h"

Object::Object(const char *objPath)
{ 
  if(!Model_Loader(objPath)){
    std::cout << "FAILED TO LOAD OBJECT" << std::endl;
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

Object::~Object()
{
  Vertices.clear();
  Indices.clear();
}

void Object::Update(unsigned int dt, EventFlag e_flags)
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

void Object::Set_RotateSpeed(float r_speed)
{
  rotate_speed = r_speed;
}


void Object::Set_Scale( float sclr )
{
  scaler = sclr;
}

glm::mat4 Object::GetModel()
{
  return model;
}

void Object::Render()
{
  glEnableVertexAttribArray(0);
  glEnableVertexAttribArray(1);

  glBindBuffer(GL_ARRAY_BUFFER, VB);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex,color));

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IB);

  glDrawElements(GL_TRIANGLES, Indices.size(), GL_UNSIGNED_INT, 0);

  glDisableVertexAttribArray(0);
  glDisableVertexAttribArray(1);
}

bool Object::Model_Loader(const char *filePath)
{
  
  Assimp::Importer importer;
  int face_indx_offset = 0;

  /*
  aiProcessPreset_TargetRealtime_Fast
  ( aiProcess_CalcTangentSpace |  
    aiProcess_GenNormals |  
    aiProcess_JoinIdenticalVertices |  
    aiProcess_Triangulate |  
    aiProcess_GenUVCoords |  
    aiProcess_SortByPType |  
    0 
  )
  */
  const aiScene *scene = importer.ReadFile( filePath, aiProcessPreset_TargetRealtime_Fast );
 
  // Failed to import from file
  if( !scene )
  {
    printf("Error opening '%s': '%s'\n", filePath, importer.GetErrorString());
    return false;
  }

  // Read in vertices and face indices for each mesh
  for( unsigned int mesh_indx = 0; mesh_indx < scene->mNumMeshes; mesh_indx++){
    const aiMesh *mesh = scene->mMeshes[mesh_indx];
    const aiMaterial *mtl = scene->mMaterials[mesh->mMaterialIndex];
    
    // Get the diffuse color for the current mesh
    aiColor3D color (0.0f,0.0f,0.0f);
    mtl->Get(AI_MATKEY_COLOR_DIFFUSE,color);
    
    // Get vertices for the current mesh
    for( unsigned int vert_indx = 0; vert_indx < mesh->mNumVertices; vert_indx++){
      const aiVector3D v_position = mesh->mVertices[vert_indx];
             
      Vertex vert(glm::vec3(0.0f), glm::vec3(0.0f));
      vert.vertex = glm::vec3(v_position.x, v_position.y, v_position.z);
      vert.color = glm::vec3( color.r, color.g, color.b);
      Vertices.push_back(vert);
    }

    // Get face indices for the current mesh
    for( unsigned int face_indx = 0; face_indx < mesh->mNumFaces; face_indx++){
      const aiFace face = mesh->mFaces[face_indx];

      for( unsigned int indx = 0; indx < face.mNumIndices; indx++){
        Indices.push_back(face.mIndices[indx] + face_indx_offset);
      }
    }
    
    // Set vertex offset to current size of Verticies vector for next mesh
    face_indx_offset = Vertices.size();
  }
 
  return true;
}

