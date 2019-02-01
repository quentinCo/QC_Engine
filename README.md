# QC Engine 

QC Engine is a personal challenge which has for objective to develop a real time rendering engine, and try the latest rendering method.

## Objectives

### A generic library - in process

The first objective of this project is to develop a library which provides the basic elements to enable quick implementation of new rendering method.
- Window management
- Cameras
- OpenGL encapsulation
- Mesh generation
- 3d file importer
- ...

[//]: # (Currently implemented:) 
[//]: # (- Basic opengl objects) 
[//]: # (		- Buffer Objects) 
[//]: # (		- Vertex Array)
[//]: # (		- Shader)
[//]: # (		- Program)
[//]: # (- Window)
[//]: # (- Basic camera)
[//]: # (- Camera controlers - FPS)
[//]: # (- Transform)
	
### The engine - coming

The second objective is the realisation of the engine itself.
- Scene graph
- Multimaterial
- Rendering options with various optimisation (culling methods, tessellation, ...)
- ...

## Compilation instruction
Execute the cmake to generate the project.
- [x] Tested on windows (visual studio)
- [ ] Tested on linux
- [ ] Tested on Mac

On visual studio set "engine" as main project.

## Directory organisation
- **/apps/engine**: contains the main application.
- **/lib**: contains the library "qc_graphic". This library contains basic objects to create a quick environment (window, camera and opengl objects).
- **/third-party**:
	- [glfw](http://www.glfw.org/): window and input gestion (easier than SDL).
	- [glm](http://glm.g-truc.net/0.9.8/index.html): mathematical library.
	- [glad](http://glad.dav1d.de/): openGl loader (like GLEW)

## Author
*[Quentin Couillard](http://www.quentincouillard.com/)*