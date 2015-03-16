#include "post3D/SphericalCoord.h"
#include <cmath>
using namespace std;
namespace math3D{
	const float SphericalCoord::MAXANG = 3.1415912f;
	const float SphericalCoord::MINANG = 0.0f;
	const float SphericalCoord::EPS = 0.000001f;
	SphericalCoord::SphericalCoord(const Vec3& cartesian,const Vec3& relOrigin){
		this->setFromCartesian(cartesian,relOrigin);
	}

	float SphericalCoord::getPhi()const{
		return this->phi;
	}

	float SphericalCoord::getTheta()const{
		return this->theta;
	}

	float SphericalCoord::getR()const{
		return this->r;
	}

	SphericalCoord& SphericalCoord::setPhi(float phi){
		this->phi = phi;
		return *this;
	}

	SphericalCoord& SphericalCoord::setTheta(float theta){
		this->theta = theta;
		return *this;
	}

	SphericalCoord& SphericalCoord::setR(float r){
		this->r = r;
		return *this;
	}

	SphericalCoord& SphericalCoord::setFromCartesian(const Vec3& position,const Vec3& relOrigin){
		Vec3 sub = position - relOrigin;
		float x2z2 = sqrt((sub.getX()*sub.getX())+(sub.getZ()*sub.getZ()));
		this->theta = atan2(sub.getX(),sub.getZ());
		this->phi = atan2(x2z2,sub.getY());
		this->r = sub.length();
		return *this;
	}

	Vec3 SphericalCoord::getCartesian(const Vec3& relOrigin)const{
		Vec3 position{};
		position.setX(r * sin(this->phi) * sin(this->theta) + relOrigin.getX());
		position.setY(r * cos(this->phi) + relOrigin.getY());
		position.setZ(r * sin(this->phi) * cos(this->theta) + relOrigin.getZ());
		return position;
	}

	Vec3 orbit(const Vec3& origin, const Vec3& position, float deltaPhi, float deltaTheta, float radiusFactor){
		SphericalCoord s{position,origin};
		float r = s.getR() * radiusFactor;
		s.setR(max(0.2f,min(99.0f,r)));
		float phi = s.getPhi() + deltaPhi;
		phi = max(SphericalCoord::MINANG,min(SphericalCoord::MAXANG,phi));
		s.setPhi(max(SphericalCoord::EPS,min(SphericalCoord::MAXANG - SphericalCoord::EPS,phi)));
		s.setTheta( s.getTheta() + deltaTheta);
		return s.getCartesian(origin);
	}

}