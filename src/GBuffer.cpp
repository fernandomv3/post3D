#include "post3D/GBuffer.h"
#include <GL/glew.h>
#include <iostream>

using namespace std;
namespace render{

GBuffer::GBuffer(){
  this->fbo =0;
  this->textures = vector<shared_ptr<Texture> >();
  this->depthTexture = shared_ptr<Texture>();
  this->width =0;
  this->height = 0;
}

GBuffer::GBuffer(int width, int height){
  this->fbo =0;
  this->textures = vector<shared_ptr<Texture> >();
  this->depthTexture = shared_ptr<Texture>();
  this->width =width;
  this->height = height;
}


GBuffer::GBuffer(GBuffer&& gbuffer){
  this->fbo = gbuffer.fbo;
  this->textures = move(gbuffer.textures);
  this->depthTexture = move(gbuffer.depthTexture);
  this->width = gbuffer.width;
  this->height = gbuffer.height;
}

GBuffer& GBuffer::operator=(GBuffer&& gbuffer){
  this->fbo = gbuffer.fbo;
  this->textures = move(gbuffer.textures);
  this->depthTexture = move(gbuffer.depthTexture);
  this->width = gbuffer.width;
  this->height = gbuffer.height;
  return *this;
}

GBuffer::~GBuffer(){
  glDeleteFramebuffers(1,(GLuint*)&this->fbo);
}

int GBuffer::getFbo()const{
  return this->fbo;
}

int GBuffer::getHeight()const{
  return this->height;
}

int GBuffer::getWidth()const{
  return this->width;
}

shared_ptr<Texture> GBuffer::getDepthTexture()const{
  return this->depthTexture;
}

const vector<shared_ptr<Texture>>& GBuffer::getTextures() const{
  return this->textures;
}

GBuffer& GBuffer::setFbo(int fbo){
  this->fbo = fbo;
  return *this;
}

GBuffer& GBuffer::setHeight(int height){
  this->height = height;
  return *this;
}

GBuffer& GBuffer::setWidth(int width){
  this->width = width;
  return *this;
}

GBuffer& GBuffer::setDepthTexture(shared_ptr<Texture> depthTexture){
  this->depthTexture = depthTexture;
  return *this;
}

GBuffer& GBuffer::create(){
  /*glGenFramebuffers(1,(GLuint*)&(this->fbo));
  glBindFramebuffer(GL_FRAMEBUFFER,this->fbo);
  for(int i=0; i<4;i++){
    auto tex = shared_ptr<Texture>(new Texture());
    tex->setTexture(tex->createTexture(
      GL_TEXTURE_2D,
      this->width,
      this->height,
      GL_RGB32F,
      GL_RGBA8,
      nullptr,
      GL_FLOAT
    ));
    this->textures.push_back(tex);
  }
  this->depthTexture = shared_ptr<Texture>(new Texture());
  this->depthTexture->setTexture(this->depthTexture->createTexture(
    GL_TEXTURE_2D,
    this->width,
    this->height,
    GL_DEPTH_COMPONENT,
    GL_DEPTH_COMPONENT,
    nullptr
  ));*/
  return *this;
}

GBuffer& GBuffer::configure(){/*
  for(int i =0; i<4;i++){
    glBindTexture(GL_TEXTURE_2D,this->textures[i]->getTexture());
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glFramebufferTexture2D(
      GL_DRAW_FRAMEBUFFER,
      GL_COLOR_ATTACHMENT0 + i,
      GL_TEXTURE_2D,
      this->textures[i]->getTexture(),
      0
    );
  }
  glBindTexture(GL_TEXTURE_2D,this->depthTexture->getTexture());
  glFramebufferTexture2D(
    GL_DRAW_FRAMEBUFFER,
    GL_DEPTH_ATTACHMENT,
    GL_TEXTURE_2D, 
    this->depthTexture->getTexture(),
    0
  );
  GLenum drawBuffers[] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2, GL_COLOR_ATTACHMENT3 };
  glDrawBuffers(4,drawBuffers);
  if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE){
    cout << "Framebuffer not created correctly" << endl;
  }
  glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
  */return *this;
}

GBuffer& GBuffer::bindForWriting(){
  glBindFramebuffer(GL_FRAMEBUFFER, this->fbo);
  glViewport(0,0,this->width,this->height);
  return *this;
}

GBuffer& GBuffer::bindForReading(){
  glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);/*Sets the default framebuffer*/
  /*for(int i = 0; i< 4;i++){
    glActiveTexture(GL_TEXTURE0 + i);
    glBindTexture(GL_TEXTURE_2D,this->textures[i]->getTexture());
  }*/
  return *this;
}

GBuffer& GBuffer::deactivate(){
  glBindFramebuffer(GL_FRAMEBUFFER,0);
  glViewport(0,0,this->width,this->height);
  return *this;
}
}