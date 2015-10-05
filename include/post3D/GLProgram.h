#ifndef GLPROGRAM_H
#define GLPROGRAM_H

#include <GL/glew.h>
#include <string>
#include <memory>
#include <unordered_map>
#include <vector>

namespace material{

struct uniform{
  std::string name;
  std::string type;
  int location = -1;
  void* value;
};
typedef struct uniform Uniform;

struct shader{
  std::string name;
  std::string type;
  std::string source;
  int shaderObject = 0;
};
typedef struct shader Shader;

class GLProgram{
private:
  int program;
  int attrPosition;
  int attrNormal;
  int attrUV;
  int attrTangent;
  std::unordered_map<std::string,Uniform> puniforms;
  std::vector<Shader> shaders;
public:
  GLProgram();
  int getProgram()const;
  int getAttrPosition()const;
  int getAttrUV()const;
  int getAttrTangent()const;
  int getAttrNormal()const;
  GLProgram& setProgram(int program);
  GLProgram& setAttrPosition(int attrPosition);
  GLProgram& setAttrUV(int attrUV);
  GLProgram& setAttrTangent(int attrTangent);
  GLProgram& setAttrNormal(int attrNormal);
  std::string getSourceFromFile(std::string filename);
  int compileShader(Shader& shader);
  std::unordered_map<std::string,Uniform>& getpUniforms();
  std::vector<Shader>& getShaders();
  int makeProgram();
  ~GLProgram();
  GLProgram& addUniform(std::string name, std::string type, void* value);
};
}
#endif