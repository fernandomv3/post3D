#include "ShadowMapFramebuffer.h"
#include <GL/glew.h>

namespace render{
	ShadowMapFramebuffer::ShadowMapFramebuffer():Framebuffer(){
		this->material = shared_ptr<DepthMaterial>(new DepthMaterial());
		this->sampleSize = 1;
	}

	ShadowMapFramebuffer::ShadowMapFramebuffer(int width, int height):Framebuffer(width,height){
		this->material = shared_ptr<DepthMaterial>(new DepthMaterial());
		this->sampleSize = 1;
	}

	ShadowMapFramebuffer::ShadowMapFramebuffer(const ShadowMapFramebuffer& smfb):Framebuffer(smfb){
		this->material = smfb.material;
		this->sampleSize = smfb.sampleSize;
	}

	ShadowMapFramebuffer::ShadowMapFramebuffer(ShadowMapFramebuffer&& smfb):Framebuffer(smfb){
		this->material = move(smfb.material);
		this->sampleSize = smfb.sampleSize;
	}

	ShadowMapFramebuffer& ShadowMapFramebuffer::operator=(const ShadowMapFramebuffer& smfb){
		Framebuffer::operator=(smfb);
		this->material = smfb.material;
		this->sampleSize = smfb.sampleSize;
		return *this;
	}

	ShadowMapFramebuffer& ShadowMapFramebuffer::operator=(ShadowMapFramebuffer&& smfb){
		Framebuffer::operator=(move(smfb));
		this->material = move(smfb.material);
		this->sampleSize = smfb.sampleSize;
		return *this;
	}

	ShadowMapFramebuffer& ShadowMapFramebuffer::create(){
		this->getTexture()->setTexture(this->getTexture()->createTexture(
			GL_TEXTURE_2D,
			this->getWidth(),
			this->getHeight(),
			GL_DEPTH_COMPONENT,
			GL_DEPTH_COMPONENT,
			nullptr
		));
		int fb;
		glGenFramebuffers(1,(GLuint*)&fb);
		this->setFbo(fb);
		glBindFramebuffer(GL_FRAMEBUFFER,this->getFbo());
		return *this;
	}

	bool ShadowMapFramebuffer::configure(){
		Framebuffer::configure(GL_DEPTH_ATTACHMENT);
		glDrawBuffer(GL_NONE);
		glReadBuffer(GL_NONE);
		return true;
	}

	shared_ptr<DepthMaterial> ShadowMapFramebuffer::getMaterial() const{
		return this->material;
	}

	ShadowMapFramebuffer& ShadowMapFramebuffer::setMaterial(shared_ptr<DepthMaterial> material){
		this->material = material;
		return *this;
	}

	int ShadowMapFramebuffer::getSampleSize() const{
		return this->sampleSize;
	}

	ShadowMapFramebuffer& ShadowMapFramebuffer::setSampleSize(int sampleSize) {
		this->sampleSize = sampleSize;
		return *this;
	}

}