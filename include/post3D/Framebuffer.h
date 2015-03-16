#ifndef FRAMEBUFFER_H
#define FRAMEBUFFER_H
#include "post3D/Texture.h"
#include <GL/glew.h>
#include <memory>

using namespace texture;
using namespace std;
namespace render{
	class Framebuffer{
	private:
		int fbo;
		int depthBuffer;
		shared_ptr<Texture> texture;
		int width;
		int height;
	public:
		Framebuffer();
		Framebuffer(int width, int height);
		Framebuffer(const Framebuffer& fb);
		Framebuffer(Framebuffer&& fb);
		~Framebuffer();
		Framebuffer& operator=(const Framebuffer& fb);
		Framebuffer& operator=(Framebuffer&& fb);
		int getFbo()const;
		int getDepthBuffer()const;
		shared_ptr<Texture> getTexture()const;
		int getWidth()const;
		int getHeight()const;
		Framebuffer& setFbo(int fbo);
		Framebuffer& setDepthBuffer(int depthBuffer);
		Framebuffer& setTexture(shared_ptr<Texture> texture);
		Framebuffer& setWidth(int width);
		Framebuffer& setHeight(int height);
		virtual Framebuffer& create(bool depth);
		virtual bool configure(GLenum fboType = GL_COLOR_ATTACHMENT0);
		Framebuffer& bindForWriting();
		Framebuffer& bindForReading(int textureUnit);
		Framebuffer& deactivate();
		Framebuffer& createDepthBuffer(int width, int height);
	};
}

#endif