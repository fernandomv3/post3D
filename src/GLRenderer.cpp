#include "post3D/GLRenderer.h"

namespace render{

GLRenderer::GLRenderer(){
}

Renderer& GLRenderer::render(Scene& scene){
  //set framebuffer parameters

  //init and set camera
  
  //for each light in scene
    //init light

  //foreach object in scene
    //init geomerties
    //init materials

  //build shaders
  
  //for each object in scene
    //set geometry data
    // set material data

  //for each light in scene
    //set light data

  //bind shadowMaps and geometry render channels if framebuffer uses them
  
  //render

  return *this;
}

/*int makeBuffer(GLenum target, void* data, int size, GLenum usage){
  int buf;
  glGenBuffers(1,(GLuint*)&buf);
  glBindBuffer(target,buf);
  glBufferData(target,size,data,usage);
  return buf;
}*/
}

