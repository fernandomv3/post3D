#ifndef MESH_H
#define MESH_H

#include "post3D/Object3D.h"
#include "post3D/Geometry.h"
#include <memory>

namespace material{
class Material;
}

using namespace material;

namespace object3D{
class Mesh : public Object3D{
private:
  std::shared_ptr<Geometry> geometry;
  std::shared_ptr<Material> material;
  bool castShadows;
public:
  Mesh();
  Mesh(std::shared_ptr<Geometry> geometry);
  Mesh(std::shared_ptr<Geometry> geometry, std::shared_ptr<Material> material);
  Mesh(const Mesh& mesh);
  Mesh(Mesh&& mesh);
  Mesh& operator=(const Mesh& mesh);
  Mesh& operator=(Mesh&& mesh);
  ~Mesh()=default;
  std::shared_ptr<Geometry> getGeometry()const;
  std::shared_ptr<Material> getMaterial()const;
  Mesh& setGeometry(std::shared_ptr<Geometry> geometry);
  Mesh& setMaterial(std::shared_ptr<Material> material);
  bool getCastShadows()const;
  Mesh& setCastShadows(bool castShadows);
};
}


#endif