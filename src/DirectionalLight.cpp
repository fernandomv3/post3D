#include "post3D/DirectionalLight.h"
#include "post3D/Vec3.h"

namespace light{
	DirectionalLight::DirectionalLight():Light(){
		this->target = shared_ptr<Object3D>(new Object3D());
		this->intensity = 1.0f;
	}

	DirectionalLight::DirectionalLight(const DirectionalLight& dLight):Light(dLight){
		this->target = dLight.target;
		this->intensity = dLight.intensity;
	}

	DirectionalLight::DirectionalLight(DirectionalLight&& dLight):Light(dLight){
		this->target = move(dLight.target);
		this->intensity = dLight.intensity;
	}

	DirectionalLight& DirectionalLight::operator=(const DirectionalLight& dLight){
		Light::operator=(dLight);
		this->target = dLight.target;
		this->intensity = dLight.intensity;
		return *this;
	}

	DirectionalLight& DirectionalLight::operator=(DirectionalLight&& dLight){
		Light::operator=(move(dLight));
		this->target = move(dLight.target);
		this->intensity = dLight.intensity;
		return *this;
	}

	shared_ptr<Object3D> DirectionalLight::getTarget()const{
		return this->target;
	}

	DirectionalLight& DirectionalLight::setTarget(shared_ptr<Object3D>obj){
		this->target = obj;
		return *this;
	}

	float DirectionalLight::getIntensity()const{
		return this->intensity;
	}

	DirectionalLight& DirectionalLight::setIntensity(float intensity){
		this->intensity = intensity;
		return *this;
	}

	unique_ptr<float[]> DirectionalLight::getVectorToLightAsArray(const Camera& camera)const{
		unique_ptr<float[]> vec{new float[4]};
		Vec3 worldLightPos = *(camera.getWorldMatrix()) * *(this->getPosition());
		Vec3 worldTargetPos = *(camera.getWorldMatrix()) * *(this->target->getPosition());

		vec[0]= worldLightPos.getX() - worldTargetPos.getX();
		vec[1]= worldLightPos.getY() - worldTargetPos.getY();
		vec[2]= worldLightPos.getZ() - worldTargetPos.getZ();
		vec[3]= 0.0;
		return vec;
	}

	unique_ptr<DirLight> DirectionalLight::getAsStruct(const Camera& camera)const{
		unique_ptr<DirLight> light = unique_ptr<DirLight>{new DirLight};
		light->intensity = this->intensity;
		auto color = this->getColor()->getAsArray();
		light->color[0] = color[0];
		light->color[1] = color[1];
		light->color[2] = color[2];
		light->color[3] = color[3];
		auto vec = this->getVectorToLightAsArray(camera);
		light->vectorToLight[0] = vec[0];
		light->vectorToLight[1] = vec[1];
		light->vectorToLight[2] = vec[2];
		light->vectorToLight[3] = vec[3];
		return light;
	}

}