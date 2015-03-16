#include <string>
#include <cmath>
#include "post3D/Euler.h"
#include "post3D/Quaternion.h"
namespace math3D{

		Euler::Euler(){
			this->x = 0.0f;
			this->y = 0.0f;
			this->z = 0.0f;
			this->order = string("XYZ",3);
		}

		Euler::Euler(float x, float y, float z, const string order){
			this->x = x;
			this->y = y;
			this->z = z;
			this->order = order;
		}

		Euler::Euler(float x, float y, float z){
			this->x = x;
			this->y = y;
			this->z = z;
			this->order = string("XYZ",3);
		}

		Euler::Euler(const Euler& euler){
			this->x =  euler.x;
			this->y = euler.y;
			this->z = euler.z;
			this->order = euler.order;
			this->quaternion = euler.quaternion;
		}

		Euler::Euler(Euler&& euler){
			this->x =  euler.x;
			this->y = euler.y;
			this->z = euler.z;
			this->order = euler.order;
			this->quaternion = move(euler.quaternion);
		}

		Euler& Euler::operator=(const Euler& euler){
			this->x =  euler.x;
			this->y = euler.y;
			this->z = euler.z;
			this->order = euler.order;
			this->quaternion = euler.quaternion;
			return *this;
		}

		Euler& Euler::operator=(Euler&& euler){
			this->x =  euler.x;
			this->y = euler.y;
			this->z = euler.z;
			this->order = euler.order;
			this->quaternion = move(euler.quaternion);
			return *this;
		}

		string Euler::getOrder()const{
			return this->order;
		}

		float Euler::getX()const{
			return this->x;
		}

		Euler& Euler::setX(float x){
			this->x = x;
			this->updateQuaternion();
			return *this;
		}

		float Euler::getY()const{
			return this->y;
		}

		Euler& Euler::setY(float y){
			this->y = y;
			this->updateQuaternion();
			return *this;
		}

		float Euler::getZ()const{
			return this->z;
		}

		Euler& Euler::setZ(float z){
			this->z = z;
			this->updateQuaternion();
			return *this;
		}

		Euler& Euler::setFromQuaternion(Quaternion& q, const string order, bool update){
			float x2 = q.getX() * q.getX(), 
			      y2 = q.getY() * q.getY(), 
			      z2 = q.getZ() * q.getZ(), 
			      w2 = q.getW() * q.getW();

			if (order == string("XYZ")){
				this->x = atan2( 2 * ( q.getX() * q.getW() - q.getY() * q.getZ() ), ( w2 - x2 - y2 + z2 ) );
		        this->y = asin(  clamp( 2 * ( q.getX() * q.getZ() + q.getY() * q.getW() ) ) );
		        this->z = atan2( 2 * ( q.getZ() * q.getW() - q.getX() * q.getY() ), ( w2 + x2 - y2 - z2 ) );

			}
			else if (order == string("YXZ")){
				this->x = asin(  clamp( 2 * ( q.getX() * q.getW() - q.getY() * q.getZ() ) ) );
		        this->y = atan2( 2 * ( q.getX() * q.getZ() + q.getY() * q.getW() ), ( w2 - x2 - y2 + z2 ) );
		        this->z = atan2( 2 * ( q.getX() * q.getY() + q.getZ() * q.getW() ), ( w2 - x2 + y2 - z2 ) );
			}
			else if (order == string("ZXY")){
				this->x = asin(  clamp( 2 * ( q.getX() * q.getW() + q.getY() * q.getZ() ) ) );
				this->y = atan2( 2 * ( q.getY() * q.getW() - q.getZ() * q.getX() ), ( w2 - x2 - y2 + z2 ) );
				this->z = atan2( 2 * ( q.getZ() * q.getW() - q.getX() * q.getY() ), ( w2 - x2 + y2 - z2 ) );
			}
			else if (order == string("ZYX")){
				this->x = atan2( 2 * ( q.getX() * q.getW() + q.getZ() * q.getY() ), ( w2 - x2 - y2 + z2 ) );
		        this->y = asin(  clamp( 2 * ( q.getY() * q.getW() - q.getX() * q.getZ() ) ) );
		        this->z = atan2( 2 * ( q.getX() * q.getY() + q.getZ() * q.getW() ), ( w2 + x2 - y2 - z2 ) );
			}
			else if (order == string("YZX")){
				this->x = atan2( 2 * ( q.getX() * q.getW() - q.getZ() * q.getY() ), ( w2 - x2 + y2 - z2 ) );
		        this->y = atan2( 2 * ( q.getY() * q.getW() - q.getX() * q.getZ() ), ( w2 + x2 - y2 - z2 ) );
		        this->z = asin(  clamp( 2 * ( q.getX() * q.getY() + q.getZ() * q.getW() ) ) );
			}
			else if (order == string("XZY")){
				this->x = atan2( 2 * ( q.getX() * q.getW() + q.getY() * q.getZ() ), ( w2 - x2 + y2 - z2 ) );
		        this->y = atan2( 2 * ( q.getX() * q.getZ() + q.getY() * q.getW() ), ( w2 + x2 - y2 - z2 ) );
		        this->z = asin(  clamp( 2 * ( q.getZ() * q.getW() - q.getX() * q.getY() ) ) );
			}


			this->order = order;
			if(update){
				this->updateQuaternion();
			}
			return *this;
		}

		Euler& Euler::updateQuaternion(){
			auto p = this->quaternion.lock();
			if(p){
				p->setFromEuler(*this , false);
			}
			return *this;
		}

		shared_ptr<Quaternion> Euler::getQuaternion()const{
			return this->quaternion.lock();
		}

		Euler& Euler::setQuaternion(shared_ptr<Quaternion> q){
			this->quaternion = weak_ptr<Quaternion>{q};
			return *this;
		}

		float clamp(float num){
			return min(max(num,-1.0f),1.0f);
		}
}