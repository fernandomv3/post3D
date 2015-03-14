#ifndef VEC3_H
#define VEC3_H 
#include <memory>
using namespace std;

/*Should be a Vec4 class with w component*/
namespace math3D{
	class Mat4;
	
	class Vec3{
	private:
		float x;
		float y;
		float z;
		float w;
	public:
		Vec3()=default;
		Vec3(float x, float y, float z);
		~Vec3()=default;
		Vec3(const Vec3& v)=default;
		Vec3(Vec3&& v)=default;
		Vec3& operator=(const Vec3& v)=default;
		Vec3& operator=(Vec3&& v)=default;

		//setters & getters soon to be deprecated
		Vec3& setX(float x);
		Vec3& setY(float y);
		Vec3& setZ(float z);
		float getX()const;
		float getY()const;
		float getZ()const;

		Vec3& setW(float w);
		float getW()const;
		
		Vec3& setComponent(int index, float value);
		float getComponent(int index) const;
		Vec3& setFromArray(float value[],int len);

		//overload the next methods
		Vec3& applyMatrix(const Mat4& matrix, bool normalize = false);
		Vec3& crossProduct(const Vec3& vec);
		float dotProduct(const Vec3& vec) const;
		Vec3& operator+=(const Vec3& v);
		Vec3& operator-=(const Vec3& v);

		Vec3& normalize();
		float length() const;
		bool insideUnitCube()const;
		unique_ptr<float[]> getAsArray()const;
	};

	float distance(const Vec3& v1, const Vec3& v2);
	float dot(const Vec3& v1, const Vec3& v2);
	Vec3 cross(const Vec3& v1, const Vec3& v2);
	Vec3 operator+(const Vec3& v1, const Vec3& v2);
	Vec3 operator-(const Vec3& v1, const Vec3& v2);
	Vec3 operator*(const Mat4& mat, const Vec3& v);
}
#endif