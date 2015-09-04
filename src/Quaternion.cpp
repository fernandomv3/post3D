#include "post3D/Quaternion.h"
#include "post3D/Vec3.h"
#include "post3D/Euler.h"
#include <string>
#include <cmath>
namespace math3D{

Quaternion::Quaternion(){
  this->x = 0.0f;
  this->y = 0.0f;
  this->z = 0.0f;
  this->w = 1.0f;
}

Quaternion::Quaternion(float x, float y, float z, float w){
  this->x = x;
  this->y = y;
  this->z = z;
  this->w = w;
}

Quaternion::Quaternion(const Quaternion& q){
  this->x = q.x;
  this->y = q.y;
  this->z = q.z;
  this->w = q.w;
  this->euler= std::weak_ptr<Euler>{q.euler};
}

Quaternion::Quaternion(Quaternion&& q){
  this->x = q.x;
  this->y = q.y;
  this->z = q.z;
  this->w = q.w;
  this->euler= std::move(q.euler);
}

Quaternion& Quaternion::operator=(const Quaternion& q){
  this->x = q.x;
  this->y = q.y;
  this->z = q.z;
  this->w = q.w;
  this->euler= std::weak_ptr<Euler>{q.euler};
  return *this;
}

Quaternion& Quaternion::operator=(Quaternion&& q){
  this->x = q.x;
  this->y = q.y;
  this->z = q.z;
  this->w = q.w;
  this->euler= std::move(q.euler);
  return *this;
}


Quaternion& Quaternion::setX(float x){
  this->x = x;
  this->updateEuler();
  return *this;
}

Quaternion& Quaternion::setY(float y){
  this->y = y;
  this->updateEuler();
  return *this;
}

Quaternion& Quaternion::setZ(float z){
  this->z = z;
  this->updateEuler();
  return *this;
}

Quaternion& Quaternion::setW(float w){
  this->w = w;
  this->updateEuler();
  return *this;
}

float Quaternion::getX()const{
  return this->x;
}

float Quaternion::getY()const{
  return this->y;
}

float Quaternion::getZ()const{
  return this->z;
}

float Quaternion::getW()const{
  return this->w;
}

std::shared_ptr<Euler> Quaternion::getEuler()const{
  return this->euler.lock();
}

Quaternion& Quaternion::setEuler(std::shared_ptr<Euler> euler){
  this->euler= std::weak_ptr<Euler>(euler);
  return *this;
}

Quaternion& Quaternion::updateEuler(){
  auto p = this->euler.lock();
  if(p){
    p->setFromQuaternion(*this,p->getOrder(),false);
  }
  return *this;
}


Quaternion& Quaternion::setComponent(int index, float value){
  switch(index){
    case 0:
      this->x = value;
      break;
    case 1:
      this->y = value;
      break;
    case 2:
      this->z = value;
      break;
    case 3:
        this->w = value;
        break;
  }
  this->updateEuler();
  return *this;
}

float Quaternion::getComponent(int index)const{
  switch(index){
    case 0:
      return this->x;
    case 1:
      return this->y;
    case 2:
      return this->z;
    case 3:
        return this->w;
  }
  return 0.0f;
}

Quaternion& Quaternion::setFromEuler(Euler& euler , bool update){
  float c1 = cos( euler.getX() / 2 ),
          c2 = cos( euler.getY() / 2 ),
          c3 = cos( euler.getZ() / 2 ),
          s1 = sin( euler.getX() / 2 ),
          s2 = sin( euler.getY() / 2 ),
          s3 = sin( euler.getZ() / 2 );

    if ( euler.getOrder() == std::string("XYZ") ) {
        this->x = s1 * c2 * c3 + c1 * s2 * s3;
        this->y = c1 * s2 * c3 - s1 * c2 * s3;
        this->z = c1 * c2 * s3 + s1 * s2 * c3;
        this->w = c1 * c2 * c3 - s1 * s2 * s3;

    } else if ( euler.getOrder() == std::string("YXZ") ) {
        this->x = s1 * c2 * c3 + c1 * s2 * s3;
        this->y = c1 * s2 * c3 - s1 * c2 * s3;
        this->z = c1 * c2 * s3 - s1 * s2 * c3;
        this->w = c1 * c2 * c3 + s1 * s2 * s3;

    } else if ( euler.getOrder() == std::string("ZXY") ) {
        this->x = s1 * c2 * c3 - c1 * s2 * s3;
        this->y = c1 * s2 * c3 + s1 * c2 * s3;
        this->z = c1 * c2 * s3 + s1 * s2 * c3;
        this->w = c1 * c2 * c3 - s1 * s2 * s3;

    } else if ( euler.getOrder() == std::string("ZYX") ) {
        this->x = s1 * c2 * c3 - c1 * s2 * s3;
        this->y = c1 * s2 * c3 + s1 * c2 * s3;
        this->z = c1 * c2 * s3 - s1 * s2 * c3;
        this->w = c1 * c2 * c3 + s1 * s2 * s3;

    } else if ( euler.getOrder() == std::string("YZX") ) {
        this->x = s1 * c2 * c3 + c1 * s2 * s3;
        this->y = c1 * s2 * c3 + s1 * c2 * s3;
        this->z = c1 * c2 * s3 - s1 * s2 * c3;
        this->w = c1 * c2 * c3 - s1 * s2 * s3;

    } else if ( euler.getOrder() == std::string("XZY") ) {
        this->x = s1 * c2 * c3 - c1 * s2 * s3;
        this->y = c1 * s2 * c3 - s1 * c2 * s3;
        this->z = c1 * c2 * s3 + s1 * s2 * c3;
        this->w = c1 * c2 * c3 + s1 * s2 * s3;
    }

    if (update){
      this->updateEuler();
    }

    return *this;
}

Quaternion& Quaternion::setFromMat4(const Mat4& mat4){
  return *this;
}

Quaternion& Quaternion::conjugate(){
  this->x = this->x * -1;
  this->y = this->y * -1;
  this->z = this->z * -1;
  return *this;
}
Quaternion& Quaternion::inverse(){
  this->conjugate();
  return this->normalize();
}

Quaternion& Quaternion::normalize(){
  float l = this->length();
  if(l==0){
    this->x=0.0f;
    this->y=0.0f;
    this->z=0.0f;
    this->w=1.0f;
  }
  else{
    l= 1.0f/l;
    this->x*=l;
    this->y*=l;
    this->z*=l;
    this->w*=l;
  }
  return *this;
}

float Quaternion::length()const{
  return sqrt((this->x * this->x)+(this->y * this->y)+(this->z * this->z)+(this->w * this->w));
}


Quaternion& Quaternion::operator*=(const Quaternion& q){
  //refactor this!!!
  Quaternion cpy{*this};
  this->x = cpy.x * q.w + cpy.w * q.x + cpy.y * q.z - cpy.z * q.y;
  this->y = cpy.y * q.w + cpy.w * q.y + cpy.z * q.x - cpy.x * q.z;
  this->z = cpy.z * q.w + cpy.w * q.z + cpy.x * q.y - cpy.y * q.x;
  this->w = cpy.w * q.w - cpy.x * q.x - cpy.y * q.y - cpy.z * q.z;
  this->updateEuler();
  return *this;
}

bool Quaternion::operator==(const Quaternion& q) const{
  bool x = this->x < q.x +0.001f && this->x > q.x -0.001f;
  bool y = this->y < q.y +0.001f && this->y > q.y -0.001f;
  bool z = this->z < q.z +0.001f && this->z > q.z -0.001f;
  bool w = this->w < q.w +0.001f && this->w > q.w -0.001f;
  return x && y && z && w;
}

float Quaternion::dotProduct(const Quaternion& q)const{
  return this->x * q.x + this->y * q.y + this->z * q.z + this->w * q.w;
}


Quaternion operator*(const Quaternion& q1, const Quaternion& q2){
  Quaternion res{q1};
  return res *= q2;
}

float dot(const Quaternion& q1, const Quaternion& q2){
  Quaternion res{q1};
  return res.dotProduct(q2);
}

Quaternion rotationBetweenVectors(const Vec3& vec1,const Vec3& vec2){
  float e = dot(vec1,vec2);
  Vec3 vec = cross(vec1,vec2);
  float term = sqrt(2*(1+e));
  return Quaternion(vec.getX() * 1.0f/term, vec.getY() * 1.0f/term, vec.getZ() * 1.0f/term, term / 2.0f);
}

Quaternion inverse(const Quaternion& q){
  Quaternion res{q};
  return res.conjugate().normalize();
}

}