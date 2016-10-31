# PA8: Bullet
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
To build this project use CMake. Shader file paths and the config file path must be sent as arguments to the program using the following format:  

exe-filename vert-shader-filepath frag-shader-filepath config-filepath  

### CMake Instructions
The building of the project is done using CMake, installation with apt-get or brew may be necessary.

```bash
mkdir build
cd build
cmake ..
make
./PA8 shaders/vShader.vert shaders/fShader.frag ../config.json
```

## Keyboard Events
The following keyboard commands are used to interact with the program:

Key          - Event  

w            - Move into screen  
s            - Move out of screen  
a            - Move left  
d            - Move right  
0            - Zoom on Mercury  
1            - Zoom on Venus  
2            - Zoom on Earth  
3            - Zoom on Moon  
4            - Zoom on Mars  
5            - Zoom on Jupiter  
6            - Zoom on Saturn  
7            - Zoom on Uranus  
8            - Zoom on Neptune  
9            - Zoom on Pluto  
Backspace    - Reset camera to origin  
"-"          - Decrees Speed  
"="          - Increase Speed  
Pause        - Pause or resume program  
Esc          - Close program  

## Mouse Events
The following mouse commands are used to interact with the program:  

Move Mouse    - Look around (up->lookup, down->lookdown, right->lookleft, right->lookright )  
MouseWheel    - Zoom in(scroll up)/out(sroll down)  


## Ubuntu.cse.unr.edu
OpenGL 3.3 will run on the [ubuntu.cse.unr.edu](https://ubuntu.cse.unr.edu/) website. To do so follow the build instructions, but when running the executable use this line to execute.
```bash
/usr/NX/scripts/vgl/vglrun ./PA8 shaders/vShader.vert shaders/fShader.frag config-filepath
```
