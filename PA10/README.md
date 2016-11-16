# PA10: PinBall
Collaborators:

Kevin Green,
Husain Tazarvi,
Gautham Yerroju

# Dependencies, Building, and Running

## Dependency Instructions
For both of the operating systems to run this project installation of these programs are required [GLEW](http://glew.sourceforge.net/), [GLM](http://glm.g-truc.net/0.9.7/index.html), [SDL2](https://wiki.libsdl.org/Tutorials), [ASSIMP](http://www.assimp.org/lib_html/index.html), [Magick++](http://www.imagemagick.org/Magick++/), and [BULLET](http://bulletphysics.org/wordpress/).

This project uses OpenGL 3.3. Some computers, such as virtual machines in the ECC, can not run this version. In in order to run OpenGL 2.7 follow the instructions at [Using OpenGL 2.7](https://github.com/HPC-Vis/computer-graphics/wiki/Using-OpenGL-2.7)

### Ubuntu/Linux
```bash
sudo apt-get install libglew-dev libglm-dev libsdl2-dev libassimp-dev libmagick++-dev libbullet-dev
```

### Mac OSX
Installation of brew is suggested to easily install the libs. Ensure that the latest version of the Developer Tools is installed.
```bash
brew install glew glm sdl2 assimp imagemagick bullet
```
Note: This project has not been tested on Max OSX and cannot be guaranteed to work.

## Building and Running
To build this project use CMake. Shader file paths are defined in config and the config file path must be sent as arguments to the program using the following format:

exe-filename config-filepath

### CMake Instructions
The building of the project is done using CMake, installation with apt-get or brew may be necessary.

```bash
mkdir build
cd build
cmake ..
make
./PA10 ../config.json
```

## Keyboard Events
The following keyboard commands are used to interact with the program:

|Key|Event|
|---|---|
|Esc|Close program|
|__Lighting Controls__|
|1|Change to per-fregment lighting|
|2|Change to per-vertex lighting|
|A|Select ambient scalar|
|D|Select diffuse scalar|
|S|Select specular scalar|
|C|Select spotlight angle|
|=|Increase currently selected scalar|
|-|Decrease currently selected scalar|
|__Pinball Controls__|
|Down|Activate plunger (hold for more power)|
|Left|Left flipper|
|Right|Right flipper|
|__Camera__|
|F10|Zoom in/zoom out|


## Mouse Events
The following mouse commands are used to interact with the program:

Left Mouse Click    - move paddle up


## Ubuntu.cse.unr.edu
OpenGL 3.3 will run on the [ubuntu.cse.unr.edu](https://ubuntu.cse.unr.edu/) website. To do so follow the build instructions, but when running the executable use this line to execute.
```bash
/usr/NX/scripts/vgl/vglrun ./PA10 ../config.json
```
