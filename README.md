# ARDnD
An augmented reality tabletop using OpenGL and OpenCV.

## Credits
* OpenCV code for detecting squares is based on [an OpenCV tutorial](https://github.com/Itseez/opencv/blob/master/samples/cpp/squares.cpp).
  * I added the ability to filter colors and detect the orientation and scale of the square.
* OpenGL code is a combination of [multiple](https://open.gl/) [different](http://www.opengl-tutorial.org/beginners-tutorials/tutorial-8-basic-shading/) [tutorials](http://learnopengl.com/).
  * I added the ability to render multiple different objects in the same frame (surprisingly not many tutorials cover this.)

## Running
A build for rendering frames from a folder of sample images is currently available under the GitHub releases section.

## Screenshot
![Screenshot](https://raw.githubusercontent.com/wastevensv/ARDnD/master/screenshot.png?token=ABMgCQ8jK4b5XpNzAdJBHwRp9bFcPj6Cks5XSu_rwA%3D%3D)

## Dependencies
* OpenCV
* OpenGL (3.3 or above)
* glfw3
* glew32
* SOIL

## Building
This project uses CMake for building. It works using Visual Studio 2015 on my Windows 10 laptop. Currently the process of setting up dependencies is a bit involved. It is primarily based on [this tutorial](http://web.cs.sunyit.edu/~realemj/guides/installOpenGL.html) for OpenGL, glfw, glew, and SOIL. In addition I [built OpenCV from source](http://docs.opencv.org/2.4/doc/tutorials/introduction/windows_install/windows_install.html#installation-by-making-your-own-libraries-from-the-source-files) (however, its probably possible to just use the installer.). After that, the any .lib files being used should be copied to `$DEP_ROOT_DIR/lib`, then any headers should be put in `$DEP_ROOT_DIR/include`, and all the OpenCV .dll's should be put in `$DEP_ROOT_DIR/bin`.

Once the dependencies are initially setup, all that is left is to configure and generate the build system from CMake, and build the ardnd target. If using static images (the default), then the samples target should also be built.
