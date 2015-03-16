#include "post3D/GLProgram.h"
#include <iostream>
#include <memory>

namespace material{

    Uniforms::Uniforms(){
    	this->unifModelMatrix= -1;
	    this->unifBlockAmbientLight= -1;
	    this->unifDiffuseColor= -1;
	    this->unifSpecularColor= -1;
	    this->unifShininess= -1;
	    this->unifDistanceToCamera= -1;
	    this->unifMaxLightIntensity= -1;
	    this->unifInvGamma= -1;
	    this->unifMapSampler= -1;
	    this->unifNormalMapSampler= -1;
	    this->unifShadowMapSampler= -1;
	    this->unifWorldMatrix= -1;
	    this->unifProjectionMatrix= -1;
	    this->unifDirLightColor= -1;
	    this->unifDirLightVectorToLight= -1;
	    this->unifDirLightIntensity= -1;
	    this->unifPointLightColor= -1;
	    this->unifPointLightPosition= -1;
	    this->unifPointLightIntensity= -1;
	    this->unifPointLightAttenuation= -1;
	    this->unifAmbientLight= -1;
	    this->unifDepthWorldMatrix= -1;
	    this->unifSampleSize= -1;
	    this->unifShadowMapSize= -1;
    }

	GLProgram::GLProgram(){
		this->vertexShader = 0;
		this->fragmentShader = 0;
		this->tessEvaluationShader = 0;
		this->tessControlShader = 0;
		this->program = 0;
		this->attrPosition = 0;
		this->attrNormal = 0;
		this->attrUV = 0;
		this->attrTangent = 0;
		this->uniforms = shared_ptr<Uniforms>(new Uniforms());
	}

	int GLProgram::getVertexShader()const{
		return this->vertexShader;
	}

	int GLProgram::getFragmentShader()const{
		return this->fragmentShader;
	}

	int GLProgram::getTessControlShader()const{
		return this->tessControlShader;
	}

	int GLProgram::getTessEvaluationShader()const{
		return this->tessEvaluationShader;
	}

	int GLProgram::getProgram()const{
		return this->program;
	}

	int GLProgram::getAttrPosition()const{
		return this->attrPosition;
	}

	int GLProgram::getAttrUV()const{
		return this->attrUV;
	}

	int GLProgram::getAttrTangent()const{
		return this->attrTangent;
	}

	int GLProgram::getAttrNormal()const{
		return this->attrNormal;
	}

	shared_ptr<Uniforms> GLProgram::getUniforms()const{
		return this->uniforms;
	}

	GLProgram& GLProgram::setVertexShader(int vertexShader){
		this->vertexShader = vertexShader;
		return *this;
	}

	GLProgram& GLProgram::setFragmentShader(int fragmentShader){
		this->fragmentShader = fragmentShader;
		return *this;
	}

	GLProgram& GLProgram::setTessEvaluationShader(int tessEvaluationShader){
		this->tessEvaluationShader = tessEvaluationShader;
		return *this;
	}

	GLProgram& GLProgram::setTessControlShader(int tessControlShader){
		this->tessControlShader = tessControlShader;
		return *this;
	}

	GLProgram& GLProgram::setProgram(int program){
		this->program = program;
		return *this;
	}

	GLProgram& GLProgram::setAttrPosition(int attrPosition){
		this->attrPosition = attrPosition;
		return *this;
	}

	GLProgram& GLProgram::setAttrUV(int attrUV){
		this->attrUV = attrUV;
		return *this;
	}

	GLProgram& GLProgram::setAttrTangent(int attrTangent){
		this->attrTangent = attrTangent;
		return *this;
	}

	GLProgram& GLProgram::setAttrNormal(int attrNormal){
		this->attrNormal = attrNormal;
		return *this;
	}

	GLProgram& GLProgram::setUniforms(shared_ptr<Uniforms> uniforms){
		this->uniforms = uniforms;
		return *this;
	}

	string GLProgram::getSourceFromFile(string filename){
		return string("");
	}

	int GLProgram::compileShader(GLenum type, string source){
		int length = source.length();
		int shader;
		int shaderOk;

		if(source.empty()) return 0;

		shader = glCreateShader(type);
		const char* c_source = source.c_str();
		glShaderSource(shader, 1, (const GLchar**)&c_source, (GLint*)&length);
		glCompileShader(shader);
		glGetShaderiv(shader, GL_COMPILE_STATUS,&shaderOk);
		if(!shaderOk){
			int maxLength = 0;
			cerr << "Failed to compile:" << endl;
			glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &maxLength);
			auto infoLog = unique_ptr<char[]>(new char[maxLength]);
			glGetShaderInfoLog(shader, maxLength, &maxLength, &infoLog[0]);
			string ilog = string(infoLog.get());
			cerr << ilog << endl;
			glDeleteShader(shader);
        	return 0; 
		}
		return shader;
	}

	int GLProgram::linkProgram(int vertexShader, int fragmentShader){
		int programOk;
		int programValid;
		int program = glCreateProgram();
		glAttachShader(program, vertexShader);
		glAttachShader(program, fragmentShader);
		glLinkProgram(program);
		glGetProgramiv(program, GL_LINK_STATUS, &programOk);
		glValidateProgram(program);
	    glGetProgramiv(program, GL_VALIDATE_STATUS, &programValid);
	    if (!programOk || !programValid) {
	    	int maxLength = 0;
	        cerr << "Failed to link shader program:"<< endl;
	        glGetProgramiv(program, GL_INFO_LOG_LENGTH, &maxLength);
	        auto infoLog = unique_ptr<char[]>(new char[maxLength]);
	        glGetProgramInfoLog(program, maxLength, &maxLength, &infoLog[0]);
	        string ilog = string(infoLog.get());
			cerr << ilog << endl;
	        glDeleteProgram(program);
	        glDeleteShader(vertexShader);
	        glDeleteShader(fragmentShader);
	        return 0;
	    }
	    glDetachShader(program,vertexShader);
	    glDetachShader(program,vertexShader);
	    return program;
	}

	GLProgram::~GLProgram(){
		//glDeleteShader(this->vertexShader);
		//glDeleteShader(this->fragmentShader);
		//glDeleteProgram(this->program);
	}
}