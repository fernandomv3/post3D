#include "post3D/Camera.h"
#include <iostream>

namespace scene{
std::shared_ptr<Mat4> Camera::getProjectionMatrix()const{
  return this->projectionMatrix;
}

Camera& Camera::setProjectionMatrix(std::shared_ptr<Mat4> mat){
  this->projectionMatrix = mat;
  return *this;
}

std::shared_ptr<Mat4> Camera::getWorldMatrix()const{
  return this->worldMatrix;
}

Camera& Camera::setWorldMatrix(std::shared_ptr<Mat4> mat){
  this->worldMatrix = mat;
  return *this;
}

Camera::Camera():Object3D(){
  this->projectionMatrix = std::shared_ptr<Mat4>(new Mat4());
  this->worldMatrix = std::shared_ptr<Mat4>(new Mat4());
  this->target = std::shared_ptr<Vec3>();
  this->gamma = 2.2f;
}

Camera::Camera(const Camera& cam):Object3D(cam){
  this->projectionMatrix = cam.projectionMatrix;
  this->worldMatrix = cam.worldMatrix;
  this->target = cam.target;
}

Camera::Camera(Camera&& cam):Object3D(cam){
  this->projectionMatrix = std::move(cam.projectionMatrix);
  this->worldMatrix = std::move(cam.worldMatrix);
  this->target = std::move(cam.target);
}

Camera& Camera::operator=(const Camera& cam){
  Object3D::operator=(cam);
  this->projectionMatrix = cam.projectionMatrix;
  this->worldMatrix = cam.worldMatrix;
  this->target = cam.target;
  return *this;
}

Camera& Camera::operator=(Camera&& cam){
  Object3D::operator=(std::move(cam));
  this->projectionMatrix = std::move(cam.projectionMatrix);
  this->worldMatrix = std::move(cam.worldMatrix);
  this->target = std::move(cam.target);
  return *this;
}

std::shared_ptr<float> Camera::getMatricesArray()const{
  float matrices[32];
  Mat4 worldTraspose = this->worldMatrix->getTraspose();
  Mat4 projectionTraspose = this->projectionMatrix->getTraspose();
  for(int i=0; i < 16; i++){
    matrices[i]= worldTraspose[i];
    matrices[i+16]= projectionTraspose[i];
  }
  return std::shared_ptr<float>(matrices);
}

Camera& Camera::updateWorldMatrix(){
  //this->updateModelMatrix();
  Mat4 translation = Mat4::translationMatrix(
    this->getPosition()->getX() * -1,
    this->getPosition()->getY() * -1,
    this->getPosition()->getZ() * -1);
  Mat4 rot;
  if(!this->target){
    Quaternion q = inverse(*this->getQuaternion());
    rot = Mat4::rotationMatrixFromQuaternion(q);
  }
  else{
    rot = this->lookAt();
  }

  Mat4 scale = Mat4::scaleMatrix(
    1 / this->getScale()->getX(),
    1 / this->getScale()->getY(),
    1 / this->getScale()->getZ());
  Mat4 res = Mat4::identityMatrix() * scale * rot * translation;
  this->worldMatrix = std::shared_ptr<Mat4>(new Mat4(res));
  return *this;
}

std::shared_ptr<Vec3> Camera::getTarget()const{
  return this->target;
}

Camera& Camera::setTarget(std::shared_ptr<Vec3> target){
  this->target = target;
  return *this;
}

float Camera::getGamma()const{
  return this->gamma;
}

Camera& Camera::setGamma(float gamma){
  this->gamma = gamma;
  return *this;
}

Mat4 Camera::lookAt()const{
  Vec3 up = Vec3(0.0,1.0,0.0);
  Mat4 rot = Mat4::lookAt(*(this->getPosition()), *(this->target),up);
  return rot;
}
}