#ifndef GEOMETRY_H
#define GEOMETRY_H

#include <vector>
#include <string>

namespace object3D{
class Geometry{
private:
  //use vectors instead of shared_ptr
  std::vector<float> vertices;
  std::vector<int> elements;
  std::vector<float> normals;
  std::vector<float> texCoords;
  std::vector<float> tangents;
  std::vector<float> bitangents;

  int vertexBuffer;
  int elementBuffer;
  int normalBuffer;
  int texCoordBuffer;
  int tangentBuffer;

  bool initialized;
public:
  Geometry();
  Geometry(const Geometry& geom)=delete;
  Geometry(Geometry&& geom)=default;
  Geometry& operator=(const Geometry& geom)=delete;
  Geometry& operator=(Geometry&& geom)=default;
  ~Geometry();

  const std::vector<float>& getVertices()const;
  const std::vector<int>& getElements()const;
  const std::vector<float>& getNormals()const;
  const std::vector<float>& getTexCoords()const;
  const std::vector<float>& getTangents()const;
  const std::vector<float>& getBitangents()const;

  bool isInitialized()const;
  Geometry& setInitialized(bool initialized);

  Geometry& setVertices(std::vector<float> vertices);
  Geometry& setElements(std::vector<int> elements);
  Geometry& setNormals(std::vector<float> normals);
  Geometry& setTexCoords(std::vector<float> texCoords);
  Geometry& setTangents(std::vector<float> tangents);
  Geometry& setBitangents(std::vector<float> bitangents);

  int getVertexBuffer()const;
  int getElementBuffer()const;
  int getNormalBuffer()const;
  int getTexCoordBuffer()const;
  int getTangentBuffer()const;

  Geometry& setVertexBuffer(int vertexBuffer);
  Geometry& setElementBuffer(int elementBuffer);
  Geometry& setNormalBuffer(int normalBuffer);
  Geometry& setTexCoordBuffer(int texCoordsBuffer);
  Geometry& setTangentBuffer(int tangentBuffer);
  friend Geometry loadDataFromFile(std::string filename);
};
Geometry loadDataFromFile(std::string filename);
Geometry generateQuadGeometry(int size =1);
Geometry generateCubeGeometry(int size);
Geometry generateCubeWireframe(int size);
}

#endif