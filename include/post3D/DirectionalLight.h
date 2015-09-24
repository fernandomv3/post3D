#ifndef DIRECTIONALLIGHT_H
#define DIRECTIONALLIGHT_H

#include "post3D/Object3D.h"
#include "post3D/Light.h"
#include "post3D/Camera.h"
#include <memory>
using namespace scene;
using namespace object3D;
using namespace math3D;
namespace light{
struct dirLight{
  float color[4];
  float vectorToLight[4];
  float intensity;
  float extra[3];
};

typedef struct dirLight DirLight;

class DirectionalLight : public Light{
private:
  std::shared_ptr<Object3D> target;
  float intensity;
  std::shared_ptr<Mat4> lightWorldMatrix;
public:
  DirectionalLight();
  DirectionalLight(const DirectionalLight& dLight);
  DirectionalLight(DirectionalLight&& dLight);
  DirectionalLight& operator=(const DirectionalLight& dLight);
  DirectionalLight& operator=(DirectionalLight&& dLight);
  ~DirectionalLight()=default;
  std::shared_ptr<Object3D> getTarget()const;
  DirectionalLight& setTarget(std::shared_ptr<Object3D> obj);
  float getIntensity()const;
  DirectionalLight& setIntensity(float intensity);
  std::unique_ptr<DirLight> getAsStruct(const Camera& camera)const;
  std::unique_ptr<float[]> getVectorToLightAsArray(const Camera& camera)const;
  std::shared_ptr<Mat4> getLightWorldMatrix() const;
  DirectionalLight& setLightWorldMatrix(std::shared_ptr<Mat4> lightWorldMatrix);
};
}

#endif