#ifndef CAMERA_H
#define CAMERA_H
#include "Mat4.h"
#include "Vec3.h"
#include "Object3D.h"
#include <memory>
using namespace std;
using namespace object3D;
namespace scene{
	class Camera : public Object3D{
	private:
		shared_ptr<Mat4> projectionMatrix;
		shared_ptr<Mat4> worldMatrix;
		shared_ptr<Vec3> target;
		int matricesUBO;
		float gamma;
	public:
		Camera();
		Camera(const Camera& cam);
		Camera(Camera&& cam);
		Camera& operator=(const Camera& cam);
		Camera& operator=(Camera&& cam);
		~Camera()=default;
		shared_ptr<Mat4> getProjectionMatrix()const;
		Camera& setProjectionMatrix(shared_ptr<Mat4> mat);
		shared_ptr<Mat4> getWorldMatrix()const;
		Camera& setWorldMatrix(shared_ptr<Mat4> mat);
		Camera& updateWorldMatrix();
		int getMatricesUBO()const;
		Camera& setMatricesUBO(int ubo);
		shared_ptr<float> getMatricesArray()const;
		shared_ptr<Vec3> getTarget()const;
		Camera& setTarget(shared_ptr<Vec3> target);
		float getGamma()const;
		Camera& setGamma(float gamma);
		Mat4 lookAt() const;
	};
}
#endif