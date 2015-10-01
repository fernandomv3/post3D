#include "post3D/GLProgram.h"
#include <iostream>
#include <memory>

namespace material{

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

std::string GLProgram::getSourceFromFile(std::string filename){
  return std::string("");
}

int GLProgram::compileShader(GLenum type, std::string source){
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
    std::cerr << "Failed to compile:" << std::endl;
    glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &maxLength);
    auto infoLog = std::unique_ptr<char[]>(new char[maxLength]);
    glGetShaderInfoLog(shader, maxLength, &maxLength, &infoLog[0]);
    std::string ilog = std::string(infoLog.get());
    std::cerr << ilog << std::endl;
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
    std::cerr << "Failed to link shader program:"<< std::endl;
    glGetProgramiv(program, GL_INFO_LOG_LENGTH, &maxLength);
    auto infoLog = std::unique_ptr<char[]>(new char[maxLength]);
    glGetProgramInfoLog(program, maxLength, &maxLength, &infoLog[0]);
    std::string ilog = std::string(infoLog.get());
    std::cerr << ilog << std::endl;
    glDeleteProgram(program);
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
    return 0;
  }
  glDetachShader(program,vertexShader);
  glDetachShader(program,vertexShader);
  return program;
}

std::unordered_map<std::string,Uniform>& GLProgram::getpUniforms(){
  return this->puniforms;
}

const std::vector<Shader>& GLProgram::getShaders(){
  return this->shaders;
}


GLProgram::~GLProgram(){
  //glDeleteShader(this->vertexShader);
  //glDeleteShader(this->fragmentShader);
  //glDeleteProgram(this->program);
}
}