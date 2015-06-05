#ifndef PHONGMATERIAL_H
#define PHONGMATERIAL_H

#include "post3D/Material.h"
#include "post3D/Scene.h"

using namespace scene;
namespace material{
	class PhongMaterial: public Material{
	public:
		PhongMaterial();
		PhongMaterial(const PhongMaterial& mat)=default;
		PhongMaterial(PhongMaterial&& mat)=default;
		PhongMaterial& operator=(const PhongMaterial& mat)=default;
		PhongMaterial& operator=(PhongMaterial&& mat)=default;
		void makePrograms(const Scene& scene,bool deferred = false);
		void makeForwardShader();
		void makeDeferredShader();
	};
}

#endif