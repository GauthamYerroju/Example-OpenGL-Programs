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

struct BoundingBox
{
  glm::vec3 position;
  glm::vec3 size;
  BoundingBox(glm::vec3 pos, glm::vec3 sz): position(pos), size(sz) {}
  bool collidesWith(BoundingBox other)
  {
    float left = position.x - size.x / 2;
    float right = position.x + size.x / 2;
    float top = position.y + size.y / 2;
    float bottom = position.y - size.y / 2;
    float front = position.z - size.z / 2;
    float back = position.z + size.z / 2;

    float other_left = other.position.x - other.size.x / 2;
    float other_right = other.position.x + other.size.x / 2;
    float other_top = other.position.y + other.size.y / 2;
    float other_bottom = other.position.y - other.size.y / 2;
    float other_front = other.position.z - other.size.z / 2;
    float other_back = other.position.z + other.size.z / 2;

    if (other_right < left)
      return false;
    if (other_left > right)
      return false;
    if (other_top < bottom)
      return false;
    if (other_bottom > top)
      return false;
    if (other_front > back)
      return false;
    if (other_back < front)
      return false;
    
    return true;
  }
};

class GameTrack
{
  public:
    GameTrack(btTransform worldTrans);
    ~GameTrack();
    bool Initialize(std::string levelName, std::string skybox, float gravMod, std::vector<Tile> tiles);
    bool InitializeFromJson(json levelDesc);
    std::vector<Tile> loadTilesFromJson(json tileListJson);
    void Update();
    void Render();
    
    void addToWorld(PhysicsWorld *world);
    PhysicsObject* GetBase();
    PhysicsObject* GetObstacles();
    std::vector<PhysicsObject> GetObjects();

    std::string getName();
    std::string getSkyBoxFilename();
    float getGravityModifier();

    bool inTunnel(BoundingBox shipBox);

  private:
    btTransform worldTransform;
    PhysicsObject *trackBase;
    PhysicsObject *trackObstacles;
    std::vector<PhysicsObject> trackObjects;

    std::vector<BoundingBox> tunnels;

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
