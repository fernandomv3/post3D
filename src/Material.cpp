#include "post3D/Material.h"
#include <sstream>
namespace material{
	Material::Material(){
		this->diffuseColor = shared_ptr<Color>(new Color());
		this->specularColor = shared_ptr<Color>(new Color());
		this->shininess = 1.0f;
		this->vertexShaderSource = string("");
		this->fragmentShaderSource = string("");
		this->program = shared_ptr<GLProgram>();
		this->map = shared_ptr<Texture>();
		this->normalMap = shared_ptr<Texture>();
		this->bumpMap = shared_ptr<Texture>();
		this->specularMap = shared_ptr<Texture>();
		this->program = shared_ptr<GLProgram>();
	}

	Material::Material(const Material& mat){
		this->diffuseColor = mat.diffuseColor;
		this->specularColor = mat.specularColor;
		this->shininess = mat.shininess;
		this->vertexShaderSource = mat.vertexShaderSource;
		this->fragmentShaderSource = mat.fragmentShaderSource;
		this->program = mat.program;
		this->map = mat.map;
		this->normalMap = mat.normalMap;
		this->bumpMap = mat.bumpMap;
		this->specularMap = mat.specularMap;
		this->program = mat.program;
	}

	Material::Material(Material&& mat){
		this->diffuseColor = move(mat.diffuseColor);
		this->specularColor = move(mat.specularColor);
		this->shininess = mat.shininess;
		this->vertexShaderSource = move(mat.vertexShaderSource);
		this->fragmentShaderSource = move(mat.fragmentShaderSource);
		this->program = move(mat.program);
		this->map = move(mat.map);
		this->normalMap = move(mat.normalMap);
		this->bumpMap = move(mat.bumpMap);
		this->specularMap = move(mat.specularMap);
		this->program = move(mat.program);
	}

	Material& Material::operator=(const Material& mat){
		this->diffuseColor = mat.diffuseColor;
		this->specularColor = mat.specularColor;
		this->shininess = mat.shininess;
		this->vertexShaderSource = mat.vertexShaderSource;
		this->fragmentShaderSource = mat.fragmentShaderSource;
		this->program = mat.program;
		this->map = mat.map;
		this->normalMap = mat.normalMap;
		this->bumpMap = mat.bumpMap;
		this->specularMap = mat.specularMap;
		this->program = mat.program;
		return *this;
	}

	Material& Material::operator=(Material&& mat){
		this->diffuseColor = move(mat.diffuseColor);
		this->specularColor = move(mat.specularColor);
		this->shininess = mat.shininess;
		this->vertexShaderSource = move(mat.vertexShaderSource);
		this->fragmentShaderSource = move(mat.fragmentShaderSource);
		this->program = move(mat.program);
		this->map = move(mat.map);
		this->normalMap = move(mat.normalMap);
		this->bumpMap = move(mat.bumpMap);
		this->specularMap = move(mat.specularMap);
		this->program = move(mat.program);
		return *this;
	}

	shared_ptr<GLProgram> Material::getProgram() const{
		return this->program;
	}

	Material& Material::setProgram(shared_ptr<GLProgram> program){
		this->program = program;
		return *this;
	}

	string Material::getVertexShaderSource()const{
		return this->vertexShaderSource;
	}

	Material& Material::setVertexShaderSource(string vertexShaderSource){
		this->vertexShaderSource = vertexShaderSource;
		return *this;
	}

	string Material::getFragmentShaderSource()const{
		return this->fragmentShaderSource;
	}

	Material& Material::setFragmentShaderSource(string fragmentShaderSource){
		this->fragmentShaderSource = fragmentShaderSource;
		return *this;
	}

	Material& Material::setDiffuseColor(shared_ptr<Color> color){
		this->diffuseColor = color;
		return *this;
	}

	shared_ptr<Color> Material::getDiffuseColor()const{
		return this->diffuseColor;
	}

	Material& Material::setSpecularColor(shared_ptr<Color> color){
		this->specularColor = color;
		return *this;
	}

	shared_ptr<Color> Material::getSpecularColor()const{
		return this->specularColor;
	}

	Material& Material::setShininess(float shininess){
		this->shininess = shininess;
		return *this;
	}

	float Material::getShininess()const{
		return this->shininess;
	}

	shared_ptr<Texture> Material::getMap()const{
		return this->map;
	}

	Material& Material::setMap(shared_ptr<Texture> map){
		this->map = map;
		return *this;
	}

	shared_ptr<Texture> Material::getNormalMap()const{
		return this->normalMap;
	}

	Material& Material::setNormalMap(shared_ptr<Texture> map){
		this->normalMap = map;
		return *this;
	}

	shared_ptr<Texture> Material::getBumpMap()const{
		return this->bumpMap;
	}

	Material& Material::setBumpMap(shared_ptr<Texture> map){
		this->bumpMap = map;
		return *this;
	}

	shared_ptr<Texture> Material::getSpecularMap()const{
		return this->specularMap;
	}

	Material& Material::setSpecularMap(shared_ptr<Texture> map){
		this->specularMap = map;
		return *this;
	}

	unique_ptr<MaterialStruct> Material::getAsStruct()const{
		unique_ptr<MaterialStruct> mat = unique_ptr<MaterialStruct>(new MaterialStruct);
		mat->diffuseColor[0] = this->diffuseColor->getComponent('r');
		mat->diffuseColor[1] = this->diffuseColor->getComponent('g');
		mat->diffuseColor[2] = this->diffuseColor->getComponent('b');

		mat->specularColor[0] = this->specularColor->getComponent('r');
		mat->specularColor[1] = this->specularColor->getComponent('g');
		mat->specularColor[2] = this->specularColor->getComponent('b');
		mat->shininess = this->shininess;
		
		return move(mat);
	}

	MaterialType Material::getType()const{
		return this->type;
	}

	string Material::configureSource(string source,int numDirLights,int numPLights,bool colorMap,bool normalMap,bool shadowMap,bool PCFShadows){
		ostringstream ndlights;
		ndlights << numDirLights;
		
		ostringstream nplights;
		nplights << numPLights;
		
		string head = "#version 330 core \n";
		head += "#define MAX_DIR_LIGHTS "+ ndlights.str() +"\n";
		head += "#define MAX_P_LIGHTS "+ nplights.str()+"\n";
		if(colorMap) {
            head += "#define MAP\n";
        }
        if(normalMap){
            head += "#define NORMALMAP\n";
        }
        if(shadowMap){
            head += "#define SHADOWMAP\n";
        }
        if (PCFShadows){
            head += "#define PCFSHADOW\n";
        }
        return head + source;
	}

	void Material::makePrograms(const Scene& scene){

	}

}