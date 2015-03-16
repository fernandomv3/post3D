#ifndef EULER_H
#define EULER_H
#include <memory>
#include <string>
using namespace std;
namespace math3D{
	class Quaternion;

	class Euler{
	private:
		float x;
		float y;
		float z;
		string order;
		weak_ptr<Quaternion> quaternion;

	public:
		Euler();
		Euler(float x, float y, float z, const string order);
		Euler(float x, float y, float z);
		Euler(const Euler& euler);
		Euler(Euler&& euler);
		Euler& operator=(const Euler& euler);
		Euler& operator=(Euler&& euler);
		~Euler()=default;
		string getOrder()const;
		float getX()const;
		Euler& setX(float x);
		float getY()const;
		Euler& setY(float y);
		float getZ()const;
		Euler& setZ(float z);
		Euler& setFromQuaternion(Quaternion& q, const string order, bool update = true);
		Euler& updateQuaternion();
		Euler& setQuaternion(shared_ptr<Quaternion> q);
		shared_ptr<Quaternion> getQuaternion()const;
	};
	float clamp(float num);
}
#endif