#ifndef TEXTUREMATERIAL_H
#define TEXTUREMATERIAL_H

#include "Material.h"
#include "Scene.h"

using namespace scene;
namespace material{
	class TextureMaterial: public Material{
	public:
		TextureMaterial();
		TextureMaterial(const TextureMaterial& mat)=default;
		TextureMaterial(TextureMaterial&& mat)=default;
		TextureMaterial& operator=(const TextureMaterial& mat)=default;
		TextureMaterial& operator=(TextureMaterial&& mat)=default;
		void makePrograms(const Scene& scene);
	};
}

#endif