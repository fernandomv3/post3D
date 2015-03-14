#ifndef OBJECT3D_H
#define OBJECT3D_H
#include "Vec3.h"
#include "Mat4.h"
#include "Quaternion.h"
#include "Euler.h"
#include <memory>
using namespace std;
using namespace math3D;
namespace object3D{
	class Object3D{
	private:
		shared_ptr<Vec3> position;
		shared_ptr<Euler> rotation;
		shared_ptr<Quaternion> quaternion;
		shared_ptr<Vec3> scale;
		shared_ptr<Mat4> modelMatrix;
		bool visible;
		weak_ptr<Object3D> parent;
		float distanceToCamera;
	public:
		Object3D();
		Object3D(const Object3D& object);
		Object3D(Object3D&& object);
		Object3D& operator=(const Object3D& object);
		Object3D& operator=(Object3D&& object);
		virtual ~Object3D()=default;
		shared_ptr<Vec3> getPosition()const;
		shared_ptr<Euler> getRotation()const;
		shared_ptr<Vec3> getScale()const;
		Object3D& setPosition(shared_ptr<Vec3> position);
		Object3D& setRotation(shared_ptr<Euler> rotation);
		Object3D& setScale(shared_ptr<Vec3> scale);
		shared_ptr<Mat4> getModelMatrix()const;
		Object3D& updateModelMatrix();
		Object3D& setQuaternion(shared_ptr<Quaternion> quaternion);
		shared_ptr<Quaternion> getQuaternion();
		bool getVisible()const;
		Object3D& setVisible(bool visible);
		shared_ptr<Object3D> getParent()const;
		Object3D& setParent(shared_ptr<Object3D> parent);
		float getDistanceToCamera()const;
		Object3D& setDistanceToCamera(float distanceToCamera);
	};
}
#endif