#include "object.hpp"

Object::Object(const char *objPath, const char *planet_name, const char *planet_orbiting)
{ 
  et = 0.0;
  orbit_step = 0;
  parent = NULL;

  planet = std::string(planet_name);

  if( planet_orbiting == NULL ){
    orbit_planet = "null";
  }
  else
  {
    orbit_planet = std::string(planet_orbiting);
  }
  
  if(!Model_Loader(objPath)){
    std::cout << "FAILED TO LOAD OBJECT" << std::endl;
  }

  for( unsigned int i = 0; i < meshes.size(); i++ )
  {
    meshes[i].Initialize();
  }
  
  orbit_center = glm::mat4(1.0f);
  orbit_radius = glm::vec3(0.0f);
  angle_rotate = 0.0f;
  rotate_speed = 1.0f;
  orbit_speed = 3600;
  scaler = 1.0f;
  speed_sclr = 1.0f;
  rad_scaler = 1.0f;
}

Object::~Object()
{
  delete parent;
}

void Object::Update(unsigned int dt, EventFlag e_flags)
{
  //std::cout << "\tUpdating...\n";
  // If system not paused
  if( !e_flags.pause_all ){

    if(parent != NULL)
    {
      if(e_flags.incrSpeed){
        if( (speed_sclr + 2) >=1 & (speed_sclr + 2) < 10)
          speed_sclr += 2;
      }
      if(e_flags.dcrSpeed){
        if( (speed_sclr - 2) >=1 & (speed_sclr - 2) < 10)
          speed_sclr -= 2;
      }

      orbit_center = parent->GetPosition();

      double dist[3];
      spkpos_c(planet.c_str(), et, "ECLIPJ2000", "None", orbit_planet.c_str(), dist, &lt);
     
      //counterclockwise
      orbit_radius = glm::vec3((float)dist[1]/rad_scaler, (float)dist[2]/rad_scaler, (float)dist[0]/rad_scaler);
      
      et = orbit_speed * orbit_step * speed_sclr;
      orbit_step++; 
    }

    angle_rotate += (dt * M_PI/1000) * rotate_speed * speed_sclr;     
  }

  translation = glm::translate(orbit_center, orbit_radius);
  rotation = glm::rotate(glm::mat4(1.0f), (angle_rotate), glm::vec3(0.0, 1.0, 0.0));
  scale = glm::scale(glm::mat4(1.0f), glm::vec3(1.0, 1.0, 1.0) * scaler);

  model = translation * rotation * scale;
}

void Object::Set_RotateSpeed(float r_speed)
{
  rotate_speed = r_speed;
}


void Object::Set_Scale( float sclr )
{
  scaler = sclr;
}

void Object::Set_RadScale( float r_sclr )
{
  rad_scaler = r_sclr;
}

glm::mat4 Object::GetModel()
{
  return model;
}

glm::mat4 Object::GetPosition(){
  return translation;
}

std::string Object::Get_Name()
{
  return planet;
}

std::string Object::Get_ParentName()
{
  return orbit_planet;
}

void Object::Set_Parent(Object * parentPointer)
{
  parent = parentPointer;
}

void Object::Render()
{
  for( unsigned int i = 0; i < meshes.size(); i++ )
  {
    meshes[i].Render();
  }
  //std::cout << planet << " rendered\n";
}

bool Object::Model_Loader(const char *filePath)
{
  
  Assimp::Importer importer;

  const aiScene *scene = importer.ReadFile( filePath, aiProcessPreset_TargetRealtime_Fast | aiProcess_Triangulate );
 
  // Check if import successful
  if( !scene )
  {
    printf("Error opening '%s': '%s'\n", filePath, importer.GetErrorString());
    return false;
  }

  // Read in vertices and face indices for each mesh
  for( unsigned int mesh_indx = 0; mesh_indx < scene->mNumMeshes; mesh_indx++)
  {
    Mesh *tmpMesh = new Mesh();
    
    const aiMesh *mesh = scene->mMeshes[mesh_indx];
    const aiMaterial *mtl = scene->mMaterials[mesh->mMaterialIndex];
    
    // Get vertex and uv coordinates for the current mesh
    for( unsigned int vert_indx = 0; vert_indx < mesh->mNumVertices; vert_indx++){
      const aiVector3D v_position = mesh->mVertices[vert_indx];        
      Vertex vert(glm::vec3(0.0f), glm::vec2(0.0f));

      // Vertex coordinates
      vert.vertex = glm::vec3(v_position.x, v_position.y, v_position.z);
      
      // uv coordinates
      if(mesh->HasTextureCoords(0)){
        const aiVector3D tex_uv = mesh->mTextureCoords[0][vert_indx];
        vert.uv_Coords = glm::vec2( tex_uv.x, tex_uv.y );
      }

      tmpMesh->Vertices.push_back(vert);
    }

    // Get face indices for the current mesh
    for( unsigned int face_indx = 0; face_indx < mesh->mNumFaces; face_indx++){
      const aiFace face = mesh->mFaces[face_indx];

      for( unsigned int indx = 0; indx < face.mNumIndices; indx++){
        tmpMesh->Indices.push_back(face.mIndices[indx]);
      }
    }

    // Get textures for the current mesh
    for( unsigned int t_indx = 0; t_indx <  mtl->GetTextureCount(aiTextureType_DIFFUSE); t_indx++ )
    {
      aiString tFileName;
      std::string tPath;

      if ( mtl->GetTexture(aiTextureType_DIFFUSE, 0, &tFileName, NULL, NULL, NULL, NULL, NULL) == AI_SUCCESS )
      {
        tPath = "models/" + std::string( tFileName.data );
        if ( !Texture_Loader(tPath.c_str(), tmpMesh ) )
        {
          printf("Failed to load diffuse texture #%d for '%s'\n", mtl->GetTextureCount(aiTextureType_DIFFUSE), mesh->mName.C_Str() );
        }
      }
    }

    meshes.push_back(*tmpMesh);
  }
 
  return true;
}

bool Object::Texture_Loader(const char *filePath, Mesh *mesh )
{
  GLuint textureID;
  
  Magick::Blob m_Blob;
  Magick::Image m_pImage;

  try
  {
    m_pImage.read( filePath );
    m_pImage.write( &m_Blob, "RGBA" );
    
    glGenTextures(1, &textureID);

    // Assign texture to ID
    glBindTexture(GL_TEXTURE_2D, textureID);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_pImage.columns(), m_pImage.rows(), 0, GL_RGBA, GL_UNSIGNED_BYTE, m_Blob.data());
    
    // Parameters
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    mesh->textures.push_back(textureID);
  }
  catch( Magick::Error& error )
  {
    printf("Error loading image '%s': %s\n", filePath, error.what());
    return false;
  }

  return true;
}
