#ifndef PHONGMATERIAL_H
#define PHONGMATERIAL_H

#include "Material.h"
#include "Scene.h"

using namespace scene;
namespace material{
	class PhongMaterial: public Material{
	public:
		PhongMaterial();
		PhongMaterial(const PhongMaterial& mat)=default;
		PhongMaterial(PhongMaterial&& mat)=default;
		PhongMaterial& operator=(const PhongMaterial& mat)=default;
		PhongMaterial& operator=(PhongMaterial&& mat)=default;
		void makePrograms(const Scene& scene);
	};
}

#endif