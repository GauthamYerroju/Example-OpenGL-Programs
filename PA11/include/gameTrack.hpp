#ifndef GAME_TRACK
#define GAME_TRACK

#include "graphics_headers.hpp"
#include "physicsObject.hpp"
#include "physicsWorld.hpp"

#include "json.hpp"
using json = nlohmann::json;

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

class GameTrack
{
  public:
    GameTrack(btTransform worldTrans);
    ~GameTrack();
    bool Initialize(std::string levelName, std::string skybox, float gravMod, std::vector<Tile> tiles);
    // bool InitializeFromJson(std::string levelJson);
    bool InitializeFromJson(json levelDesc);
    void Update();
    void Render();
    
    void addToWorld(PhysicsWorld *world);
    PhysicsObject* GetBase();
    PhysicsObject* GetObstacles();
    std::vector<PhysicsObject> GetObjects();

    std::string getName();
    std::string getSkyBoxFilename();
    float getGravityModifier();

    std::vector<Tile> loadTilesFromJson(json tileListJson);

  private:
    

    btTransform worldTransform;
    PhysicsObject *trackBase;
    PhysicsObject *trackObstacles;
    std::vector<PhysicsObject> trackObjects;

    std::string name;
    std::string skyboxFilename;
    float gravityModifier;

    btCompoundShape *shapeBase;
    btCompoundShape *shapeObstacles;
    
    bool generateLevel(std::vector<Tile> tiles);
    Mesh* loadMesh(const char *filePath);
    Mesh* getTerrainMesh(short unsigned int terrainId);
    bool _initialize(std::vector<Tile> tiles);
};

#endif /* GAME_TRACK */
