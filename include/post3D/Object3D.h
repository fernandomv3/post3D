#ifndef OBJECT3D_H
#define OBJECT3D_H
#include "post3D/Vec3.h"
#include "post3D/Mat4.h"
#include "post3D/Quaternion.h"
#include "post3D/Euler.h"
#include <memory>
#include <string>

using namespace math3D;
namespace object3D{
class Object3D{
private:
  std::string uuid;
  std::shared_ptr<Vec3> position;
  std::shared_ptr<Euler> rotation;
  std::shared_ptr<Quaternion> quaternion;
  std::shared_ptr<Vec3> scale;
  std::shared_ptr<Mat4> modelMatrix;
  bool visible;
  std::weak_ptr<Object3D> parent;
  float distanceToCamera;
public:
  Object3D();
  Object3D(const Object3D& object);
  Object3D(Object3D&& object);
  Object3D& operator=(const Object3D& object);
  Object3D& operator=(Object3D&& object);
  virtual ~Object3D()=default;
  std::string getUUID()const;
  std::shared_ptr<Vec3> getPosition()const;
  std::shared_ptr<Euler> getRotation()const;
  std::shared_ptr<Vec3> getScale()const;
  Object3D& setPosition(std::shared_ptr<Vec3> position);
  Object3D& setRotation(std::shared_ptr<Euler> rotation);
  Object3D& setScale(std::shared_ptr<Vec3> scale);
  std::shared_ptr<Mat4> getModelMatrix()const;
  Object3D& updateModelMatrix();
  Object3D& setQuaternion(std::shared_ptr<Quaternion> quaternion);
  std::shared_ptr<Quaternion> getQuaternion();
  bool getVisible()const;
  Object3D& setVisible(bool visible);
  std::shared_ptr<Object3D> getParent()const;
  Object3D& setParent(std::shared_ptr<Object3D> parent);
  float getDistanceToCamera()const;
  Object3D& setDistanceToCamera(float distanceToCamera);
};
}
#endif