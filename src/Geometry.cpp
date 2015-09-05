#include <string>
#include "post3D/Geometry.h"
#include "assimp/Importer.hpp"
#include "assimp/scene.h"
#include "assimp/mesh.h"
#include "assimp/postprocess.h"
#include "GL/glew.h"


namespace object3D{
Geometry::Geometry(){
  //zero means not created?
  this->uuid = sole::uuid1().str();

  this->initialized = false;

  this->vertices = std::vector<float>();
  this->elements = std::vector<int>();
  this->normals = std::vector<float>();
  this->texCoords = std::vector<float>();
  this->tangents = std::vector<float>();
  this->bitangents = std::vector<float>();
}

Geometry::~Geometry(){
  
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

std::string Geometry::getUUID()const{
  return this->uuid;
}

const std::vector<float>& Geometry::getVertices()const{
  return this->vertices;
}


const std::vector<int>& Geometry::getElements()const{
  return this->elements;
}


const std::vector<float>& Geometry::getNormals()const{
  return this->normals;
}


const std::vector<float>& Geometry::getTexCoords()const{
  return this->texCoords;
}


const std::vector<float>& Geometry::getTangents()const{
  return this->tangents;
}


const std::vector<float>& Geometry::getBitangents()const{
  return this->bitangents;
}


Geometry& Geometry::setVertices(std::vector<float> vertices){
  this->vertices = vertices;
  return *this;
}

Geometry& Geometry::setElements(std::vector<int> elements){
  this->elements = elements;
  return *this;
}

Geometry& Geometry::setNormals(std::vector<float> normals){
  this->normals = normals;
  return *this;
}

Geometry& Geometry::setTexCoords(std::vector<float> texCoords){
  this->texCoords = texCoords;
  return *this;
}

Geometry& Geometry::setTangents(std::vector<float> tangents){
  this->tangents = tangents;
  return *this;
}

Geometry& Geometry::setBitangents(std::vector<float> bitangents){
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

Geometry loadDataFromFile(std::string filename){
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
    1.0, -1.0, 0.0,
    1.0, 1.0, 0.0,
    -1.0, 1.0, 0.0,
    -1.0, -1.0, 0.0,
    1.0, -1.0, 0.0,
    -1.0, 1.0, 0.0
  };
  float normals[18] = {
    0.0, 0.0, 1.0,
    0.0, 0.0, 1.0,
    0.0, 0.0, 1.0,
    0.0, 0.0, 1.0,
    0.0, 0.0, 1.0,
    0.0, 0.0, 1.0
  };

  Geometry geom = Geometry();
  geom.setVertices(std::vector<float>(vertices,vertices+18));
  geom.setNormals(std::vector<float>(normals,normals+18));
  return geom;
}

Geometry generateCubeGeometry(int size){
  return Geometry();
}

Geometry generateCubeWireframe(int size){
  return Geometry();
}

}