#include "Framebuffer.h"
#include <iostream>

namespace render{
	Framebuffer::Framebuffer(){
		this->fbo = 0;
		this->depthBuffer = 0;
		this->texture = shared_ptr<Texture>(new Texture());
		this->width = 0;
		this->height = 0;
	}

	Framebuffer::Framebuffer(int width, int height){
		this->fbo = 0;
		this->depthBuffer = 0;
		this->texture = shared_ptr<Texture>(new Texture());
		this->width = width;
		this->height = height;
	}

	Framebuffer::Framebuffer(const Framebuffer& fb){
		this->fbo = fb.fbo;
		this->depthBuffer = fb.depthBuffer;
		this->texture = fb.texture;
		this->width = fb.width;
		this->height = fb.height;
	}

	Framebuffer::Framebuffer(Framebuffer&& fb){
		this->fbo = fb.fbo;
		this->depthBuffer = fb.depthBuffer;
		this->texture = move(fb.texture);
		this->width = fb.width;
		this->height = fb.height;
	}

	Framebuffer::~Framebuffer(){

	}

	Framebuffer& Framebuffer::operator=(const Framebuffer& fb){
		this->fbo = fb.fbo;
		this->depthBuffer = fb.depthBuffer;
		this->texture = fb.texture;
		this->width = fb.width;
		this->height = fb.height;
		return *this;
	}

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

	Framebuffer& Framebuffer::create(bool depth){
		this->texture->setTexture(this->texture->createTexture(
			GL_TEXTURE_2D,
			this->width,
			this->height,
			GL_SRGB8,
			GL_SRGB8,
			nullptr
		));
		glGenFramebuffers(1,(GLuint*)&(this->fbo));
		glBindFramebuffer(GL_FRAMEBUFFER,this->fbo);
		if(depth){
			this->createDepthBuffer(this->width,this->height);
		}
		return *this;
	}

	bool Framebuffer::configure(GLenum fboType){
		glFramebufferTexture2D(
			GL_FRAMEBUFFER,
			fboType,
			GL_TEXTURE_2D,
			this->texture->getTexture(),
			0
		);
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

	Framebuffer& Framebuffer::bindForReading(int textureUnit){
		glActiveTexture(textureUnit);
		glBindTexture(GL_TEXTURE_2D,this->texture->getTexture());
		return *this;
	}

	Framebuffer& Framebuffer::deactivate(){
		glBindFramebuffer(GL_FRAMEBUFFER,0);
		glViewport(0,0,this->width,this->height);
		return *this;
	}

	Framebuffer& Framebuffer::createDepthBuffer(int width, int height){
		glGenRenderbuffers(1,(GLuint*)&(this->depthBuffer));
		glBindRenderbuffer(GL_RENDERBUFFER,this->depthBuffer);
		glRenderbufferStorage(GL_RENDERBUFFER,GL_DEPTH_COMPONENT24,width,height);
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, this->depthBuffer);
		return *this;
	}

}