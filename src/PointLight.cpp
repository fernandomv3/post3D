#include "post3D/PointLight.h"
#include "post3D/Light.h"
#include "post3D/Camera.h"
#include "post3D/Vec3.h"
#include <memory>
using namespace std;
using namespace scene;

namespace light{
PointLight::PointLight():Light(){
  this->intensity = 1.0f;
  this->attenuation= 0.3f;

}

PointLight::PointLight(const PointLight& light):Light(light){
  this->intensity = light.intensity;
  this->attenuation= light.attenuation;
}

PointLight::PointLight(PointLight&& light):Light(light){
  this->intensity = light.intensity;
  this->attenuation= light.attenuation;
}

PointLight& PointLight::operator=(const PointLight& light){
  Light::operator=(light);
  this->intensity = light.intensity;
  this->attenuation= light.attenuation;
  return *this;
}

PointLight& PointLight::operator=(PointLight&& light){
  Light::operator=(std::move(light));
  this->intensity = light.intensity;
  this->attenuation= light.attenuation;
  return *this;
}

PointLight& PointLight::setIntensity(float intensity){
  this->intensity = intensity;
  return *this;
}

float PointLight::getIntensity()const{
  return this-> intensity;
}

PointLight& PointLight::setAttenuation(float attenuation){
  this->attenuation = attenuation;
  return *this;
}

float PointLight::getAttenuation()const{
  return attenuation;
}

std::unique_ptr<PLight> PointLight::getAsStruct(const Camera& camera)const{
  std::unique_ptr<PLight> light = std::unique_ptr<PLight>{new PLight};
  light->intensity = this->intensity;
  light->attenuation = this->attenuation;
  auto color = this->getColor()->getAsArray();
  light->color[0] = color[0];
  light->color[1] = color[1];
  light->color[2] = color[2];
  light->color[3] = color[3];
  auto position = (*(camera.getWorldMatrix()) * *(this->getPosition())).getAsArray();
  light->position[0] = position[0];
  light->position[1] = position[1];
  light->position[2] = position[2];
  light->position[3] = position[3];
  return light;
}

}