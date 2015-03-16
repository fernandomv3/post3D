#ifndef DIRECTIONALLIGHT_H
#define DIRECTIONALLIGHT_H

#include "post3D/Object3D.h"
#include "post3D/Light.h"
#include "post3D/Camera.h"
#include <memory>
using namespace scene;
using namespace object3D;
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
		shared_ptr<Object3D> target;
		float intensity;
	public:
		DirectionalLight();
		DirectionalLight(const DirectionalLight& dLight);
		DirectionalLight(DirectionalLight&& dLight);
		DirectionalLight& operator=(const DirectionalLight& dLight);
		DirectionalLight& operator=(DirectionalLight&& dLight);
		~DirectionalLight()=default;
		shared_ptr<Object3D> getTarget()const;
		DirectionalLight& setTarget(shared_ptr<Object3D> obj);
		float getIntensity()const;
		DirectionalLight& setIntensity(float intensity);
		unique_ptr<DirLight> getAsStruct(const Camera& camera)const;
		unique_ptr<float[]> getVectorToLightAsArray(const Camera& camera)const;
	};
}

#endif