#ifndef POINTLIGHT_H
#define POINTLIGHT_H
#include "post3D/Light.h"
#include "post3D/Camera.h"
#include <memory>
using namespace std;
using namespace scene;

namespace light{

  struct pLight{
  	float color[4];
  	float position[4];
  	float intensity;
    float attenuation;
  	float stride[2];
  };

  typedef struct pLight PLight;

  class PointLight : public Light{
    private:
    	float intensity;
      float attenuation;
    public:
    	PointLight();
      PointLight(const PointLight& light);
      PointLight(PointLight&& light);
      PointLight& operator=(const PointLight& light);
      PointLight& operator=(PointLight&& light);
    	PointLight& setIntensity(float intensity);
    	float getIntensity()const;
      PointLight& setAttenuation(float attenuation);
      float getAttenuation()const;
    	~PointLight()=default;
    	unique_ptr<PLight> getAsStruct(const Camera& camera)const;
  };
}

#endif