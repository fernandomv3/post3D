#include "Light.h"
using namespace std;
using namespace math3D;
namespace light{
		Light::Light():Object3D(){
			this->color = shared_ptr<Color>(new Color());
		}

		Light::Light(const Light& light):Object3D(light){
			this->color = light.color;
		}

		Light::Light(Light&& light):Object3D(light){
			this->color = move(light.color);
		}

		Light& Light::operator=(const Light& light){
			Object3D::operator=(light);
			this->color = light.color;
			return *this;
		}

		Light& Light::operator=(Light&& light){
			Object3D::operator=(move(light));
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

}