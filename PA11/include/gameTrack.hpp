#ifndef GAME_TRACK
#define GAME_TRACK

#include "graphics_headers.hpp"
#include "physicsObject.hpp"

class GameTrack
{
  public:
    GameTrack(const char *lvlPath);
    ~GameTrack();
    void Update();
    bool collideCheckObstacle(PhysicsObject *ship);
    PhysicsObject* collideCheckObject(PhysicsObject *ship);

  private:
    enum Layer
    {
      BASE = 0,
      OBSTACLE = 1,
      OBJECT = 2
    }
    struct Tile
    {
      glm::vec2 start,
      glm::vec2 stop,
      short unsigned int terrainId,
      Layer layer,
      float hOffset

      Tile(glm::vec2 strt, glm::vec2 stp, short unsigned int tId, Layer lyr, float h):
        start(strt), stop(stp), terrainId(tId), layer(lyr), hOffset(h) {}
    }

    const char *levelFile;
    // std::vector<Mesh> terrainMeshes; // Can't use weithoyut mesh duplication
    PhysicsObject *trackBase;
    PhysicsObject *trackObstacles;
    std::vector<PhysicsObject> trackObjects;
    
    bool Initialize();
    bool generateLevel(const char *filePath);
    Mesh* loadMesh(const char *filePath);
    Mesh* getTerrainMesh(int terrainId);
};

#endif /* GAME_TRACK */
