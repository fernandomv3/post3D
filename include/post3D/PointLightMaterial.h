#ifndef POINTLIGHTMATERIAL_H
#define POINTLIGHTMATERIAL_H

#include "post3D/Material.h"
#include "post3D/Scene.h"

using namespace scene;
namespace material{
	class PointLightMaterial: public Material{
	public:
		PointLightMaterial();
		PointLightMaterial(const PointLightMaterial& mat)=default;
		PointLightMaterial(PointLightMaterial&& mat)=default;
		PointLightMaterial& operator=(const PointLightMaterial& mat)=default;
		PointLightMaterial& operator=(PointLightMaterial&& mat)=default;
		void makePrograms(const Scene& scene,bool deferred = false);
		void makeForwardShader();
		void makeDeferredShader();
	};
}

#endif