#include "post3D/ShaderSourceBuilder.h"  

namespace material{

ShaderSourceBuilder::ShaderSourceBuilder(){
  this->attribute = {
    //inputs from vertex shader (attributes)
    {"normal","layout(location = 0)in vec3 normal;\n"},
    {"position","layout(location = 1)in vec3 position;\n"},
    {"uv","layout(location = 2)in vec2 uv;\n"},
    {"tangent","layout(location = 3)in vec3 tangent;\n"},
    //inputs from vertexShader to fragmentShader
    {"depthPosition",
      "#ifdef SHADOWMAP\n"
      "in vec4 depthPosition;\n"
      "#endif\n"},
    {"vertexPosition","in vec4 vertexPosition;\n"},
    {"vertexNormal", "in vec4 vertexNormal;\n"},
    {"worldSpacePosition", "in vec4 worldSpacePosition;\n"},
    {"vertexUV", "in vec2 vertexUV;\n"},
    {"vertexTangent", "in vec4 vertexTangent;\n"},
  };
  this->output ={
    //output from vertex shader to fragment shader
    {"depthPosition",
      "#ifdef SHADOWMAP\n"
      "out vec4 depthPosition;\n"
      "#endif\n"},
    {"vertexPosition","out vec4 vertexPosition;\n"},
    {"vertexNormal", "out vec4 vertexNormal;\n"},
    {"worldSpacePosition", "out vec4 worldSpacePosition;\n"},
    {"vertexUV", "out vec2 vertexUV;\n"},
    {"vertexTangent", "out vec4 vertexTangent;\n"},
    //output to render targets
    {"color","layout(location = 0) out vec4 outputColor;\n"},
    {"normal","layout(location = 1) out vec4 outputNormal;\n"},
    {"position","layout(location = 2) out vec4 outputPosition;\n"},
    {"uv","layout(location = 3) out vec4 outputUV;\n"},
    {"depth",
      "#ifdef SHADOWMAP\n"
      "layout(location = 4)out vec4 depthPosition;\n"
      "#endif\n"}
    }
  };
  this->uniforms ={
    {"modelMatrix","uniform mat4 modelMatrix;\n"},
    {"worldMatrix","uniform mat4 worldMatrix;\n"},
    {"projectionMatrix","uniform mat4 projectionMatrix;\n"},
    {"depthWorldMatrix",
      "#ifdef SHADOWMAP\n"
      "uniform mat4 depthWorldMatrix;\n"
      "#endif"},
    {"material",
      "struct Material{\n"
        "vec4 diffuseColor;\n"
        "vec4 specularColor;\n"
        "float shininess;\n"
      "};\n"
      "uniform Material material;\n"},
    {"dirLights",
      "#if MAX_DIR_LIGHTS > 0\n"
      "uniform vec4 dirLightColor[MAX_DIR_LIGHTS];\n"
      "uniform vec4 dirLightVectorToLight[MAX_DIR_LIGHTS];\n"
      "uniform float dirLightIntensity[MAX_DIR_LIGHTS];\n"
      "#endif\n"},
    {"pLights",
      "#if MAX_P_LIGHTS > 0\n"
      "uniform vec4 pointLightColor[MAX_P_LIGHTS];\n"
      "uniform vec4 pointLightPosition[MAX_P_LIGHTS];\n"
      "uniform float pointLightIntensity[MAX_P_LIGHTS];\n"
      "uniform float pointLightAttenuation[MAX_P_LIGHTS];\n"
      "#endif\n"},
    {"ambientLight","uniform vec4 ambientLight;\n"},
    {"colorMap",
      "#ifdef MAP\n"
      "uniform sampler2D colorMap;\n"
      "#endif\n"},
    {"normalMap",
      "#ifdef NORMALMAP\n"
      "uniform sampler2D normalMap;\n"
      "#endif\n"},
    {"maxLightIntensity","uniform float maxLightIntensity;\n"},
    {"invGamma","uniform float invGamma;\n"},
    {"shadowMap",
      "#ifdef SHADOWMAP\n"
      "uniform sampler2D shadowMap;\n"
      "#endif\n"},
    {"PCFShadow",
      "#if define(SHADOWMAP) && define(PCFSHADOW)"
      "uniform vec2 shadowMapSize;\n"
      "uniform int sampleSize;\n"
      "#endif\n"}
  };
  this->vertexChunk = {
    {"homogenizeVertex","vec4 pos = vec4(position,1.0);\n"},
    {"modelSpace","vec4 modelSpace = modelMatrix * pos;\n"},
    {"worldSpace","vec4 worldSpace = worldMatrix * modelSpace;\n"},
    {"nDCSpace","vec4 nDCSpace = projectionMatrix * worldSpace;\n"},
    {"gl_Position_h","gl_Position = pos;\n"},
    {"gl_Position_m","gl_Position = modelSpace;\n"},
    {"gl_Position_w","gl_Position = worldSpace;\n"},
    {"gl_Position_n","gl_Position = nDCSpace;\n"},
    {"outDepthPosition",
      "#ifdef SHADOWMAP\n"
      "depthPosition = depthWorldMatrix * modelMatrix *pos;\n"
      "#endif\n"},
    {"outVertexPosition","vertexPosition = pos;\n"},
    {"outVertexNormal", "vertexNormal = normalize(worldMatrix * modelMatrix * vec4(normal,0.0));\n"},
    {"outWorldSpacePosition", "worldSpacePosition = worldSpace;\n"},
    {"outVertexUV", "vertexUV = uv;\n"},
    {"outVertexTangent", "vertexTangent = normalize(worldMatrix * modelMatrix * vec4(tangent,0.0));\n"}
  };
}

}