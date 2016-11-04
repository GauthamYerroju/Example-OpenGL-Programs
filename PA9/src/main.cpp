#include <iostream>

#include "engine.hpp"


int main(int argc, char **argv)
{

  /******************************************/

  /* Check command line arguments */
  if( argc < 2 ){
    printf( "ERROR: Incorrect command line arguments (MISSING ARGUMENT)\n" );
    printf("\n Usage : <exe-filename> <config-filepath>\n");
    printf("\n Example : ./PA9 ../config.json\n");
    return 1;
  }
  if( argc > 2 ){
    printf( "ERROR: Incorrect command line arguments (TOO MANY ARGUMENTS)\n" );
    printf("\n Usage : <exe-filename> <config-filepath>\n");
    printf("\n Example : ./PA9 ../config.json\n");
    return 1;
  }

  /******************************************/

  // Start an engine and run it then cleanup after
  Engine *engine = new Engine("Bullet Physics", 1024, 768);
  // Engine *engine = new Engine("Bullet Physics");
  if(!engine->Initialize(argv[1]))
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
