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
  int location;
  void* value;
};
typedef struct uniform Uniform;

struct shader{
  std::string name;
  std::string type;
  std::string source;
  int shaderObject;
};
typedef struct shader Shader;

class Uniforms {
public:
  int unifModelMatrix;
  int unifBlockAmbientLight;
  int unifDiffuseColor;
  int unifSpecularColor;
  int unifShininess;
  int unifDistanceToCamera;
  int unifMaxLightIntensity;
  int unifInvGamma;
  int unifMapSampler;
  int unifNormalMapSampler;
  int unifShadowMapSampler;
  int unifWorldMatrix;
  int unifProjectionMatrix;
  int unifDirLightColor;
  int unifDirLightVectorToLight;
  int unifDirLightIntensity;
  int unifPointLightColor;
  int unifPointLightPosition;
  int unifPointLightIntensity;
  int unifPointLightAttenuation;
  int unifAmbientLight;
  int unifDepthWorldMatrix;
  int unifSampleSize;
  int unifShadowMapSize;
  Uniforms();
};

class GLProgram{
private:
  int vertexShader;
  int fragmentShader;
  int tessEvaluationShader;
  int tessControlShader;
  int program;
  int attrPosition;
  int attrNormal;
  int attrUV;
  int attrTangent;
  std::shared_ptr<Uniforms> uniforms;
  std::unordered_map<std::string,Uniform> puniforms;
  std::vector<Shader> shaders;
public:
  GLProgram();
  int getVertexShader()const;
  int getFragmentShader()const;
  int getTessControlShader()const;
  int getTessEvaluationShader()const;
  int getProgram()const;
  int getAttrPosition()const;
  int getAttrUV()const;
  int getAttrTangent()const;
  int getAttrNormal()const;
  std::shared_ptr<Uniforms> getUniforms()const;
  GLProgram& setVertexShader(int vertexShader);
  GLProgram& setFragmentShader(int fragmentShader);
  GLProgram& setTessEvaluationShader(int tessEvaluationShader);
  GLProgram& setTessControlShader(int tessControlShader);
  GLProgram& setProgram(int program);
  GLProgram& setAttrPosition(int attrPosition);
  GLProgram& setAttrUV(int attrUV);
  GLProgram& setAttrTangent(int attrTangent);
  GLProgram& setAttrNormal(int attrNormal);
  GLProgram& setUniforms(std::shared_ptr<Uniforms> uniforms);
  std::string getSourceFromFile(std::string filename);
  int compileShader(GLenum type, std::string source);
  int linkProgram(int vertexShader, int fragmentShader);
  const std::unordered_map<std::string,Uniform>& getpUniforms();
  const std::vector<Shader>& getShaders();
  //int linkProgramTessellation(int vertexShader, int fragmentShader, int tessControlShader, int tessEvaluationShader);
  ~GLProgram();
};
}
#endif