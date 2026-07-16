Leviathan Game Engine:

Description: 
A personally developed project Game Engine that can be easily interfaced through C++.
Following the steps from https://learnopengl.com/ but consolidated to make a useful tool.

# features:
 - OpenGL Rendering
 - custom models
 - custom textures
 - custom shaders
 - render effects
 - custom game object behaviors
 - keyboard, mouse input handling
 - 4 types of lights
 - conversion tool: Equirectangular image -> Cubemap faces

# Requirements:
 - cmake 3.5+
 - OpenGL 4.6
 - assimp

# Build
 > bash build.sh

# Sample Program
The sample program features:
- Basic game initialization / workflow example
- Model loading with assimp
- Geometry generation
- Dynamic lighting 
- Different lighting types 
- Rendering effects
- Keyboard / Mouse input (press "o" and "k")
- Customized / modular game object behavior 

(run sample program)
 > bash run.sh sample

# Create your own game
 - Write program as FILENAME.cpp in /games
 > bash build.sh
 > bash run.sh FILENAME

(to remake)
 - Will recompile all games
 > bash make.sh

 - Will just recompile
 > bash remake.sh

 - Will recompile and run (recommended)
 > bash run.sh FILENAME



