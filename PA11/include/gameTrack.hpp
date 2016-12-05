#ifndef GAME_TRACK
#define GAME_TRACK

#include "graphics_headers.hpp"
#include "physicsObject.hpp"
#include "physicsWorld.hpp"

class GameTrack
{
  public:
    GameTrack(btTransform worldTrans, const char *lvlPath);
    ~GameTrack();
    bool Initialize();
    void Update();
    void Render();
    
    void addToWorld(PhysicsWorld *world);
    PhysicsObject* GetBase();
    PhysicsObject* GetObstacles();
    std::vector<PhysicsObject> GetObjects();

  private:
    enum Layer
    {
      BASE = 0,
      OBSTACLE = 1,
      OBJECT = 2
    };

    struct Tile
    {
      glm::vec2 start;
      glm::vec2 stop;
      short unsigned int terrainId;
      Layer layer;
      float hOffset;

      Tile(glm::vec2 strt, glm::vec2 stp, short unsigned int tId, Layer lyr, float h):
        start(strt), stop(stp), terrainId(tId), layer(lyr), hOffset(h) {}
    };

    const char *levelFile;
    btTransform worldTransform;
    PhysicsObject *trackBase;
    PhysicsObject *trackObstacles;
    std::vector<PhysicsObject> trackObjects;

    btCompoundShape *shapeBase;
    btCompoundShape *shapeObstacles;
    
    bool generateLevel(const char *filePath);
    Mesh* loadMesh(const char *filePath);
    Mesh* getTerrainMesh(short unsigned int terrainId);
};

#endif /* GAME_TRACK */
