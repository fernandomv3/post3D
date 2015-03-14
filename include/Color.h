#ifndef COLOR_H
#define COLOR_H
#include <memory>
using namespace std;
namespace math3D{
	class Color{
	private:
		float r;
		float g;
		float b;
		float a;
	public:
		Color(float r, float g, float b, float a);
		Color()=default;
		Color(const Color& color)=default;
		Color(Color&& color)=default;
		Color& operator=(const Color& color)=default;
		Color& operator=(Color&& color)=default;
		~Color()=default;
		Color& setRGBA255(int r = 0, int g = 0, int b = 0, int a = 1);
		Color& setRGBA(float r = 0.0, float g = 0.0, float b = 0.0, float a = 1.0);
		Color& setComponent(char component, float value);
		Color& operator+=(const Color& color);
		Color& operator-=(const Color& color);
		bool operator==(const Color& color) const;
		unique_ptr<float[]> getAsArray() const;
		float getComponent(const char component) const;
	};

	Color operator+(const Color& c1,const Color& c2);
	Color operator-(const Color& c1,const Color& c2);
}

#endif