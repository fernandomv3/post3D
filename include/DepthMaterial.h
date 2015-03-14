#ifndef DEPTHMATERIAL_H
#define DEPTHMATERIAL_H

#include "Material.h"
#include "Scene.h"

using namespace scene;
namespace material{
	class DepthMaterial: public Material{
	public:
		DepthMaterial();
		DepthMaterial(const DepthMaterial& mat)=default;
		DepthMaterial(DepthMaterial&& mat)=default;
		DepthMaterial& operator=(const DepthMaterial& mat)=default;
		DepthMaterial& operator=(DepthMaterial&& mat)=default;
		void makePrograms(const Scene& scene);
	};
}

#endif