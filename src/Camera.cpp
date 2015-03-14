#include "Camera.h"
#include <iostream>
using namespace std;
namespace scene{
	shared_ptr<Mat4> Camera::getProjectionMatrix()const{
		return this->projectionMatrix;
	}

	Camera& Camera::setProjectionMatrix(shared_ptr<Mat4> mat){
		this->projectionMatrix = mat;
		return *this;
	}

	shared_ptr<Mat4> Camera::getWorldMatrix()const{
		return this->worldMatrix;
	}

	Camera& Camera::setWorldMatrix(shared_ptr<Mat4> mat){
		this->worldMatrix = mat;
		return *this;
	}

	int Camera::getMatricesUBO()const{
		return this->matricesUBO;
	}

	Camera& Camera::setMatricesUBO(int ubo){
		this->matricesUBO = ubo;
		return *this;
	}

	Camera::Camera():Object3D(){
		this->projectionMatrix = shared_ptr<Mat4>(new Mat4());
		this->worldMatrix = shared_ptr<Mat4>(new Mat4());
		this->matricesUBO = 0;
		this->target = shared_ptr<Vec3>();
		this->gamma = 2.2f;
	}

	Camera::Camera(const Camera& cam):Object3D(cam){
		this->projectionMatrix = cam.projectionMatrix;
		this->worldMatrix = cam.worldMatrix;
		this->matricesUBO = cam.matricesUBO;
		this->target = cam.target;
	}

	Camera::Camera(Camera&& cam):Object3D(cam){
		this->projectionMatrix = move(cam.projectionMatrix);
		this->worldMatrix = move(cam.worldMatrix);
		this->matricesUBO = cam.matricesUBO;
		this->target = move(cam.target);
	}

	Camera& Camera::operator=(const Camera& cam){
		Object3D::operator=(cam);
		this->projectionMatrix = cam.projectionMatrix;
		this->worldMatrix = cam.worldMatrix;
		this->matricesUBO = cam.matricesUBO;
		this->target = cam.target;
		return *this;
	}

	Camera& Camera::operator=(Camera&& cam){
		Object3D::operator=(move(cam));
		this->projectionMatrix = move(cam.projectionMatrix);
		this->worldMatrix = move(cam.worldMatrix);
		this->matricesUBO = cam.matricesUBO;
		this->target = move(cam.target);
		return *this;
	}

	shared_ptr<float> Camera::getMatricesArray()const{
		float matrices[32];
		Mat4 worldTraspose = this->worldMatrix->getTraspose();
		Mat4 projectionTraspose = this->projectionMatrix->getTraspose();
		for(int i=0; i < 16; i++){
			matrices[i]= worldTraspose[i];
			matrices[i+16]= projectionTraspose[i];
		}
		return shared_ptr<float>(matrices);
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
		this->worldMatrix = shared_ptr<Mat4>(new Mat4(res));
		return *this;
	}

	shared_ptr<Vec3> Camera::getTarget()const{
		return this->target;
	}

	Camera& Camera::setTarget(shared_ptr<Vec3> target){
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