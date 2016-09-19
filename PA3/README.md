# PA3: Moons

# Dependencies, Building, and Running

## Dependency Instructions
For both of the operating systems to run this project installation of these three programs are required [GLEW](http://glew.sourceforge.net/), [GLM](http://glm.g-truc.net/0.9.7/index.html), and [SDL2](https://wiki.libsdl.org/Tutorials).

This project uses OpenGL 3.3. Some computers, such as virtual machines in the ECC, can not run this version. In in order to run OpenGL 2.7 follow the instructions at [Using OpenGL 2.7](https://github.com/HPC-Vis/computer-graphics/wiki/Using-OpenGL-2.7)

### Ubuntu/Linux
```bash
sudo apt-get install libglew-dev libglm-dev libsdl2-dev
```

### Mac OSX
Installation of brew is suggested to easily install the libs. Ensure that the latest version of the Developer Tools is installed.
```bash
brew install glew glm sdl2
```

## Building and Running
To build this project use CMake. Shader file paths must be sent as arguments to the program  using the following format:  

executable-filename vertex-shader-filepath fragment-shader-filepath  

### CMake Instructions
The building of the project is done using CMake, installation with apt-get or brew may be necessary.

```bash
mkdir build
cd build
cmake ..
make
./Tutorial shaders/vShader.vert shaders/fShader.frag
```

## Keyboard Events
The following keyboard commands are used to interact with the program:

Key          - Event  

a            - Change planet rotation to counter-clockwise  
d            - Change planet rotation to clockwise  
w            - Change planet orbit to counter-clockwise  
s            - Change planet orbit to clockwise  
LeftArrow    - Change moon rotation to counter-clockwise  
RightArrow   - Change moon rotation to clockwise  
UpArrow      - Change moon orbit to counter-clockwise  
DownArrow    - Change moon orbit to clockwise  
1            - Toggle (start/stop) planet rotation  
2            - Toggle (start/stop) moon rotation  
3            - Toggle (start/stop) planet orbit  
4            - Toggle (start/stop) moon orbit  
Pause        - Pause or resume program  
Esc          - Close program  

## Mouse Events
The following mouse commands are used to interact with the program:

Left-Click    - Reverse planet rotation direction  
Right-Click   - Reverse planet orbit direction  

## Ubuntu.cse.unr.edu
OpenGL 3.3 will run on the [ubuntu.cse.unr.edu](https://ubuntu.cse.unr.edu/) website. To do so follow the build instructions, but when running the Tutorial executable use this line to execute.
```bash
/usr/NX/scripts/vgl/vglrun ./Tutorial
```
