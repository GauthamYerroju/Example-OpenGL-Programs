#include "object.hpp"

Object::Object()
{
  origin = glm::mat4(1.0f);
  translationVec = glm::vec3(0.0f);
  rotationAngle = 0.0f;
  scaler = 1.0f;
}

Object::Object(const char *objPath)
{

  if(!Model_Loader(objPath)){
    std::cout << "FAILED TO LOAD OBJECT" << std::endl;
  }

  for( unsigned int MeshIndx = 0; MeshIndx < meshes.size(); MeshIndx++ )
  {
    meshes[MeshIndx].Initialize();
  }

  origin = glm::mat4(1.0f);
  translationVec = glm::vec3(0.0f);
  rotationAngle = 0.0f;
  scaler = 1.0f;

}

Object::~Object()
{
}

void Object::Update()
{
  translation = glm::translate(origin, translationVec);
  rotation = glm::rotate(glm::mat4(1.0f), (rotationAngle), glm::vec3(0.0, 1.0, 0.0));
  scale = glm::scale(glm::mat4(1.0f), glm::vec3(1.0, 1.0, 1.0) * scaler);

  model = translation * rotation * scale;
}


void Object::Set_Origin( glm::mat4 orig )
{
  origin = orig;
}


void Object::Set_TranslationVec( glm::vec3 tVec )
{

  translationVec = tVec;
}

void Object::Set_RotationAngle( float rotAngle )
{
  rotationAngle = rotAngle;
}


void Object::Set_Scaler( float sclr )
{
  scaler = sclr;
}


glm::mat4 Object::GetModel()
{
  return model;
}


glm::mat4 Object::GetPosition()
{
  return translation;
}


void Object::Render()
{
  for( unsigned int meshIndx = 0; meshIndx < meshes.size(); meshIndx++ )
  {
    meshes[meshIndx].Render();
  }
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

    // Get vertices, uv coordinates, and normals for the current mesh
    for( unsigned int vert_indx = 0; vert_indx < mesh->mNumVertices; vert_indx++){
      
      Vertex vert(glm::vec3(0.0f), glm::vec2(0.0f), glm::vec3(0.0f));

      // Vertex coordinates
      if(mesh->HasPositions())
      {
        const aiVector3D v_position = mesh->mVertices[vert_indx];
        vert.vertex = glm::vec3(v_position.x, v_position.y, v_position.z);
      }
      else
        printf("%s: Mesh does not have vertices/n", mesh->mName);
      
      // Normals
      if(mesh->HasNormals())
      {
        const aiVector3D v_normal = mesh->mNormals[vert_indx];
        vert.normal = glm::vec3(v_normal.x, v_normal.y, v_normal.z);
      }
      else
        printf("%s: Mesh does not have normals/n", mesh->mName);

      // uv coordinates
      if(mesh->HasTextureCoords(0)){
        const aiVector3D tex_uv = mesh->mTextureCoords[0][vert_indx];
        vert.uv_Coords = glm::vec2( tex_uv.x, tex_uv.y );
      }
      else
        printf("%s: Mesh does not have texture coordinates/n", mesh->mName);

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
