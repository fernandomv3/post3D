#include "post3D/Vec3.h"
#include "post3D/Mat4.h"
#include <cmath>
#include <memory>
namespace math3D{

Vec3::Vec3(float x, float y, float z){
  this->x = x;
  this->y = y;
  this->z = z;
}

//setters & getters soon to be deprecated
Vec3& Vec3::setX(float x){
  this->x = x;
  return *this;
}

Vec3& Vec3::setY(float y){
  this->y = y;
  return *this;
}

Vec3& Vec3::setZ(float z){
  this->z = z;
  return *this;
}

float Vec3::getX()const{
  return this->x;
}

float Vec3::getY()const{
  return this->y;
}

float Vec3::getZ()const{
  return this->z;
}


Vec3& Vec3::setComponent(int index, float value){
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
  }
  return *this;
}

float Vec3::getComponent(int index) const{
  switch(index){
    case 0:
      return this->x;
    case 1:
      return this->y;
    case 2:
      return this->z;
  }
  return 0.0f; //should throw an exception    
}

Vec3& Vec3::setFromArray(float value[],int len){
  if(len >= 3){
    this->x = value[0];
    this->y = value[1];
    this->z = value[2];
  }else if(len == 2){
    this->x = value[0];
    this->y = value[1];
  }
  return *this;
}

Vec3& Vec3::applyMatrix(const Mat4& mat, bool normalize){
  float x = mat[0] * this->x + mat[1] * this->y + mat[2] * this->z + mat[3] * this->w;
  float y = mat[4] * this->x + mat[5] * this->y + mat[6] * this->z + mat[7] * this->w;
  float z = mat[8] * this->x + mat[9] * this->y + mat[10] * this->z + mat[11] * this->w;
  this->x =x;
  this->y =y;
  this->z =z;
  if (normalize){
    float newW  = mat[12] * this->x + mat[13] * this->y + mat[14] * this->z + mat[15] * this->w;
    this->x /= newW;
    this->y /= newW;
    this->z /= newW;
  }
  return *this;
}

Vec3& Vec3::crossProduct(const Vec3& vec){
  float x = this->y * vec.z - this->z * vec.y;
  float y = this->z * vec.x - this->x * vec.z;
  float z = this->x * vec.y - this->y * vec.x;
  this->x = x;
  this->y = y;
  this->z = z;
  return *this;
}

float Vec3::dotProduct(const Vec3& vec) const{
  return this->x * vec.x + this->y * vec.y + this->z * vec.z;
}

Vec3& Vec3::operator+=(const Vec3& v){
  this->x += v.x;
  this->y += v.y;
  this->z += v.z;
  return *this;
}

Vec3& Vec3::operator-=(const Vec3& v){
  this->x -= v.x;
  this->y -= v.y;
  this->z -= v.z;
  return *this;
}


Vec3& Vec3::normalize(){
  //TODO: normalize creates a new vec3
  float l = this->length();
  if(l==0){
    this->x=0;
    this->y=0;
    this->z=0;
  }
  else{
    l= 1/l;
    this->x*=l;
    this->y*=l;
    this->z*=l;
  }
  return *this;
}

float Vec3::length()const{
  return sqrt((this->x * this->x)+(this->y * this->y)+(this->z * this->z));
}



bool Vec3::insideUnitCube()const{
  if( this->x <= 1 && this->x >= -1 &&
    this->y <= 1 && this->y >= -1 &&
    this->z <= 1 && this->z >= -1){
    return true;
  }
  return false;
}

std::unique_ptr<float[]> Vec3::getAsArray()const{
  unique_ptr<float[]> vec{new float[4]};
  vec[0] = this->x;
  vec[1] = this->y;
  vec[2] = this->z;
  vec[3] = this->w;
  return vec;
}

float distance(const Vec3& v1, const Vec3& v2){
  return (v1 - v2).length();
}

float dot(const Vec3& v1, const Vec3& v2){
  Vec3 res{v1};
  return res.dotProduct(v2);
}

Vec3 cross(const Vec3& v1, const Vec3& v2){
  Vec3 res{v1};
  return res.crossProduct(v2);
}

Vec3 operator+(const Vec3& v1, const Vec3& v2){
  Vec3 res{v1};
  return res+=v2;
}

Vec3 operator-(const Vec3& v1, const Vec3& v2){
  Vec3 res{v1};
  return res-=v2;
}

Vec3 operator*(const Mat4& mat, const Vec3& v){
  Vec3 res{v};
  return res.applyMatrix(mat,false);
}

Vec3& Vec3::setW(float w){
  this->w = w;
  return *this;
}

float Vec3::getW()const{
  return this->w;
}


}