#ifndef LIGHT_H
#define LIGHT_H
#include <memory>
#include "Object3D.h"
#include "Color.h"

using namespace std;
using namespace math3D;
using namespace object3D;

namespace light{
	class Light : public Object3D {
	private:
		shared_ptr<Color> color;
	public:
		Light();
		Light(const Light& light);
		Light(Light&& light);
		Light& operator=(const Light& light);
		Light& operator=(Light&& light);
		virtual ~Light()=default;
		shared_ptr<Color> getColor() const;
		Light& setColor(shared_ptr<Color> color);
	};
}
#endif