#include "gameTrack.hpp"

GameTrack::GameTrack(btTransform worldTrans, const char *lvlPath)
{
	levelFile = lvlPath;
	worldTransform = worldTrans;
	trackBase = new PhysicsObject();
	trackObstacles = new PhysicsObject();
	shapeBase = new btCompoundShape();
	shapeObstacles = new btCompoundShape();
}

GameTrack::~GameTrack()
{
	delete trackBase;
	delete trackObstacles;
	delete shapeBase;
	delete shapeObstacles;
}

bool GameTrack::Initialize()
{
	if ( !generateLevel( levelFile ) )
	{
		printf("Could not generate level\n");
		return false;
	}
	// Initialize meshes in objects
	trackBase->InitializeMeshes();
	trackObstacles->InitializeMeshes();
	for(auto & obj : trackObjects)
	{
		obj.InitializeMeshes();
	}
	// Initialize Physics shapes
	trackBase->InitializeWithCompoundShape(shapeBase, worldTransform, 0, 0.8f, 1.5f);
	trackObstacles->InitializeWithCompoundShape(shapeObstacles, worldTransform, 0, 0.8f, 1.5f);

	return true;
}

bool GameTrack::generateLevel(const char *filePath)
{
	// Level data
	Tile tiles[7] = {
		{ glm::vec2(0, 0), glm::vec2(0, 7), 1, BASE, 0 },
		{ glm::vec2(1, 2), glm::vec2(1, 3), 1, BASE, 0 },
		{ glm::vec2(1, 5), glm::vec2(1, 6), 1, BASE, 0 },
		{ glm::vec2(2, 0), glm::vec2(4, 7), 1, BASE, 0 },
		{ glm::vec2(5, 2), glm::vec2(5, 3), 1, BASE, 0 },
		{ glm::vec2(5, 5), glm::vec2(5, 6), 1, BASE, 0 },
		{ glm::vec2(6, 0), glm::vec2(6, 7), 1, BASE, 0 }
	};

	// Level gen loop
	glm::vec3 tileSize = glm::vec3(9, 3, 9);
	glm::vec3 obstacleSize = glm::vec3(9, 9, 9);
	glm::vec3 objectSize = glm::vec3(9, 9, 9);

	for(unsigned int tileId = 0; tileId < 7; tileId++)
	{
		Tile tile = tiles[tileId];
		glm::vec3 layerSize;
		if			(tile.layer == BASE)			layerSize = tileSize;
		else if (tile.layer == OBSTACLE)	layerSize = obstacleSize;
		else if (tile.layer == OBJECT)		layerSize = objectSize;
		else															layerSize = tileSize;
		printf("Processing tile %d...\n", tileId);
		// Get the scale along the x and z axes
		float scaleX = tile.stop.x - tile.start.x + 1;
		float scaleZ = tile.stop.y - tile.start.y + 1;

		// Calculate the scale and position modifiers for current tile

		glm::vec3 modScale(1.0f, 1.0f, 1.0f);
		glm::vec3 modPosition(0.0f, 0.0f, 0.0f);
		// First, scale unit cube to tile size
		modScale *= layerSize;
		// Scale along xz axis (the ground plane)
		modScale.x *= scaleX;
		modScale.z *= scaleZ;
		// Calculate position offsets
		modPosition.x += (tile.start.x - 3.0) * layerSize.x;
		modPosition.z -= layerSize.z * tile.start.y; // start is a vec2, z is stored in vec2.y
		switch(tile.layer)
		{
			case BASE:
				modPosition.y += tile.hOffset - (tileSize.y / 2);
				break;
			case OBSTACLE:
				modPosition.y += tile.hOffset - (tileSize.y / 2) + (obstacleSize.y / 2);
				break;
			case OBJECT:
				// TODO: Position depends on object dimensions
				modPosition.y += tile.hOffset - (tileSize.y / 2) + (obstacleSize.y / 2);
				break;
			default:
				modPosition.y += tile.hOffset - (tileSize.y / 2) + (obstacleSize.y / 2);
				break;
		}

		// Create a copy of the terrain's template mesh
		Mesh *segment = getTerrainMesh(tile.terrainId);
		// Apply scale and position modifiers to vertices
		for(auto & vert : segment->Vertices)
		{
			vert.vertex *= modScale;
			vert.vertex += modPosition;
		}

		// Create corresponding collision shape
		btTransform localTransform = btTransform(
			btQuaternion(0, 0, 0, 0), // No rotation
			btVector3(modPosition.x, modPosition.y, -modPosition.z) // Invert z
		);

		if (tile.layer == BASE) {
			trackBase->addMesh(segment);
			btCollisionShape *block = new btBoxShape( btVector3(modScale.x / 2, modScale.y / 2, modScale.z / 2) );
			shapeBase->addChildShape( localTransform, block );
		}
		else if (tile.layer == OBSTACLE) {
			trackObstacles->addMesh(segment);
			// Mesh shape
			btCollisionShape *block = new btBoxShape( btVector3(modScale.x / 2, modScale.y / 2, modScale.z / 2) );
			shapeObstacles->addChildShape( localTransform, block );
		}
		else if (tile.layer == OBJECT)
		{
			PhysicsObject *obj = new PhysicsObject();
			obj->addMesh(segment);
			obj->Init_Box(localTransform, 0, 0.0, 0.0,
				btVector3(modScale.x / 2, modScale.y / 2, modScale.z / 2)
			);
			trackObjects.push_back(*obj);
		}
	}

	return true;
}

Mesh* GameTrack::getTerrainMesh(short unsigned int terrainId)
{
	// Load the mesh for selected terrain if it isn't already
	const char *terrainModel;
	switch (terrainId) {
		case 1:
			terrainModel = "models/terrainCube.obj";
			break;
		default:
			terrainModel = "models/terrainCube.obj";
			break;
	}
	Mesh *terrainMesh = loadMesh(terrainModel);

	return terrainMesh;
}

Mesh* GameTrack::loadMesh(const char *filePath)
{
  Assimp::Importer importer;
	// Temporary Object for using Texture_Loader
	Object *tmpObj = new Object();

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
        printf("%s: Mesh does not have vertices\n", mesh->mName.C_Str());
	  }

      // Normals
      if(mesh->HasNormals())
      {
        const aiVector3D v_normal = mesh->mNormals[vert_indx];
        vert.normal = glm::vec3(v_normal.x, v_normal.y, v_normal.z);
      }
      else
	  {
        printf("%s: Mesh does not have normals\n", mesh->mName.C_Str());
	  }

      // UV coordinates
      if(mesh->HasTextureCoords(0)){
        const aiVector3D tex_uv = mesh->mTextureCoords[0][vert_indx];
        vert.uv_Coords = glm::vec2( tex_uv.x, tex_uv.y );
      }
      else
	  {
        printf("%s: Mesh does not have texture coordinates\n", mesh->mName.C_Str());
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
        if ( !tmpObj->Texture_Loader(tPath.c_str(), tmpMesh ) )
        {
          printf("Failed to load diffuse texture #%d for '%s'\n", mtl->GetTextureCount(aiTextureType_DIFFUSE), mesh->mName.C_Str() );
        }
      }
    }
		delete tmpObj;
    return tmpMesh;
  }

	delete tmpObj;
  return NULL;
}

void GameTrack::Update()
{
	trackBase->Update();
	trackObstacles->Update();
	for( auto & obj : trackObjects )
	{
		obj.Update();
	}
}

void GameTrack::addToWorld(PhysicsWorld *world)
{
	world->AddRigidBody(trackBase->GetRigidBody());
	world->AddRigidBody(trackObstacles->GetRigidBody());
	for(auto & obj : trackObjects)
	{
		world->AddRigidBody(obj.GetRigidBody());
	}
}

PhysicsObject* GameTrack::GetBase()
{
	return trackBase;
}
PhysicsObject* GameTrack::GetObstacles()
{
	return trackObstacles;
}
std::vector<PhysicsObject> GameTrack::GetObjects()
{
	return trackObjects;
}