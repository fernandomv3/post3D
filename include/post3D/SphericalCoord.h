#ifndef SPHERICALCOORD_H
#define SPHERICALCOORD_H

#include "post3D/Vec3.h"
namespace math3D{
class SphericalCoord{
private:
  float phi;
  float theta;
  float r;
public:
  static const float MAXANG;
  static const float MINANG;
  static const float EPS;
  SphericalCoord()=default;
  SphericalCoord(const SphericalCoord& s)=default;
  SphericalCoord(SphericalCoord&& s)=default;
  SphericalCoord& operator=(const SphericalCoord& s)=default;
  SphericalCoord& operator=(SphericalCoord&& s)=default;
  ~SphericalCoord()=default;
  SphericalCoord(const Vec3& cartesian,const Vec3& relOrigin = Vec3{});
  float getPhi()const;
  float getTheta()const;
  float getR()const;
  SphericalCoord& setPhi(float phi);
  SphericalCoord& setTheta(float theta);
  SphericalCoord& setR(float r);
  SphericalCoord& setFromCartesian(const Vec3& position,const Vec3& relOrigin = Vec3{});
  Vec3 getCartesian(const Vec3& relOrigin = Vec3{})const;
};
Vec3 orbit(const Vec3& origin, const Vec3& position, float deltaPhi, float deltaTheta, float radiusFactor);
}
#endif