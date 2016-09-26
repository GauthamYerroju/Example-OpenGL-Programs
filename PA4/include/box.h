#ifndef BOX_H
#define BOX_H

#include <string>
#include <fstream>
#include <sstream>

#include "object.h"


class Box: public Object
{
  public:
	Box(const char *objPath);
	~Box();

	void Update(unsigned int dt, EventFlag e_flags);
    void Set_RotateSpeed(float r_speed); 
    void Set_Scale( float sclr );
    bool Model_Loader(const char *filePath);


  private:
    glm::mat4 rotation;
    glm::mat4 scale;

    float angle_rotate;
    float rotate_speed;
    float scaler;
};


#endif /* BOX_H */