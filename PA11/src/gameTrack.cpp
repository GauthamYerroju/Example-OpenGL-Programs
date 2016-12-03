#include "gameTrack.hpp"

GameTrack::GameTrack(const char *lvlPath) : PhysicsObject()
{
	levelFile = lvlPath;
}

GameTrack::~GameTrack()
{
	// delete terrainMeshes;
}

bool GameTrack::generateLevel(const char *filePath)
{
	// Level data
	// Row: Lane
	// Column: length and terrain type of tile
	// 			10s place: length
	//			 1s place: terrain ID
	//		TODO: Improve specification(into array) for more than 10 terrain types
	// -1: end of data, like \0 in strings
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

	btCollisionShape *cubeShape = new btBoxShape(btVector3(cubeLength/2, cubeLength/2, cubeLength/2));

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
			if (terrain > 0) {
				Mesh segment = getTerrainMesh(terrain);
				// Transform the vertices of the mesh
				for(auto & vert : segment.Vertices)
				{
					// Scale along z
					vert.vertex.z = vert.vertex.z * scale;
					// Offset z by half of cube length (bring it to start line)
					vert.vertex.z -= lastLength[lvi] + (cubeLength * scale / 2.0f) - 1.0f;
					// Offset x by lane number
					vert.vertex.x += ( (float)lvi - 3.0f ) * (cubeLength);
					// Offset y by constant amount
					// vert.vertex.y += 0.0f;
				}
				meshes.push_back(segment);
				printf("Mesh added for [%d, %d]\n", lvi, lvj);

				// Create corresponding collision shape
				btTransform localTransform = btTransform( btQuaternion(0, 0, 0, 1), btVector3(
					( (float)lvi - 3.0f ) * (cubeLength),
					0.0,
					lastLength[lvi] + (cubeLength * scale / 2.0f) - 1.0f
				));
				btCollisionShape *block = new btBoxShape( btVector3(cubeLength/2, cubeLength/2, cubeLength/2 * scale) );
				
				collisionShape->addChildShape( localTransform, block );
			}
			// Update last length = last length + scaled cube length of current block
			lastLength[lvi] += cubeLength * scale;
		}
	}
	return true;
}

Mesh GameTrack::getTerrainMesh(int terrainId)
{
	// Load the mesh for selected terrain if it isn't already
	if ( terrainMeshes[terrainId] == NULL )
	{
		const char *terrainModel;
		switch (terrainId) {
			case 1:
				terrainModel = "models/terrainCube.obj";
				break;
			default:
				terrainModel = "models/terrainCube.obj";
				break;
		}
		terrainMeshes[terrainId] = loadMesh(terrainModel);
	}

	// Create a copy of the base mesh and return it
	Mesh segment = new Mesh( terrainMeshes[terrainId] );
	return segment;
}

Mesh* GameTrack::loadMesh(const char *filePath)
{
  Assimp::Importer importer;

  const aiScene *scene = importer.ReadFile( filePath, aiProcessPreset_TargetRealtime_Fast | aiProcess_Triangulate );
  if( !scene )
  {
    printf("Error opening '%s': '%s'\n", filePath, importer.GetErrorString());
    return NULL;
  }

  // Read in vertices and face indices for each mesh
  // ... and break the loop after first mesh
  // (since we know these are terrain meshes and only have one mesh)
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
	  {
        printf("%s: Mesh does not have vertices/n", mesh->mName.C_Str());
	  }

      // Normals
      if(mesh->HasNormals())
      {
        const aiVector3D v_normal = mesh->mNormals[vert_indx];
        vert.normal = glm::vec3(v_normal.x, v_normal.y, v_normal.z);
      }
      else
	  {
        printf("%s: Mesh does not have normals/n", mesh->mName.C_Str());
	  }

      // UV coordinates
      if(mesh->HasTextureCoords(0)){
        const aiVector3D tex_uv = mesh->mTextureCoords[0][vert_indx];
        vert.uv_Coords = glm::vec2( tex_uv.x, tex_uv.y );
      }
      else
	  {
        printf("%s: Mesh does not have texture coordinates/n", mesh->mName.C_Str());
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

    return tmpMesh;
  }

  return NULL;
}

bool GameTrack::Initialize( btTransform worldTrans )
{

	collisionShape = new btCompoundShape();

	if ( !generateLevel( levelFile ) )
	{
		printf("Could not generate level\n");
	}

	// btTransform: rigid transforms with only translation and rotation
	// Rotation: btQuaternion(x, y, z, w), Translation: btVector3(x, y, z)
	motionState = new btDefaultMotionState( worldTrans );
	if( !motionState){
		printf("motionState failed to init\n");
		return false;
	}

	mass = 0;
	inertia = btVector3(0, 0, 0);
	btScalar restitution = 0.8;
	btScalar friction = 1.5;

	collisionShape->calculateLocalInertia( mass, inertia );
	btRigidBody::btRigidBodyConstructionInfo constructionInfo( mass, motionState, collisionShape, inertia );

	// Ratio of relative speed after to the realtive speed before the collision
	constructionInfo.m_restitution = restitution;
	constructionInfo.m_friction = friction;

	rigidBody = new btRigidBody( constructionInfo );
	if( !rigidBody ){
		printf("rigidBody failed to init\n");
		return false;
	}

	return true;
}
