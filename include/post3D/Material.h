#ifndef MATERIAL_H
#define MATERIAL_H
#include "post3D/GLProgram.h"
#include "post3D/Color.h"
#include "post3D/Texture.h"
#include "post3D/Scene.h"
#include <memory>
#include <string>

using namespace std;
using namespace texture;
using namespace math3D;
using namespace scene;
namespace material{
	enum MaterialType {BASIC_MATERIAL,GOURAUD_MATERIAL,PHONG_MATERIAL,TESS_MATERIAL,CEL_MATERIAL,POINT_MATERIAL,LINE_MATERIAL};
	enum {MAP,NORMALMAP,BUMPMAP,SPECULARMAP,SHADOWMAP};
	enum {GLOBALMATRICES,DIRECTIONALLIGHTS,AMBLIGHT,POINTLIGHTS};
	struct materialStruct{
		float diffuseColor[4];
		float specularColor[4];
		float shininess;
		float stride[3];
	};

	typedef struct materialStruct MaterialStruct;

	class Material{
	protected:
		shared_ptr<Color> diffuseColor;
		shared_ptr<Color> specularColor;
		float shininess;
		string vertexShaderSource;
		string fragmentShaderSource;
		shared_ptr<GLProgram> program;
		shared_ptr<Texture> map;
		shared_ptr<Texture> normalMap;
		shared_ptr<Texture> bumpMap;
		shared_ptr<Texture> specularMap;
		MaterialType type;
	public:
		Material();
		Material(const Material& mat);
		Material(Material&& mat);
		Material& operator=(const Material& mat);
		Material& operator=(Material&& mat);
		virtual ~Material()=default;
		shared_ptr<GLProgram> getProgram() const;
		Material& setProgram(shared_ptr<GLProgram> program);
		string getVertexShaderSource() const;
		Material& setVertexShaderSource(string vertexShaderSource);
		string getFragmentShaderSource() const;
		Material& setFragmentShaderSource(string fragmentShaderSource);
		Material& setDiffuseColor(shared_ptr<Color> color);
		shared_ptr<Color> getDiffuseColor() const;
		Material& setSpecularColor(shared_ptr<Color> color);
		shared_ptr<Color> getSpecularColor() const;
		Material& setShininess(float shininess);
		float getShininess() const;
		shared_ptr<Texture> getMap() const;
		Material& setMap(shared_ptr<Texture> map);
		shared_ptr<Texture> getNormalMap() const;
		Material& setNormalMap(shared_ptr<Texture> map);
		shared_ptr<Texture> getBumpMap() const;
		Material& setBumpMap(shared_ptr<Texture> map);
		shared_ptr<Texture> getSpecularMap() const;
		Material& setSpecularMap(shared_ptr<Texture> map);
		unique_ptr<MaterialStruct> getAsStruct() const;
		MaterialType getType() const;
		string configureSource(string source,int numDirLights = 0, int numPLights=0,bool colorMap = false,bool normalMap=false,bool shadowMap=false,bool PCFShadows = false);
		virtual void makePrograms(const Scene& scene, bool deferred = false);
		virtual void makeForwardShader();
		virtual void makeDeferredShader();
	};
}
#endif