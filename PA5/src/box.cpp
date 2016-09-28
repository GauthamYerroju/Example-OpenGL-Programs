#include "box.h"

Box::Box(const char *objPath)
{  
  
  if(!Model_Loader(objPath)){
    std::cout << "FAILED TO LOAD OBJECT" << std::endl;
  }

  /*
  // The index works at a 0th index
  for(unsigned int i = 0; i < Indices.size(); i++)
  {
    Indices[i] = Indices[i] - 1;
  }
  */
  
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


bool Box::Model_Loader(const char *filePath)
{
  // Create an instance of the Importer class
  Assimp::Importer importer;


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

  for( unsigned int mesh_indx = 0; mesh_indx < scene->mNumMeshes; mesh_indx++){
    const aiMesh *mesh = scene->mMeshes[mesh_indx];
    const aiMaterial *mtl = scene->mMaterials[mesh->mMaterialIndex];
    
    aiColor3D color (0.f,0.f,0.f);
    mtl->Get(AI_MATKEY_COLOR_DIFFUSE,color);
    
    std::cout << "MESH #" << (mesh_indx + 1) << std::endl;
    std::cout << "MESH NAME: " << mesh->mName.C_Str() << std::endl << std::endl;
    
    std::cout << "# of primitives (faces:triangles, polygons, lines) in this mesh: " << mesh->mNumFaces << std::endl;
    std::cout << "# of vertices in this mesh: " << mesh->mNumVertices << std::endl;
    std::cout << "The material used by this mesh (MaterialIndex): " << mesh->mMaterialIndex << std::endl;
    std::cout << "Diffuse Color: " << glm::to_string(glm::vec3( color.r, color.g, color.b)) << std::endl << std::endl;

    for( unsigned int vert_indx = 0; vert_indx < mesh->mNumVertices; vert_indx++){
      const aiVector3D v_position = mesh->mVertices[vert_indx];
             
      Vertex vert(glm::vec3(0.0f), glm::vec3(0.0f));
      vert.vertex = glm::vec3(v_position.x, v_position.y, v_position.z);
      vert.color = glm::vec3( color.r, color.g, color.b);

      Vertices.push_back(vert);

      std::cout << "Vertex: " << glm::to_string(glm::vec3(v_position.x, v_position.y, v_position.z)) << std::endl;
    }

    std::cout << std::endl;

    for( unsigned int face_indx = 0; face_indx < mesh->mNumFaces; face_indx++){
      const aiFace face = mesh->mFaces[face_indx];

      for( unsigned int indx = 0; indx < face.mNumIndices; indx++){
        std::cout << face.mIndices[indx] << " ";

        Indices.push_back(face.mIndices[indx]);
      }

      std::cout << std::endl;  
    }

    std::cout << std::endl;
  }
 
  return true;
}