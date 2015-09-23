#include "post3D/Light.h"
using namespace math3D;

namespace light{

Light::Light():Object3D(){
  this->color = std::shared_ptr<Color>(new Color());
}

Light::Light(const Light& light):Object3D(light){
  this->color = light.color;
}

Light::Light(Light&& light):Object3D(light){
  this->color = std::move(light.color);
}

Light& Light::operator=(const Light& light){
  Object3D::operator=(light);
  this->color = light.color;
  return *this;
}

Light& Light::operator=(Light&& light){
  Object3D::operator=(std::move(light));
  this->color = std::move(light.color);
  return *this;
}

std::shared_ptr<Color> Light::getColor()const{
  return this->color;
}

Light& Light::setColor(std::shared_ptr<Color> color){
  this->color = color;
  return *this;
}

}