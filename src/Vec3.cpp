#include "post3D/Vec3.h"
#include "post3D/Mat4.h"
#include <cmath>
#include <memory>
#include <vector>
namespace math3D{

Vec3::Vec3(){
  this->components = std::vector<float>(4,0.0f);
}

Vec3::Vec3(float x, float y, float z){
  this->components = std::vector<float>();
  this->components.push_back(x);
  this->components.push_back(y);
  this->components.push_back(z);
  this->components.push_back(1.0f);
}

Vec3::Vec3(const Vec3& v){
  this->components = v.components;
}
Vec3::Vec3(Vec3&& v){
  this->components = std::move(v.components);
}
Vec3& Vec3::operator=(const Vec3& v){
  this->components = v.components;
  return *this;
}
Vec3& Vec3::operator=(Vec3&& v){
  this->components = std::move(v.components);
  return *this;
}

//setters & getters soon to be deprecated
Vec3& Vec3::setX(float x){
  this->components[0] = x;
  return *this;
}

Vec3& Vec3::setY(float y){
  this->components[1] = y;
  return *this;
}

Vec3& Vec3::setZ(float z){
  this->components[2] = z;
  return *this;
}

float Vec3::getX()const{
  return this->components[0];
}

float Vec3::getY()const{
  return this->components[1];
}

float Vec3::getZ()const{
  return this->components[2];
}


Vec3& Vec3::setComponent(int index, float value){
  if (index < 3 && index >= 0){
    this->components[index] = value;
  }
  return *this;
}

float Vec3::getComponent(int index) const{
  if (index < 3 && index >= 0){
    return this->components[index];
  }
  return 0.0f; //should throw an exception    
}

Vec3& Vec3::setFromArray(float value[],int len){
  if(len >= 3){
    this->components[0] = value[0];
    this->components[1] = value[1];
    this->components[2] = value[2];
  }else if(len == 2){
    this->components[0] = value[0];
    this->components[1] = value[1];
  }
  return *this;
}

Vec3& Vec3::applyMatrix(const Mat4& mat, bool normalize){
  float x = mat[0] * this->components[0] + mat[1] * this->components[1] + mat[2] * this->components[2] + mat[3] * this->components[3];
  float y = mat[4] * this->components[0] + mat[5] * this->components[1] + mat[6] * this->components[2] + mat[7] * this->components[3];
  float z = mat[8] * this->components[0] + mat[9] * this->components[1] + mat[10] * this->components[2] + mat[11] * this->components[3];
  this->components[0] =x;
  this->components[1] =y;
  this->components[2] =z;
  if (normalize){
    float newW  = mat[12] * this->components[0] + mat[13] * this->components[1] + mat[14] * this->components[2] + mat[15] * this->components[3];
    this->components[0] /= newW;
    this->components[1] /= newW;
    this->components[2] /= newW;
  }
  return *this;
}

Vec3& Vec3::crossProduct(const Vec3& vec){
  float x = this->components[1] * vec.components[2] - this->components[2] * vec.components[1];
  float y = this->components[2] * vec.components[0] - this->components[0] * vec.components[2];
  float z = this->components[0] * vec.components[1] - this->components[1] * vec.components[0];
  this->components[0] = x;
  this->components[1] = y;
  this->components[2] = z;
  return *this;
}

float Vec3::dotProduct(const Vec3& vec) const{
  return this->components[0] * vec.components[0] + this->components[1] * vec.components[1] + this->components[2] * vec.components[2];
}

Vec3& Vec3::operator+=(const Vec3& v){
  this->components[0] += v.components[0];
  this->components[1] += v.components[1];
  this->components[2] += v.components[2];
  return *this;
}

Vec3& Vec3::operator-=(const Vec3& v){
  this->components[0] -= v.components[0];
  this->components[1] -= v.components[1];
  this->components[2] -= v.components[2];
  return *this;
}


Vec3& Vec3::normalize(){
  //TODO: normalize creates a new vec3
  float l = this->length();
  if(l==0){
    this->components[0]=0;
    this->components[1]=0;
    this->components[2]=0;
  }
  else{
    l= 1/l;
    this->components[0]*=l;
    this->components[1]*=l;
    this->components[2]*=l;
  }
  return *this;
}

float Vec3::length()const{
  return sqrt((this->components[0] * this->components[0])+(this->components[1] * this->components[1])+(this->components[2] * this->components[2]));
}



bool Vec3::insideUnitCube()const{
  if( this->components[0] <= 1 && this->components[0] >= -1 &&
    this->components[1] <= 1 && this->components[1] >= -1 &&
    this->components[2] <= 1 && this->components[2] >= -1){
    return true;
  }
  return false;
}

std::unique_ptr<float[]> Vec3::getAsArray()const{
  std::unique_ptr<float[]> vec{new float[4]};
  vec[0] = this->components[0];
  vec[1] = this->components[1];
  vec[2] = this->components[2];
  vec[3] = this->components[3];
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
  this->components[3] = w;
  return *this;
}

float Vec3::getW()const{
  return this->components[3];
}


}