#include "Object3D.h"

namespace object3D{
	Object3D::Object3D(){
		this->visible = true;
		this->position = shared_ptr<Vec3>(new Vec3(0.0f,0.0f,0.0f));
		this->rotation = shared_ptr<Euler>(new Euler(0.0f,0.0f,0.0f,"XYZ"));
		this->quaternion = shared_ptr<Quaternion>(new Quaternion(0.0f,0.0f,0.0f,1.0f));
		this->scale = shared_ptr<Vec3>(new Vec3(1.0f,1.0f,1.0f));
		this->modelMatrix = shared_ptr<Mat4>(new Mat4(Mat4::identityMatrix()));
		this->rotation->setQuaternion(this->quaternion);
		this->quaternion->setEuler(this->rotation);
		this->parent = weak_ptr<Object3D>();
		this->distanceToCamera = 1.0f;
	}

	Object3D::Object3D(const Object3D& object){
		this->visible = object.visible;
		this->position = shared_ptr<Vec3>(new Vec3(*object.position));
		this->rotation = shared_ptr<Euler>(new Euler(*object.rotation));
		this->quaternion = shared_ptr<Quaternion>(new Quaternion(*object.quaternion));
		this->scale = shared_ptr<Vec3>(new Vec3(*object.scale));
		this->modelMatrix = shared_ptr<Mat4>(new Mat4(*object.modelMatrix));
		this->parent = object.parent;
		this->distanceToCamera = object.distanceToCamera;
	}

	Object3D::Object3D(Object3D&& object){
		this->visible = object.visible;
		this->position = move(object.position);
		this->rotation = move(object.rotation);
		this->quaternion = move(object.quaternion);
		this->scale = move(object.scale);
		this->modelMatrix = move(object.modelMatrix);
		this->parent = object.parent;
		this->distanceToCamera = object.distanceToCamera;
	}

	Object3D& Object3D::operator=(const Object3D& object){
		this->visible = object.visible;
		this->position = shared_ptr<Vec3>(new Vec3(*object.position));
		this->rotation = shared_ptr<Euler>(new Euler(*object.rotation));
		this->quaternion = shared_ptr<Quaternion>(new Quaternion(*object.quaternion));
		this->scale = shared_ptr<Vec3>(new Vec3(*object.scale));
		this->modelMatrix = shared_ptr<Mat4>(new Mat4(*object.modelMatrix));
		this->parent = object.parent;
		this->distanceToCamera = object.distanceToCamera;
		return *this;
	}

	Object3D& Object3D::operator=(Object3D&& object){
		this->visible = object.visible;
		this->position = move(object.position);
		this->rotation = move(object.rotation);
		this->quaternion = move(object.quaternion);
		this->scale = move(object.scale);
		this->modelMatrix = move(object.modelMatrix);
		this->parent = object.parent;
		this->distanceToCamera = object.distanceToCamera;
		return *this;
	}

	shared_ptr<Vec3> Object3D::getPosition()const{
		return this->position;
	}

	shared_ptr<Euler> Object3D::getRotation()const{
		return this->rotation;
	}

	shared_ptr<Vec3> Object3D::getScale()const{
		return this->scale;
	}

	Object3D& Object3D::setPosition(shared_ptr<Vec3> position){
		this->position = position;
		return *this;
	}

	Object3D& Object3D::setRotation(shared_ptr<Euler> rotation){
		this->rotation = rotation;
		//delete previous quaternion in new euler if exists
		rotation->setQuaternion(this->quaternion);

		//delete the previous euler in the Object's quaternion if exists
		rotation->getQuaternion()->setFromEuler(*rotation,false);
		//this->quaternion->setEuler(rotation);
		return *this;
	}

	Object3D& Object3D::setScale(shared_ptr<Vec3> scale){
		this->scale = scale;
		return *this;
	}

	shared_ptr<Mat4> Object3D::getModelMatrix()const{
		return this->modelMatrix;
	}

	Object3D& Object3D::updateModelMatrix(){
		Mat4 translation = Mat4::translationMatrix(
			this->getPosition()->getX(),
			this->getPosition()->getY(),
			this->getPosition()->getZ());

		Mat4 rot = Mat4::rotationMatrixFromQuaternion(
			*this->getQuaternion());

		Mat4 scale = Mat4::scaleMatrix(
			this->getScale()->getX(),
			this->getScale()->getY(),
			this->getScale()->getZ());

		Mat4 res = Mat4::identityMatrix();
		auto p = this->parent.lock();
		if(p){
			p->updateModelMatrix();
			res = *(p->modelMatrix) * res;
		}
		res = res * translation * rot * scale;
		//res = scale * rot * translation * res;
		this->modelMatrix.reset(new Mat4(res));

		return *this;
	}

	Object3D& Object3D::setQuaternion(shared_ptr<Quaternion> quaternion){
		this->quaternion = quaternion;
		//delete previous euler in new quaternion if exists
		quaternion->setEuler(this->rotation);
		//quaternion->euler->setFromQuaternion(quaternion,this->rotation->getOrder(),false);
		this->rotation->setQuaternion(quaternion);
		return *this;
	}

	shared_ptr<Quaternion> Object3D::getQuaternion(){
		return this->quaternion;
	}

	bool Object3D::getVisible()const{
		return this->visible;
	}

	Object3D& Object3D::setVisible(bool visible){
		this->visible = visible;
		return *this;
	}

	shared_ptr<Object3D> Object3D::getParent()const{
		return this->parent.lock();
	}

	Object3D& Object3D::setParent(shared_ptr<Object3D> parent){
		this->parent = weak_ptr<Object3D>(parent);
		return *this;
	}

	float Object3D::getDistanceToCamera()const{
		return this->distanceToCamera;
	}

	Object3D& Object3D::setDistanceToCamera(float distanceToCamera){
		this->distanceToCamera = distanceToCamera;
		return *this;
	}

}