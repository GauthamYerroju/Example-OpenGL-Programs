# PA7: Solar System

Collaborators:

- Kevin Green
- Husain Tazarvi
- Gautham Yerroju

# Index
<!-- TOC updateOnSave:false -->

- [1. Introduction](#1-introduction)
    - [1.1 Extra Credit and Graduate Requirements](#11-extra-credit-and-graduate-requirements)
        - [Graduate Requirements](#graduate-requirements)
        - [Extra Credit](#extra-credit)
- [2. Dependencies, Building, and Running](#2-dependencies-building-and-running)
    - [2.1 Dependency Instructions](#21-dependency-instructions)
        - [Ubuntu/Linux](#ubuntulinux)
        - [Mac OSX](#mac-osx)
    - [2.2 Building and Running](#22-building-and-running)
        - [CMake Instructions](#cmake-instructions)
    - [2.3 Ubuntu.cse.unr.edu](#23-ubuntucseunredu)
- [3. Implementation Details](#3-implementation-details)
    - [3.1 Orbit Calculation](#31-orbit-calculation)
    - [3.2 Dealing with Scale](#32-dealing-with-scale)
- [4. User Interaction](#4-user-interaction)
    - [4.1 Keyboard](#41-keyboard)
    - [4.2 Mouse](#42-mouse)
- [5. Known Issues](#5-known-issues)

<!-- /TOC -->

# 1. Introduction

This program simulates the solar system. The solar objects are:
- Sun
- Mercury
- Venus
- Earth
- Moon
- Mars
- Jupiter (with rings)
- Saturn (with rings)
- Uranus (with rings)
- Neptune (with rings)
- Pluto

## 1.1 Extra Credit and Graduate Requirements

In addition to the basic requirements (and graduate requirements), some additional features are implemented for extra credit:

### Graduate Requirements
- Live adjustment of speed
- Configuration file

### Extra Credit
- Rings on planets
- [*] Option to go from actual data to scaled view

# 2. Dependencies, Building, and Running

## 2.1 Dependency Instructions
For both of the operating systems to run this project installation of these four programs are required [GLEW](http://glew.sourceforge.net/), [GLM](http://glm.g-truc.net/0.9.7/index.html), [SDL2](https://wiki.libsdl.org/Tutorials), [ASSIMP](http://www.assimp.org/lib_html/index.html), [Magick++](http://www.imagemagick.org/Magick++/).

This project uses OpenGL 3.3. Some computers, such as virtual machines in the ECC, can not run this version. In in order to run OpenGL 2.7 follow the instructions at [Using OpenGL 2.7](https://github.com/HPC-Vis/computer-graphics/wiki/Using-OpenGL-2.7)

### Ubuntu/Linux
```bash
sudo apt-get install libglew-dev libglm-dev libsdl2-dev libassimp-dev libmagick++-dev
```

### Mac OSX
Installation of brew is suggested to easily install the libs. Ensure that the latest version of the Developer Tools is installed.
```bash
brew install glew glm sdl2 assimp imagemagick
```
Note: This project has not been tested on Max OSX and cannot be guaranteed to work.

## 2.2 Building and Running
To build this project use CMake. Shader file paths and the config file path must be sent as arguments to the program using the following format:

exe-filename vert-shader-filepath frag-shader-filepath config-filepath

### CMake Instructions
The building of the project is done using CMake, installation with apt-get or brew may be necessary.

```bash
mkdir build
cd build
cmake ..
make
./PA7 shaders/vShader.vert shaders/fShader.frag ../config.json
```

## 2.3 Ubuntu.cse.unr.edu
OpenGL 3.3 will run on the [ubuntu.cse.unr.edu](https://ubuntu.cse.unr.edu/) website. To do so follow the build instructions, but when running the executable use this line to execute.
```bash
/usr/NX/scripts/vgl/vglrun ./PA7 shaders/vShader.vert shaders/fShader.frag models/sun.obj
```

# 3. Implementation Details

## 3.1 Orbit Calculation

The orbit positions are calculated using NASA's [SPICE](https://naif.jpl.nasa.gov/naif/toolkit.html) library to provide accurate solar object positions with respect to an observer object at a specific period in time. This allows for the planets to be positioned on their respective planes and have elliptical orbits. The SPICE kernel file being used to get calculation is [de421.dsp](http://naif.jpl.nasa.gov/pub/naif/generic_kernels/spk/planets/a_old_versions/de421.cmt). The distances are then scaled down by a constant factor to get a better view of the solar system.

## 3.2 Dealing with Scale

The sizes of solar objects are scaled using 1 unit as the radius of the earth (approx 6378.1 KM), giving a ratio of ```planet_radius/earth_radius```

The solar object rotations are scaled using 1 rotation of the earth about its axis per day. Thus the other solar objects are calculated using ```1/days_for_one_rotation```

The ratios, solar object names and their respective parent object which they orbit, and object files are set in a json config file in the PA7 directory called ```config.json```

# 4. User Interaction

## 4.1 Keyboard

The following keyboard commands are used to interact with the program:

|Key|Event|
|---|-----|
|w|Move into screen|
|s|Move out of screen|
|a|Move left|
|d|Move right|
|0|Zoom on Mercury|
|1|Zoom on Venus|
|2|Zoom on Earth|
|3|Zoom on Moon|
|4|Zoom on Mars|
|5|Zoom on Jupiter|
|6|Zoom on Saturn|
|7|Zoom on Uranus|
|8|Zoom on Neptune|
|9|Zoom on Pluto|
|Backspace|Reset camera to origin|
|"-"|Decrease simulation speed|
|"="|Increase simulation speed|
|Pause|Pause or resume program|
|Esc|Close program|

## 4.2 Mouse

The following mouse commands are used to interact with the program:

|Mouse event|Action|
|---|-----|
|Move Mouse|Look around (up -> lookup, down -> lookdown, right -> lookleft, right -> lookright)|
|Mouse Wheel|Zoom in (scroll up)/out(sroll down)|

# 5. Known Issues

- Changing the speed of teh simulation makes the planets jump a little. This happens because by changing speed, we query the SPICE library for positions at a future or past time, so the shift is not continuous.