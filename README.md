# Opengl Engine 

## Rendering Methods

## Compilation instruction
Execute the cmake to generate the project.
- [x] Tested on windows (visual studio)
- [ ] Tested on linux
- [ ] Tested on Mac

On visual studio set "engine" as main project.

### Possible problems
If you have an error at compilation with the include of "experimental/filesystem", it's probably that you don't support the experimental c++ methods. In this case, use the library [Boost](http://www.boost.org/) and reexecute the cmake file with command line below:

	cmake -DGLMLV_USE_BOOST_FILESYSTEM=ON ../opengl_engine/

Or by selecting GLMLV_USE_BOOST_FILESYSTEM, if you use cmake GUI.

## Directory organisation
- **/apps/engine**: contains the main application.
- **/lib**: contains the library "qc". This library contain every source codes and shaders of the graphic and physical engine.
- **/third-party**:
	- [glfw](http://www.glfw.org/): window and input gestion (easier than SDL).
	- [glm](http://glm.g-truc.net/0.9.8/index.html): mathematical library.
	- [glad](http://glad.dav1d.de/): openGl loader (like GLEW)

### Internal library organisation : qc
- **qc::graphic**: graphic library

## Author
*[Quentin Couillard](http://www.quentincouillard.com/)*