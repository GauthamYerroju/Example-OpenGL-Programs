#include "object.h"

Object::Object(const char *objPath)
{ 
  if(!Model_Loader(objPath)){
    std::cout << "FAILED TO LOAD OBJECT" << std::endl;
  }

  for( unsigned int t_i = 0; t_i < textures.size(); t_i++){

  
    glGenTextures(1, &textures[t_i].textureID);
    
    // Assign texture to ID
    glBindTexture(GL_TEXTURE_2D, textures[t_i].textureID);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, textures[t_i].m_pImage.columns(), textures[t_i].m_pImage.rows(), 0, GL_RGBA, GL_UNSIGNED_BYTE, textures[t_i].m_Blob.data());
  
    //glGenerateMipmap(GL_TEXTURE_2D);  

    // Parameters
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); 
  
    //glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT );
    //glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT );
    //glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR );
    //glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  

    glActiveTexture(textures[t_i].GL_TEXTUREi); // TextureUnit: GL_TEXTURE0, GL_TEXTURE1, etc
    glBindTexture(GL_TEXTURE_2D, textures[t_i].textureID);

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
  glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex,uv_Coords));

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IB);
  
  glDrawElements(GL_TRIANGLES, Indices.size(), GL_UNSIGNED_INT, 0);

  glDisableVertexAttribArray(0);
  glDisableVertexAttribArray(1);
}

bool Object::Model_Loader(const char *filePath)
{
  
  Assimp::Importer importer;
  int face_indx_offset = 0;
  int i = 0;

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

  std::cout << "The number of meshes in this scene: " << scene->mNumMeshes << std::endl << std::endl;

  // Read in vertices and face indices for each mesh
  for( unsigned int mesh_indx = 0; mesh_indx < scene->mNumMeshes; mesh_indx++){
    const aiMesh *mesh = scene->mMeshes[mesh_indx];
    const aiMaterial *mtl = scene->mMaterials[mesh->mMaterialIndex];
    
    std::cout << "MESH #" << (mesh_indx + 1) << std::endl;
    std::cout << "MESH NAME: " << mesh->mName.C_Str() << std::endl << std::endl;
    
    std::cout << "# of primitives (faces:triangles, polygons, lines) in this mesh: " << mesh->mNumFaces << std::endl;
    std::cout << "# of vertices in this mesh: " << mesh->mNumVertices << std::endl;
    std::cout << "The material used by this mesh (MaterialIndex): " << mesh->mMaterialIndex << std::endl;
    std::cout << "# of DIFFUSE textures: " <<  mtl->GetTextureCount(aiTextureType_DIFFUSE) << std::endl; 
    
    // Get vertices for the current mesh
    for( unsigned int vert_indx = 0; vert_indx < mesh->mNumVertices; vert_indx++){
      const aiVector3D v_position = mesh->mVertices[vert_indx];
           
      Vertex vert(glm::vec3(0.0f), glm::vec2(0.0f));
      vert.vertex = glm::vec3(v_position.x, v_position.y, v_position.z);
      
      if(mesh->HasTextureCoords(0)){
        const aiVector3D tex_uv = mesh->mTextureCoords[0][vert_indx];
        vert.uv_Coords = glm::vec2( tex_uv.x, tex_uv.y );
      }

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

    

    for( unsigned int t_indx = 0; t_indx <  mtl->GetTextureCount(aiTextureType_DIFFUSE); t_indx++ ){
      aiString tFileName;
      std::string tPath;

      if (mtl->GetTexture(aiTextureType_DIFFUSE, 0, &tFileName, NULL, NULL, NULL, NULL, NULL) == AI_SUCCESS){
        tPath = "models/" + std::string(tFileName.data);
        if( !Texture_Loader(tPath.c_str(), GL_TEXTURE0 + i ) )
        {
          //do something
        }
        i++;

      }

      std::cout << "Diffuse Texture #" << (t_indx + 1) << " file path: " << tPath << std::endl; 
    }

  }
 
  return true;
}


bool Object::Texture_Loader(const char *filePath, GLenum GL_TEXTUREi )
{
  Texture text;
  try
  {
    
    m_pImage.read(filePath);
    m_pImage.write( &m_Blob, "RGBA" );

    text.setImage(m_pImage);
    text.setBlob(m_Blob);
    text.setGLTEXTUREi(GL_TEXTUREi);
    textures.push_back(text);

  }
  catch( Magick::Error& error )
  {
    printf("Error loading image '%s': '%s'\n", filePath, error.what());
    return false;
  }
/*
  GLuint textureID;
  glGenTextures(1, &textureID);
    
  // Assign texture to ID
  glBindTexture(GL_TEXTURE_2D, textureID);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_pImage.columns(), m_pImage.rows(), 0, GL_RGBA, GL_UNSIGNED_BYTE, m_Blob.data());
  
  //glGenerateMipmap(GL_TEXTURE_2D);  

  // Parameters
  glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); 
  
  //glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT );
  //glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT );
  //glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR );
  //glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  
  std::cout << GL_TEXTUREi << std::endl;
  std::cout << GL_TEXTURE0 << " " << GL_TEXTURE1 << std::endl;
  glActiveTexture(GL_TEXTUREi); // TextureUnit: GL_TEXTURE0, GL_TEXTURE1, etc
  glBindTexture(GL_TEXTURE_2D, textureID);
*/

  return true;
}
