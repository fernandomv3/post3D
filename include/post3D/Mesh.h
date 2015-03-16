#ifndef MESH_H
#define MESH_H

#include "post3D/Object3D.h"
#include "post3D/Geometry.h"
#include "post3D/Material.h"
#include "memory"

using namespace std;
using namespace material;

namespace object3D{
	class Mesh : public Object3D{
	private:
		shared_ptr<Geometry> geometry;
		shared_ptr<Material> material;
		bool castShadows;
	public:
		Mesh();
		Mesh(shared_ptr<Geometry> geometry);
		Mesh(shared_ptr<Geometry> geometry, shared_ptr<Material> material);
		Mesh(const Mesh& mesh);
		Mesh(Mesh&& mesh);
		Mesh& operator=(const Mesh& mesh);
		Mesh& operator=(Mesh&& mesh);
		~Mesh()=default;
		shared_ptr<Geometry> getGeometry()const;
		shared_ptr<Material> getMaterial()const;
		Mesh& setGeometry(shared_ptr<Geometry> geometry);
		Mesh& setMaterial(shared_ptr<Material> material);
		bool getCastShadows()const;
		Mesh& setCastShadows(bool castShadows);
	};
}


#endif