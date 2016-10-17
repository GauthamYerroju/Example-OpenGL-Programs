#include <iostream>

#include "engine.hpp"


int main(int argc, char **argv)
{

  /******************************************/

  /* Check command line arguments */
  if( argc < 4 ){
    printf( "ERROR: Incorrect command line arguments (MISSING ARGUMENT)\n" );
    printf("\n Usage : <exe-filename> <vert-shader-filepath> <frag-shader-filepath> <obj-filepath>\n");
    printf("\n Example : ./Tutorial ../files/vShader.vert ../files/fShader.frag ../models/box.obj\n");
    return 1;
  }
  if( argc > 4 ){
    printf( "ERROR: Incorrect command line arguments (TOO MANY ARGUMENTS)\n" );
    printf("\n Usage : <exe-filename> <vert-shader-filepath> <frag-shader-filepath> <obj-filepath>\n");
    printf("\n Example : ./Tutorial ../files/vShader.vert ../files/fShader.frag ../models/box.obj\n");
    return 1;
  }

  /******************************************/

  // Start an engine and run it then cleanup after
  //Engine *engine = new Engine("Solar System", 800, 600);
  Engine *engine = new Engine("Solar System");
  if(!engine->Initialize(argv))
  {
    printf("The engine failed to start.\n");
    delete engine;
    engine = NULL;
    return 1;
  }
  engine->Run();
  delete engine;
  engine = NULL;
  return 0;
}
