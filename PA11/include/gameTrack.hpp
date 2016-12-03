#ifndef GAME_TRACK
#define GAME_TRACK

#include "graphics_headers.hpp"
#include "physicsObject.hpp"

class GameTrack : public PhysicsObject
{
  public:
    // GameTrack();
    GameTrack(const char *lvlPath);
    // ~GameTrack();

    bool Initialize( btTransform worldTrans );

  private:
    const char *levelFile;
    Mesh* terrainMeshes[7] = {NULL};
    btCompoundShape *collisionShape;
    
    bool generateLevel(const char *filePath);
    Mesh* loadMesh(const char *filePath);
    Mesh* getTerrainMesh(int terrainId);
};

#endif /* GAME_TRACK */
