#ifndef TEXTUREMATERIAL_H
#define TEXTUREMATERIAL_H

#include "post3D/Material.h"
#include "post3D/Scene.h"

using namespace scene;
namespace material{
	class TextureMaterial: public Material{
	public:
		TextureMaterial();
		TextureMaterial(const TextureMaterial& mat)=default;
		TextureMaterial(TextureMaterial&& mat)=default;
		TextureMaterial& operator=(const TextureMaterial& mat)=default;
		TextureMaterial& operator=(TextureMaterial&& mat)=default;
		void makePrograms(const Scene& scene, bool deferred = false);
	};
}

#endif