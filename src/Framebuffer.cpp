#include "post3D/Framebuffer.h"
#include <iostream>

namespace render{
Framebuffer::Framebuffer(){
  this->fbo = 0;
  this->depthBuffer = 0;
  this->width = 0;
  this->height = 0;
  this->fboType = GL_COLOR_ATTACHMENT0;
  this->hasDepth = true;
  this->texture = shared_ptr<Texture>(new Texture());
  this->texture->setWidth(this->width);
  this->texture->setHeight(this->height);
  this->texture->setInnerFormat(GL_SRGB8);
  this->texture->setFormat(GL_SRGB8);
  this->texture->setType(GL_UNSIGNED_BYTE);//check if correct
}

Framebuffer::Framebuffer(int width, int height){
  this->fbo = 0;
  this->depthBuffer = 0;
  this->texture = shared_ptr<Texture>(new Texture());
  this->width = width;
  this->height = height;
  this->fboType = GL_COLOR_ATTACHMENT0;
  this->hasDepth = true;
  this->texture->setWidth(this->width);
  this->texture->setHeight(this->height);
  this->texture->setInnerFormat(GL_SRGB8);
  this->texture->setFormat(GL_SRGB8);
  this->texture->setType(GL_UNSIGNED_BYTE);//check if correct
}

/*Framebuffer::Framebuffer(const Framebuffer& fb){
  this->fbo = fb.fbo;
  this->depthBuffer = fb.depthBuffer;
  this->texture = fb.texture;
  this->width = fb.width;
  this->height = fb.height;
}*/

Framebuffer::Framebuffer(Framebuffer&& fb){
  this->fbo = fb.fbo;
  this->depthBuffer = fb.depthBuffer;
  this->texture = move(fb.texture);
  this->width = fb.width;
  this->height = fb.height;
}

Framebuffer::~Framebuffer(){
  int buffers[2] = {this->fbo,this->depthBuffer};
  glDeleteFramebuffers(2,(GLuint*)&buffers);
}

/*Framebuffer& Framebuffer::operator=(const Framebuffer& fb){
  this->fbo = fb.fbo;
  this->depthBuffer = fb.depthBuffer;
  this->texture = fb.texture;
  this->width = fb.width;
  this->height = fb.height;
  return *this;
}*/

Framebuffer& Framebuffer::operator=(Framebuffer&& fb){
  this->fbo = fb.fbo;
  this->depthBuffer = fb.depthBuffer;
  this->texture = move(fb.texture);
  this->width = fb.width;
  this->height = fb.height;
  return *this;
}

int Framebuffer::getFbo()const{
  return this->fbo;
}

int Framebuffer::getDepthBuffer()const{
  return this->depthBuffer;
}

shared_ptr<Texture> Framebuffer::getTexture()const{
  return this->texture;
}

int Framebuffer::getWidth()const{
  return this->width;
}

int Framebuffer::getHeight()const{
  return this->height;
}

int Framebuffer::getFboType()const{
  return this->fboType;
}

bool Framebuffer::getHasDepth()const{
  return this->hasDepth;
}

Framebuffer& Framebuffer::setFbo(int fbo){
  this->fbo = fbo;
  return *this;
}

Framebuffer& Framebuffer::setDepthBuffer(int depthBuffer){
  this->depthBuffer = depthBuffer;
  return *this;
}

Framebuffer& Framebuffer::setTexture(shared_ptr<Texture> texture){
  this->texture = texture;
  return *this;
}

Framebuffer& Framebuffer::setWidth(int width){
  this->width = width;
  return *this;
}

Framebuffer& Framebuffer::setHeight(int height){
  this->height = height;
  return *this;
}

Framebuffer& Framebuffer::setFboType(int fboType){
  this->fboType= fboType;
  return *this;
}

Framebuffer& Framebuffer::setHasDepth(bool hasDepth){
  this->hasDepth = hasDepth;
  return *this;
}

Framebuffer& Framebuffer::create(){
  glGenFramebuffers(1,(GLuint*)&(this->fbo));
  glBindFramebuffer(GL_FRAMEBUFFER,this->fbo);
  if(this->hasDepth){
    this->createDepthBuffer();
  }
  return *this;
}

bool Framebuffer::configure(int texture){
  glFramebufferTexture2D(
    GL_FRAMEBUFFER,
    this->fboType,
    this->texture->getTarget(),
    texture,
    0
  );
  if(this->fboType == GL_DEPTH_ATTACHMENT){
    glDrawBuffer(GL_NONE);
    glReadBuffer(GL_NONE);
  }

  if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE){
    cout << "Framebuffer not created correctly" << endl;
    return false;
  }
  return true;
}

Framebuffer& Framebuffer::bindForWriting(){
  glBindFramebuffer(GL_FRAMEBUFFER, this->fbo);
  glViewport(0,0,this->width,this->height);
  return *this;
}

Framebuffer& Framebuffer::deactivate(){
  glBindFramebuffer(GL_FRAMEBUFFER,0);
  glViewport(0,0,this->width,this->height);
  return *this;
}

Framebuffer& Framebuffer::createDepthBuffer(){
  glGenRenderbuffers(1,(GLuint*)&(this->depthBuffer));
  glBindRenderbuffer(GL_RENDERBUFFER,this->depthBuffer);
  glRenderbufferStorage(GL_RENDERBUFFER,GL_DEPTH_COMPONENT24,this->width,this->height);
  glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, this->depthBuffer);
  return *this;
}

}