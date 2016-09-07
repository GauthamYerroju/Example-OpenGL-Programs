#include <iostream>

#include "engine.h"


int main(int argc, char **argv)
{

  /******************************************/

  /* Check command line arguments */
  if( argc < 3 ){
    printf( "ERROR: Incorrect command line arguments (MISSING ARGUMENT)\n" );
    printf("\n Usage : <executable-filename> <vertex-shader-filepath> <fragment-shader-filepath>\n");
    printf("\n Example : ./Tutorial ../files/vShader.vert ../files/fShader.frag\n");
    return 1;
  }
  if( argc > 3 ){
    printf( "ERROR: Incorrect command line arguments (TOO MANY ARGUMENTS)\n" );
    printf("\n Usage : <executable-filename> <vertex-shader-filepath> <fragment-shader-filepath>\n");
    printf("\n Example : ./Tutorial shaders/vShader.vert shaders/fShader.frag\n");
    return 1;
  }

  /******************************************/

  // Start an engine and run it then cleanup after
  Engine *engine = new Engine("Tutorial Window Name", 800, 600);
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
