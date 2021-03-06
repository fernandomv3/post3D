﻿#include "post3D/Light.h"
using namespace std;
using namespace math3D;

namespace light{

Light::Light():Mesh(){
	this->color = shared_ptr<Color>(new Color());
}

Light::Light(const Light& light):Mesh(light){
	this->color = light.color;
}

Light::Light(Light&& light):Mesh(light){
	this->color = move(light.color);
}

Light& Light::operator=(const Light& light){
	Mesh::operator=(light);
	this->color = light.color;
	return *this;
}

Light& Light::operator=(Light&& light){
	Mesh::operator=(move(light));
	this->color = move(light.color);
	return *this;
}

shared_ptr<Color> Light::getColor()const{
	return this->color;
}

Light& Light::setColor(shared_ptr<Color> color){
	this->color = color;
	return *this;
}

/*shared_ptr<Mesh> Light::getMesh() const{
	return this->mesh;
}

Light& Light::setMesh(shared_ptr<Mesh> mesh){
	this->mesh = mesh;
	return *this;
}*/

}