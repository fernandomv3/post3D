#include "post3D/Mesh.h"

namespace object3D{

	Mesh::Mesh():Object3D(){
		this->geometry = shared_ptr<Geometry>();
		this->material = shared_ptr<Material>();
		this->castShadows = true;
	}

	Mesh::Mesh(shared_ptr<Geometry> geometry):Object3D(){
		this->geometry = geometry;
		this->material = shared_ptr<Material>();
		this->castShadows = true;
	}

	Mesh::Mesh(shared_ptr<Geometry> geometry, shared_ptr<Material> material):Object3D(){
		this->geometry = geometry;
		this->material = material;
		this->castShadows = true;
	}

	Mesh::Mesh(const Mesh& mesh):Object3D(mesh){
		this->geometry = mesh.geometry;
		this->material = mesh.material;
		this->castShadows = mesh.castShadows;
	}

	Mesh::Mesh(Mesh&& mesh):Object3D(mesh){
		this->geometry = move(mesh.geometry);
		this->material = move(mesh.material);
		this->castShadows = mesh.castShadows;
	}

	Mesh& Mesh::operator=(const Mesh& mesh){
		Object3D::operator=(mesh);
		this->geometry = mesh.geometry;
		this->material = mesh.material;
		this->castShadows = mesh.castShadows;
		return *this;
	}

	Mesh& Mesh::operator=(Mesh&& mesh){
		Object3D::operator=(move(mesh));
		this->geometry = move(mesh.geometry);
		this->material = move(mesh.material);
		this->castShadows = mesh.castShadows;
		return *this;
	}

/*	Mesh::~Mesh(){

	}*/

	shared_ptr<Geometry> Mesh::getGeometry()const{
		return this->geometry;
	}

	shared_ptr<Material> Mesh::getMaterial()const{
		return this->material;
	}

	Mesh& Mesh::setGeometry(shared_ptr<Geometry> geometry){
		this->geometry = geometry;
		return *this;
	}

	Mesh& Mesh::setMaterial(shared_ptr<Material> material){
		this->material = material;
		return *this;
	}

	bool Mesh::getCastShadows()const{
		return this->castShadows;
	}

	Mesh& Mesh::setCastShadows(bool castShadows){
		this->castShadows = castShadows;
		return *this;
	}


}