#ifndef EULER_H
#define EULER_H
#include <memory>
#include <string>
namespace math3D{
class Quaternion;

class Euler{
private:
  float x;
  float y;
  float z;
  std::string order;
  std::weak_ptr<Quaternion> quaternion;

public:
  Euler();
  Euler(float x, float y, float z, const std::string order);
  Euler(float x, float y, float z);
  Euler(const Euler& euler);
  Euler(Euler&& euler);
  Euler& operator=(const Euler& euler);
  Euler& operator=(Euler&& euler);
  ~Euler()=default;
  std::string getOrder()const;
  float getX()const;
  Euler& setX(float x);
  float getY()const;
  Euler& setY(float y);
  float getZ()const;
  Euler& setZ(float z);
  Euler& setFromQuaternion(Quaternion& q, const std::string order, bool update = true);
  Euler& updateQuaternion();
  Euler& setQuaternion(std::shared_ptr<Quaternion> q);
  std::shared_ptr<Quaternion> getQuaternion()const;
};
float clamp(float num);
}
#endif