#include "gameTrack.hpp"

GameTrack::GameTrack(btTransform worldTrans)
{
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

bool GameTrack::Initialize(std::string levelName, std::string skybox, float gravMod, std::vector<Tile> tiles)
{
	name = levelName;
	skyboxFilename = skybox;
	gravityModifier = gravMod;
	return _initialize(tiles);
}
bool GameTrack::InitializeFromJson(json levelDesc)
{
	name = levelDesc["name"];
	skyboxFilename = levelDesc["skybox"];
	gravityModifier = levelDesc["gravity"];

	std::vector<Tile> tiles = loadTilesFromJson(levelDesc["tiles"]);

	return _initialize( tiles );
}

bool GameTrack::_initialize(std::vector<Tile> tiles)
{
	if ( !generateLevel(tiles) )
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

bool GameTrack::generateLevel(std::vector<Tile> tiles)
{
	// Level gen loop
	glm::vec3 tileSize = glm::vec3(9, 1, 9);
	glm::vec3 obstacleSize = glm::vec3(9, 4, 9);
	glm::vec3 tunnelSize = glm::vec3(9, 9, 9);
	glm::vec3 objectSize = glm::vec3(9, 9, 9);

	for(auto& tile : tiles)
	{
		glm::vec3 layerSize;
		if (tile.layer == BASE) {
			layerSize = tileSize;
		}
		else if (tile.layer == OBSTACLE) {
			layerSize = obstacleSize;
		}
		else if (tile.layer == OBJECT) {
			layerSize = objectSize;
		}
		else {
			layerSize = tileSize;
		}

		if (tile.terrainId == 2)
		{
			layerSize = tunnelSize;
		}

		// Get the scale along the x and z axes
		float scaleX = tile.stop.x - tile.start.x + 1;
		float scaleZ = tile.stop.y - tile.start.y + 1;

		// Calculate the scale and position modifiers for current tile

		glm::vec3 modScale;
		glm::vec3 modPosition;
		// First, scale unit cube to tile size
		modScale.x = layerSize.x * scaleX;
		modScale.y = layerSize.y;
		modScale.z = layerSize.z * scaleZ;
		// Calculate position offsets
		modPosition.x = layerSize.x * (scaleX * 0.5 + tile.start.x - 3.5);
		modPosition.z = -( layerSize.z * (scaleZ * 0.5 + tile.start.y) ); // start is a vec2, z is stored in vec2.y
		switch(tile.layer)
		{
			case BASE:
				modPosition.y = tileSize.y * (tile.hOffset - 0.5);
				break;
			default:
				modPosition.y = layerSize.y * (tile.hOffset + 0.5);
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
			btQuaternion(0, 0, 0, 1), // No rotation
			btVector3(modPosition.x, modPosition.y, modPosition.z)
		);

		if (tile.layer == BASE) {
			trackBase->addMesh(segment);
			btCollisionShape *block = new btBoxShape( btVector3(modScale.x / 2, modScale.y / 2, modScale.z / 2) );
			shapeBase->addChildShape( localTransform, block );
		}
		else if (tile.layer == OBSTACLE) {
			trackObstacles->addMesh(segment);
			if (tile.terrainId == 2)
			{
				btTransform lt_left = btTransform( btQuaternion(0, 0, 0, 1), btVector3(modPosition.x - modScale.x/2, modPosition.y, modPosition.z));
				btTransform lt_right = btTransform( btQuaternion(0, 0, 0, 1), btVector3(modPosition.x + modScale.x/2, modPosition.y, modPosition.z));
				btTransform lt_top = btTransform( btQuaternion(0, 0, 0, 1), btVector3(modPosition.x, modPosition.y + modScale.y/2, modPosition.z));
				btCollisionShape *wall_sides = new btBoxShape( btVector3(0.1 * modScale.x / 2, modScale.y / 2, modScale.z / 2) );
				btCollisionShape *wall_top = new btBoxShape( btVector3(0.1 * modScale.x / 2, modScale.y / 2, modScale.z / 2) );

				shapeObstacles->addChildShape( lt_left, wall_sides );
				shapeObstacles->addChildShape( lt_right, wall_sides );
				shapeObstacles->addChildShape( lt_top, wall_top );
			}
			else {
				btCollisionShape *block = new btBoxShape( btVector3(modScale.x / 2, modScale.y / 2, modScale.z / 2) );
				shapeObstacles->addChildShape( localTransform, block );
			}
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
		case 2:
			terrainModel = "models/tunnel.obj";
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
std::string GameTrack::getName()
{
	return name;
}
std::string GameTrack::getSkyBoxFilename()
{
	return skyboxFilename;
}
float GameTrack::getGravityModifier()
{
	return gravityModifier;
}

std::vector<Tile> GameTrack::loadTilesFromJson(json tileList)
{
	std::vector<Tile> tiles;
	for(auto& tile : tileList)
	{
		float x, y;

		x = tile[0][0];
		y = tile[0][1];
		glm::vec2 start(x, y);

		x = tile[1][0];
		y = tile[1][1];
		glm::vec2 stop(x, y);

		short unsigned int terrainId = tile[2];

		int lyr = tile[3];
		Layer layer = static_cast<Layer>(lyr);

		float hOffset = tile[4];

		tiles.push_back( Tile(start, stop, terrainId, layer, hOffset) );
	}
	return tiles;
}