#ifndef GEOMETRY_H
#define GEOMETRY_H

#include <vector>
#include <string>

using namespace std;

namespace object3D{
	class Geometry{
	private:
		//use vectors instead of shared_ptr
		vector<float> vertices;
		vector<int> elements;
		vector<float> normals;
		vector<float> texCoords;
		vector<float> tangents;
		vector<float> bitangents;

		int vertexBuffer;
		int elementBuffer;
		int normalBuffer;
		int texCoordBuffer;
		int tangentBuffer;

		bool initialized;
	public:
		Geometry();
		Geometry(const Geometry& geom)=default;
		Geometry(Geometry&& geom)=default;
		Geometry& operator=(const Geometry& geom)=default;
		Geometry& operator=(Geometry&& geom)=default;
		~Geometry()=default;

		const vector<float>& getVertices()const;
		const vector<int>& getElements()const;
		const vector<float>& getNormals()const;
		const vector<float>& getTexCoords()const;
		const vector<float>& getTangents()const;
		const vector<float>& getBitangents()const;

		bool isInitialized()const;
		Geometry& setInitialized(bool initialized);

		Geometry& setVertices(vector<float> vertices);
		Geometry& setElements(vector<int> elements);
		Geometry& setNormals(vector<float> normals);
		Geometry& setTexCoords(vector<float> texCoords);
		Geometry& setTangents(vector<float> tangents);
		Geometry& setBitangents(vector<float> bitangents);

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
		friend Geometry loadDataFromFile(string filename);
	};
	Geometry loadDataFromFile(string filename);
	Geometry generateQuadGeometry(int size =1);
	Geometry generateCubeGeometry(int size);
	Geometry generateCubeWireframe(int size);
}

#endif