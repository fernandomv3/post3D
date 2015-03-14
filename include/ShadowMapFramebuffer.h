#ifndef SHADOWMAPFRAMEBUFFER_H
#define SHADOWMAPFRAMEBUFFER_H

#include <memory>
#include "Framebuffer.h"
#include "DepthMaterial.h"

using namespace material;
using namespace std;

namespace render{
	class ShadowMapFramebuffer: public Framebuffer{
	private:
		shared_ptr<DepthMaterial> material; //change this to Depth Material when implemented
		int sampleSize;
	public:
		ShadowMapFramebuffer();
		ShadowMapFramebuffer(int width, int height);
		ShadowMapFramebuffer(const ShadowMapFramebuffer& smfb);
		ShadowMapFramebuffer(ShadowMapFramebuffer&& smfb);
		ShadowMapFramebuffer& operator=(const ShadowMapFramebuffer& smfb);
		ShadowMapFramebuffer& operator=(ShadowMapFramebuffer&& smfb);
		ShadowMapFramebuffer& create();
		bool configure();
		shared_ptr<DepthMaterial> getMaterial() const;
		ShadowMapFramebuffer& setMaterial(shared_ptr<DepthMaterial> material);
		int getSampleSize()const;
		ShadowMapFramebuffer& setSampleSize(int sampleSize);
	};
}

#endif