#include "object2.hpp"

Object2::Object2()
{
  origin = glm::mat4(1.0f);
  translationVec = glm::vec3(0.0f);
  rotationAngle = 0.0f;
  rotationAxis = glm::vec3(0.0f);
  scaler = 1.0f;
}

Object2::Object2(const char *objPath)
{

  if(!loadLevel()){
    std::cout << "FAILED TO LOAD OBJECT" << std::endl;
  }

  for( unsigned int MeshIndx = 0; MeshIndx < meshes.size(); MeshIndx++ )
  {
    meshes[MeshIndx].Initialize();
  }

  origin = glm::mat4(1.0f);
  translationVec = glm::vec3(0.0f);
  rotationAngle = 0.0f;
  rotationAxis = glm::vec3(1.0f);
  scaler = 1.0f;

}

Object2::~Object2()
{
}

void Object2::Update()
{
  translation = glm::translate(origin, translationVec);
  rotation = glm::rotate(glm::mat4(1.0f), (rotationAngle), rotationAxis);
  scale = glm::scale(glm::mat4(1.0f), glm::vec3(1.0, 1.0, 1.0) * scaler);

  model = translation * rotation * scale;
}

void Object2::Set_Origin( glm::mat4 orig )
{
  origin = orig;
}


void Object2::Set_TranslationVec( glm::vec3 tVec )
{

  translationVec = tVec;
}

void Object2::Set_RotationAngle( float rotAngle, glm::vec3 rotAxis )
{
  rotationAngle = rotAngle;
  rotationAxis = rotAxis;
}


void Object2::Set_Scaler( float sclr )
{
  scaler = sclr;
}


glm::mat4 Object2::GetModel()
{
  return model;
}


glm::mat4 Object2::GetPosition()
{
  return translation;
}


void Object2::Render()
{
  for( unsigned int meshIndx = 0; meshIndx < meshes.size(); meshIndx++ )
  {
    meshes[meshIndx].Render();
  }
}

bool Object2::loadLevel()
{
  // Level data
  int lData[7][4] = {
    {71, -1, -1, -1},
    {20, 11, 20, 11},
    {71, -1, -1, -1},
    {71, -1, -1, -1},
    {71, -1, -1, -1},
    {20, 11, 20, 11},
    {71, -1, -1, -1}
  };

  // Level gen loop
  int lastLength[7] = {0, 0, 0, 0, 0, 0, 0};
  float cubeLength = 2.0f;

  for(unsigned int lvi = 0; lvi < 7; lvi++) {
    for(unsigned int lvj = 0; lvj < 4; lvj++) {
      if (lData[lvi][lvj] == -1)
        break;
      
      float scale = (float) ( lData[lvi][lvj] / 10 );
      int terrain = lData[lvi][lvj];
      while (terrain >= 10) {
        terrain -= 10;
      }
      printf("\n[%d, %d, %d]: Scale: %f, terrain: %d\t", lvi, lvj, lData[lvi][lvj], scale, terrain);
      // Create mesh only if terrain exists here
      if (terrain == 1) {
        Mesh segment = Model_Loader( "models/Cube.obj", lvi, scale, cubeLength, lastLength[lvi] );
        meshes.push_back(segment);
        printf("Mesh added for [%d, %d]\n", lvi, lvj);
      }
      // Update last length = last length + scaled cube length of current block
      lastLength[lvi] += cubeLength * scale;
    }
  }
  return true;
}

Mesh Object2::Model_Loader(const char *filePath, int lane, float scale, float cubeLength, int lastLength)
{
  Mesh oneMesh;

  Assimp::Importer importer;

  const aiScene *scene = importer.ReadFile( filePath, aiProcessPreset_TargetRealtime_Fast | aiProcess_Triangulate );

  // Check if import successful
  if( !scene )
  {
    printf("Error opening '%s': '%s'\n", filePath, importer.GetErrorString());
    return oneMesh;
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
        // Scale cube along Z
        vert.vertex.z = vert.vertex.z * scale;
        // Offset z by half of cube length (bring it to start line)
        vert.vertex.z -= lastLength + (cubeLength * scale / 2.0f) - 1.0f;
        // Offset x by lane number
        vert.vertex.x += ( (float)lane - 3.0f ) * (cubeLength);
        // Offset y by constant amount (for now)
        vert.vertex.y += 1.0f;
      }
      else
        printf("%s: Mesh does not have vertices/n", mesh->mName.C_Str());

      // Normals
      if(mesh->HasNormals())
      {
        const aiVector3D v_normal = mesh->mNormals[vert_indx];
        vert.normal = glm::vec3(v_normal.x, v_normal.y, v_normal.z);
      }
      else
        printf("%s: Mesh does not have normals/n", mesh->mName.C_Str());

      // uv coordinates
      if(mesh->HasTextureCoords(0)){
        const aiVector3D tex_uv = mesh->mTextureCoords[0][vert_indx];
        vert.uv_Coords = glm::vec2( tex_uv.x, tex_uv.y );
      }
      else
        printf("%s: Mesh does not have texture coordinates/n", mesh->mName.C_Str());

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

    // meshes.push_back(*tmpMesh); // GTM
    oneMesh = *tmpMesh;
    break;
  }

  return oneMesh;
}


bool Object2::Texture_Loader(const char *filePath, Mesh *mesh )
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
    // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    mesh->textures.push_back(textureID);
  }
  catch( Magick::Error& error )
  {
    printf("Error loading image '%s': %s\n", filePath, error.what());
    return false;
  }

  return true;
}
