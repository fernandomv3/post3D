#ifndef LIGHT_H
#define LIGHT_H
#include "post3D/Mesh.h"
#include "post3D/Color.h"
#include <memory>

using namespace object3D;

namespace light{
class Light : public Object3D {
private:
  std::shared_ptr<Color> color;
public:
  Light();
  Light(const Light& light);
  Light(Light&& light);
  Light& operator=(const Light& light);
  Light& operator=(Light&& light);
  virtual ~Light()=default;
  std::shared_ptr<Color> getColor() const;
  Light& setColor(std::shared_ptr<Color> color);
};
}
#endif