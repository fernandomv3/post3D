#ifndef DIRECTIONALLIGHTMATERIAL_H
#define DIRECTIONALLIGHTMATERIAL_H

#include "post3D/Material.h"
#include "post3D/Scene.h"

using namespace scene;
namespace material{
	class DirectionalLightMaterial: public Material{
	public:
		DirectionalLightMaterial();
		DirectionalLightMaterial(const DirectionalLightMaterial& mat)=default;
		DirectionalLightMaterial(DirectionalLightMaterial&& mat)=default;
		DirectionalLightMaterial& operator=(const DirectionalLightMaterial& mat)=default;
		DirectionalLightMaterial& operator=(DirectionalLightMaterial&& mat)=default;
		void makePrograms(const Scene& scene,bool deferred = false);
		void makeForwardShader();
		void makeDeferredShader();
	};
}

#endif