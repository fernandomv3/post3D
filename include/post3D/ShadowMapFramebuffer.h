#ifndef SHADOWMAPFRAMEBUFFER_H
#define SHADOWMAPFRAMEBUFFER_H

#include "post3D/Framebuffer.h"
#include "post3D/DepthMaterial.h"
#include <memory>

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
  ShadowMapFramebuffer(const ShadowMapFramebuffer& smfb)=delete;
  ShadowMapFramebuffer(ShadowMapFramebuffer&& smfb);
  ShadowMapFramebuffer& operator=(const ShadowMapFramebuffer& smfb)=delete;
  ShadowMapFramebuffer& operator=(ShadowMapFramebuffer&& smfb);
  shared_ptr<DepthMaterial> getMaterial() const;
  ShadowMapFramebuffer& setMaterial(shared_ptr<DepthMaterial> material);
  int getSampleSize()const;
  ShadowMapFramebuffer& setSampleSize(int sampleSize);
};
}

#endif