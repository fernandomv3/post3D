#include "post3D/Geometry.h"
#include "assimp/Importer.hpp"
#include "assimp/scene.h"
#include "assimp/mesh.h"
#include "assimp/postprocess.h"
#include "GL/glew.h"


namespace object3D{
	Geometry::Geometry(){

		//zero means not created?
		this->vertexBuffer = 0;
		this->elementBuffer = 0;
		this->normalBuffer = 0;
		this->texCoordBuffer = 0;
		this->tangentBuffer = 0;

		this->initialized = false;

		this->vertices = vector<float>();
		this->elements = vector<int>();
		this->normals = vector<float>();
		this->texCoords = vector<float>();
		this->tangents = vector<float>();
		this->bitangents = vector<float>();
	}

	Geometry::~Geometry(){
		int buffers[5] = {
			this->vertexBuffer,
			this->elementBuffer,
			this->normalBuffer,
			this->texCoordBuffer,
			this->tangentBuffer
		};
		glDeleteBuffers(5,(GLuint*)(&buffers));
	}
/*
	Geometry::Geometry(const Geometry& geom){

	}

	Geometry::Geometry(Geometry&& geom){

	}

	Geometry& Geometry::operator=(const Geometry& geom){

	}

	Geometry& Geometry::operator=(Geometry&& geom){

	}
*/
	const vector<float>& Geometry::getVertices()const{
		return this->vertices;
	}


	const vector<int>& Geometry::getElements()const{
		return this->elements;
	}


	const vector<float>& Geometry::getNormals()const{
		return this->normals;
	}


	const vector<float>& Geometry::getTexCoords()const{
		return this->texCoords;
	}


	const vector<float>& Geometry::getTangents()const{
		return this->tangents;
	}


	const vector<float>& Geometry::getBitangents()const{
		return this->bitangents;
	}


	Geometry& Geometry::setVertices(vector<float> vertices){
		this->vertices = vertices;
		return *this;
	}

	Geometry& Geometry::setElements(vector<int> elements){
		this->elements = elements;
		return *this;
	}

	Geometry& Geometry::setNormals(vector<float> normals){
		this->normals = normals;
		return *this;
	}

	Geometry& Geometry::setTexCoords(vector<float> texCoords){
		this->texCoords = texCoords;
		return *this;
	}

	Geometry& Geometry::setTangents(vector<float> tangents){
		this->tangents = tangents;
		return *this;
	}

	Geometry& Geometry::setBitangents(vector<float> bitangents){
		this->bitangents = bitangents;
		return *this;
	}

	bool Geometry::isInitialized()const{
		return this->initialized;
	}

	Geometry& Geometry::setInitialized(bool initialized){
		this->initialized = initialized;
		return *this;
	}


	int Geometry::getVertexBuffer()const{
		return this->vertexBuffer;
	}

	int Geometry::getElementBuffer()const{
		return this->elementBuffer;
	}

	int Geometry::getNormalBuffer()const{
		return this->normalBuffer;
	}

	int Geometry::getTexCoordBuffer()const{
		return this->texCoordBuffer;
	}

	int Geometry::getTangentBuffer()const{
		return this->tangentBuffer;
	}

	Geometry& Geometry::setVertexBuffer(int vertexBuffer){
		this->vertexBuffer = vertexBuffer;
		return *this;
	}

	Geometry& Geometry::setElementBuffer(int elementBuffer){
		this->elementBuffer = elementBuffer;
		return *this;
	}

	Geometry& Geometry::setNormalBuffer(int normalBuffer){
		this->normalBuffer = normalBuffer;
		return *this;
	}

	Geometry& Geometry::setTexCoordBuffer(int texCoordBuffer){
		this->texCoordBuffer = texCoordBuffer;
		return *this;
	}

	Geometry& Geometry::setTangentBuffer(int tangentBuffer){
		this->tangentBuffer = tangentBuffer;
		return *this;
	}

	Geometry loadDataFromFile(string filename){
		Geometry result = Geometry();
		Assimp::Importer importer;
		const aiScene* scene = importer.ReadFile(filename,
			aiProcess_Triangulate |
			aiProcess_CalcTangentSpace 
		);
		aiMesh *mesh = scene->mMeshes[0];
		for(unsigned int i = 0;i<mesh->mNumFaces;i++){
			const aiFace& face = mesh->mFaces[i];
			for(int j=0; j <3 ; j++){
				int ind = face.mIndices[j];
				aiVector3D vert = mesh->mVertices[ind];
				result.vertices.push_back(vert.x);
				result.vertices.push_back(vert.y);
				result.vertices.push_back(vert.z);
				aiVector3D norm = mesh->mNormals[ind];
				result.normals.push_back(norm.x);
				result.normals.push_back(norm.y);
				result.normals.push_back(norm.z);
				aiVector3D uv = mesh->mTextureCoords[0][ind];
				result.texCoords.push_back(uv.x);
				result.texCoords.push_back(1 - uv.y);
				aiVector3D tang = mesh->mTangents[ind];
				result.tangents.push_back(tang.x);
				result.tangents.push_back(tang.y);
				result.tangents.push_back(tang.z);
			}
		}
		return result;
	}

	Geometry generateQuadGeometry(int size){
		float vertices[18] = {
			-1.0, -1.0, 0.0,
    		1.0, -1.0, 0.0,
    		-1.0,  1.0, 0.0,
    		-1.0,  1.0, 0.0,
    		1.0, -1.0, 0.0,
    		1.0,  1.0, 0.0
		};
		Geometry geom = Geometry();
		geom.setVertices(vector<float>(vertices,vertices+18));
		return geom;
	}

	Geometry generateCubeGeometry(int size){
		return Geometry();
	}

	Geometry generateCubeWireframe(int size){
		return Geometry();
	}

}