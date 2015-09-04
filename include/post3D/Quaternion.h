#ifndef QUATERNION_H
#define QUATERNION_H
#include <memory>

namespace math3D{
class Mat4;
class Vec3;
class Euler;
class Quaternion{
private:
  float x;
  float y;
  float z;
  float w;
  std::weak_ptr<Euler> euler;
public:
  Quaternion();
  Quaternion(float x, float y, float z, float w);
  ~Quaternion()=default;
  Quaternion(const Quaternion& q);
  Quaternion(Quaternion&& q);
  Quaternion& operator=(const Quaternion& q);
  Quaternion& operator=(Quaternion&& q);

  Quaternion& setX(float x);
  Quaternion& setY(float y);
  Quaternion& setZ(float z);
  Quaternion& setW(float w);
  float getX() const;
  float getY() const;
  float getZ() const;
  float getW() const;
  std::shared_ptr<Euler> getEuler() const;
  Quaternion& setEuler(std::shared_ptr<Euler> euler);
  Quaternion& updateEuler();

  Quaternion& setComponent(int index, float value);
  float getComponent(int index)const;
  Quaternion& setFromEuler(Euler& euler , bool update = true);
  Quaternion& setFromMat4(const Mat4& mat4);
  Quaternion& conjugate();
  Quaternion& inverse();
  Quaternion& normalize();
  float length() const;

  Quaternion& operator*=(const Quaternion& q);
  bool operator==(const Quaternion& q) const;
  float dotProduct(const Quaternion& q) const;
  
};

Quaternion operator*(const Quaternion& q1, const Quaternion& q2);
float dot(const Quaternion& q1, const Quaternion& q2);
Quaternion rotationBetweenVectors(const Vec3& vec1,const Vec3& vec2);
Quaternion inverse(const Quaternion& q);
}
#endif