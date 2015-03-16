#ifndef MAT4_H
#define MAT4_H
#include <cmath>
#include <memory>
#include <vector>

using namespace std;
namespace math3D{
	class Quaternion;
	class Vec3;

	class Mat4{
	private:
		vector<float> elements;
	public:
		Mat4();
		Mat4(float value);
		Mat4(const float elements[16]);
		~Mat4()=default;
		Mat4(const Mat4& mat);
		Mat4(Mat4&& mat);
		Mat4& operator=(const Mat4& mat);
		Mat4& operator=(Mat4&& mat);

		Mat4 getTraspose() const;
		unique_ptr<float[]> getAsArray() const;
		
		static Mat4 identityMatrix();
		static Mat4 translationMatrix(float x, float y , float z);
		static Mat4 rotationMatrixFromQuaternion(const Quaternion& q);
		static Mat4 lookAt(const Vec3& eye, const Vec3& target,const Vec3& up);
		static Mat4 scaleMatrix(float x, float y , float z);
		static Mat4 rotationMatrix(float x , float y, float z);
		static Mat4 perspectiveMatrix(float fov, float aspectRatio, float zNear, float zFar);
		static Mat4 orthographicMatrix(float left,float right,float top,float bottom,float near,float far);
		
		Mat4& operator*=(float scalar);
		Mat4& operator*=(const Mat4& mat);
		Mat4& crossProduct(const Mat4& mat);
		const float& operator[](int index) const;
		float& operator[](int index);
		bool operator==(const Mat4& mat)const;
		Mat4& print();
	};
	
	Mat4 operator*(float scalar,const Mat4& mat);
	Mat4 operator*(const Mat4& m1, const Mat4& m2);
	Mat4 cross(const Mat4& m1, const Mat4& m2);
}
#endif