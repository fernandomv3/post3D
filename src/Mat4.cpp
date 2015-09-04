#include "post3D/Vec3.h"
#include "post3D/Mat4.h"
#include "post3D/Quaternion.h"
#include <iostream>
#include <cmath>

namespace math3D{
Mat4::Mat4(){
  this->elements = std::vector<float>(16,0.0f);
}

Mat4::Mat4(float value){
  this->elements = std::vector<float>(16,value);
}

Mat4::Mat4(const float elements[16]){
  this->elements = std::vector<float>(elements,elements+16);
}

Mat4::Mat4(const Mat4& mat){
  this->elements = std::vector<float>(16);
  this->elements = mat.elements;
}

Mat4::Mat4(Mat4&& mat){
  this->elements = move(mat.elements);
}

Mat4& Mat4::operator=(const Mat4& mat){
  this->elements = std::vector<float>(16);
  this->elements = mat.elements;
  return *this;
}

Mat4& Mat4::operator=(Mat4&& mat){
  this->elements = move(mat.elements);
  return *this;
}

float& Mat4::operator[](int index){
  return this->elements[index];
}

const float& Mat4::operator[](int index) const{
  return this->elements[index];
}

std::unique_ptr<float[]> Mat4::getAsArray() const{
  auto res = std::unique_ptr<float[]>(new float[16]);
  for(int i = 0; i < 16 ; i++){
    res[i] = this->elements[i];
  }
  return move(res);
}


Mat4 Mat4::identityMatrix(){
  Mat4 mat{};
  mat[0] =1.0f;
  mat[5] =1.0f;
  mat[10] =1.0f;
  mat[15] =1.0f;
  return mat;
}

Mat4 Mat4::translationMatrix(float x, float y , float z){
  Mat4 mat{Mat4::identityMatrix()};
  mat[3] =x;
  mat[7] =y;
  mat[11]=z;
  return mat;
}

Mat4 Mat4::rotationMatrixFromQuaternion(const Quaternion& q){
  Mat4 mat{Mat4::identityMatrix()};
  Quaternion q1{q};
  q1.normalize();
  mat[0] = 1-(2*q1.getY()*q1.getY())-(2*q1.getZ()*q1.getZ());
  mat[1] = (2*q1.getX()*q1.getY())-(2*q1.getW()*q1.getZ());
  mat[2] = (2*q1.getX()*q1.getZ())+(2*q1.getW()*q1.getY());
  mat[4] = (2*q1.getX()*q1.getY())+(2*q1.getW()*q1.getZ());
  mat[5] = 1-(2*q1.getX()*q1.getX())-(2*q1.getZ()*q1.getZ());
  mat[6] = (2*q1.getY()*q1.getZ())-(2*q1.getW()*q1.getX());
  mat[8] = (2*q1.getX()*q1.getZ())-(2*q1.getW()*q1.getY());
  mat[9] = (2*q1.getY()*q1.getZ())+(2*q1.getW()*q1.getX());
  mat[10] = 1-(2*q1.getX()*q1.getX())-(2*q1.getY()*q1.getY());
  return mat;
}

Mat4 Mat4::scaleMatrix(float x, float y , float z){
  Mat4 mat{Mat4::identityMatrix()};
  mat[0] =x;
  mat[5] =y;
  mat[10]=z;
  return mat;
}

Mat4& Mat4::operator*=(float scalar){
  for(int i =0; i < 16 ; i++){
    this->elements[i] *= scalar;
  }
  return *this;
}

Mat4 Mat4::rotationMatrix(float x , float y, float z){
  float radX = 3.14159 * x /180.0;
  float radY = 3.14159 * y /180.0;
  float radZ = 3.14159 * z /180.0;

  Mat4 rotX{Mat4::identityMatrix()};
  rotX[5] = cos(radX);
  rotX[6] = -sin(radX);
  rotX[9] = sin(radX);
  rotX[10] = cos(radX);

  Mat4 rotY{Mat4::identityMatrix()};
  rotY[0] = cos(radY);
  rotY[2] = sin(radY);
  rotY[8] = -sin(radY);
  rotY[10] = cos(radY);

  Mat4 rotZ{Mat4::identityMatrix()};
  rotZ[0] = cos(radZ);
  rotZ[1] = -sin(radZ);
  rotZ[4] = sin(radZ);
  rotZ[5] = cos(radZ);
  return rotZ *rotY * rotX;
}

Mat4 Mat4::perspectiveMatrix(float fov, float aspectRatio, float zNear, float zFar){
  Mat4 mat{};
  float radians = fov * (3.14159f / 180.0f);
  mat[0] = (1 / tan(radians));
  mat[5] = aspectRatio * mat[0];
  mat[10] = ((zFar +zNear)/(zNear -zFar));
  mat[11] = ((2.0 * zNear * zFar)/(zNear -zFar));
  mat[14] = -1.0;
  return mat;
}

Mat4 Mat4::orthographicMatrix(float left,float right,float top,float bottom,float near,float far){
  Mat4 mat{Mat4::identityMatrix()};
  
  float w = right - left;
  float h = top - bottom;
  float p = far - near;

  float x = (right + left)/w;
  float y = (top + bottom)/h;
  float z = (far + near)/p;

  mat[0]= 2.0f/w;
  mat[5]= 2.0f/h;
  mat[10]= -2.0f/p;
  mat[3]= -x;
  mat[7]= -y;
  mat[11]= -z;

  return mat;
}

Mat4& Mat4::crossProduct(const Mat4& mat){
  //do this code more legible
  //std::unique_ptr<float[]> temp = std::unique_ptr<float[]>(new float[16]);
  auto temp = Mat4{};
  temp[0] = this->elements[0] * mat[0] + this->elements[1] * mat[4] + this->elements[2] * mat[8] + this->elements[3] * mat[12];
    temp[1] = this->elements[0] * mat[1] + this->elements[1] * mat[5] + this->elements[2] * mat[9] + this->elements[3] * mat[13];
    temp[2] = this->elements[0] * mat[2] + this->elements[1] * mat[6] + this->elements[2] * mat[10] + this->elements[3] * mat[14];
    temp[3] = this->elements[0] * mat[3] + this->elements[1] * mat[7] + this->elements[2] * mat[11] + this->elements[3] * mat[15];

    temp[4] = this->elements[4] * mat[0] + this->elements[5] * mat[4] + this->elements[6] * mat[8] + this->elements[7] * mat[12];
    temp[5] = this->elements[4] * mat[1] + this->elements[5] * mat[5] + this->elements[6] * mat[9] + this->elements[7] * mat[13];
    temp[6] = this->elements[4] * mat[2] + this->elements[5] * mat[6] + this->elements[6] * mat[10] + this->elements[7] * mat[14];
    temp[7] = this->elements[4] * mat[3] + this->elements[5] * mat[7] + this->elements[6] * mat[11] + this->elements[7] * mat[15];

    temp[8] = this->elements[8] * mat[0] + this->elements[9] * mat[4] + this->elements[10] * mat[8] + this->elements[11] * mat[12];
    temp[9] = this->elements[8] * mat[1] + this->elements[9] * mat[5] + this->elements[10] * mat[9] + this->elements[11] * mat[13];
    temp[10] = this->elements[8] * mat[2] + this->elements[9] * mat[6] + this->elements[10] * mat[10] + this->elements[11] * mat[14];
    temp[11] = this->elements[8] * mat[3] + this->elements[9] * mat[7] + this->elements[10] * mat[11] + this->elements[11] * mat[15];

    temp[12] = this->elements[12] * mat[0] + this->elements[13] * mat[4] + this->elements[14] * mat[8] + this->elements[15] * mat[12];
    temp[13] = this->elements[12] * mat[1] + this->elements[13] * mat[5] + this->elements[14] * mat[9] + this->elements[15] * mat[13];
    temp[14] = this->elements[12] * mat[2] + this->elements[13] * mat[6] + this->elements[14] * mat[10] + this->elements[15] * mat[14];
    temp[15] = this->elements[12] * mat[3] + this->elements[13] * mat[7] + this->elements[14] * mat[11] + this->elements[15] * mat[15];

    for(int i = 0; i< 16; i++){
      this->elements[i] = temp.elements[i];
    }
  return *this;
}

Mat4& Mat4::operator*=(const Mat4& mat){
  return this->crossProduct(mat);
}

Mat4 Mat4::getTraspose() const{
  Mat4 result{};
  for(int i=0; i < 4; i++){
    for(int j=0; j < 4 ; j++){
      result[i*4+j] = this->elements[j*4+i];
    }
  }
  return result;    
}

Mat4 Mat4::lookAt(const Vec3& eye, const Vec3& target,const Vec3& up){
  Mat4 lookAt{Mat4::identityMatrix()};

  Vec3 zAxis = eye - target;
  zAxis.normalize();
  if(zAxis.length()==0){
    zAxis.setComponent(2,1.0f);
  }

  Vec3 xAxis = cross(up , zAxis);
  xAxis.normalize();
  if(xAxis.length() == 0){
    zAxis -= Vec3{0.0001f,0.0f,0.0f};
    xAxis = cross(up , zAxis);
    xAxis.normalize();
  }

  Vec3 yAxis = cross(zAxis , xAxis);

  //use something instead of getters
  lookAt[0] = xAxis.getX();
  lookAt[4] = yAxis.getX();
  lookAt[8] = zAxis.getX();

  lookAt[1] = xAxis.getY();
  lookAt[5] = yAxis.getY();
  lookAt[9] = zAxis.getY();

  lookAt[2] = xAxis.getZ();
  lookAt[6] = yAxis.getZ();
  lookAt[10] = zAxis.getZ();

  return lookAt;
}

bool Mat4::operator==(const Mat4& mat)const{
  for(int i =0; i< 16; i++){
    bool almostEqual = this->elements[i] < mat[i] + 0.001f;
    almostEqual = almostEqual && this->elements[i] > mat[i] - 0.001f;
    if(!almostEqual){
      return false;
    }
  }
  return true;
}

Mat4& Mat4::print(){
  for(int i = 0; i < 16 ; i++){
    printf("%.2f ",this->elements[i]);
  }
  printf("\n");
  return *this;
}

Mat4 operator*(float scalar,const Mat4& mat){
  Mat4 res{mat};
  return res *= scalar;
}

Mat4 operator*(const Mat4& m1, const Mat4& m2){
  return cross(m2,m1);
}

Mat4 cross(const Mat4& m1, const Mat4& m2){
  Mat4 res{m2};
  return res.crossProduct(m1);
}
}