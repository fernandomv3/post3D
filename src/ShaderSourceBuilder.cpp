#include "post3D/ShaderSourceBuilder.h" 
#include <fstream>
#include <streambuf>
#include <cerrno>

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
    {"vertexTangent", "in vec4 vertexTangent;\n"}
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
  };
  this->uniform ={
    {"modelMatrix","uniform mat4 modelMatrix;\n"},
    {"worldMatrix","uniform mat4 worldMatrix;\n"},
    {"projectionMatrix","uniform mat4 projectionMatrix;\n"},
    {"depthWorldMatrix",
      "#ifdef SHADOWMAP\n"
      "uniform mat4 depthWorldMatrix;\n"
      "#endif"},
    {"material",
      "struct Material{\n"
      "  vec4 diffuseColor;\n"
      "  vec4 specularColor;\n"
      "  float shininess;\n"
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
    {"dirLight",
      "uniform vec4 dirLightColor;\n"
      "uniform vec4 dirLightVectorToLight;\n"
      "uniform float dirLightIntensity;\n"},
    {"colorMap",
      "#ifdef MAP\n"
      "uniform sampler2D colorMap;\n"
      "#endif\n"},
    {"normalMap",
      "#ifdef NORMALMAP\n"
      "uniform sampler2D normalMap;\n"
      "#endif\n"},
    //texturemaps for deferred shading
    {"deferredPositionMap","uniform sampler2D positionMap;\n"},
    {"deferredWorldPositionMap","uniform sampler2D worldPositionMap;\n"},
    {"deferredColorMap","uniform sampler2D colorMap;\n"},
    {"deferredNormalMap","uniform sampler2D normalMap;\n"},
    {"deferredUvMap","uniform sampler2D uvMap;\n"},
    {"maxLightIntensity","uniform float maxLightIntensity;\n"},
    {"invGamma","uniform float invGamma;\n"},
    {"shadowMap",
      "#ifdef SHADOWMAP\n"
      "uniform sampler2D shadowMap;\n"
      "#endif\n"},
    {"PCFShadow",
      "#ifdef SHADOWMAP\n"
      "#ifdef PCFSHADOW\n"
      "uniform vec2 shadowMapSize;\n"
      "uniform int sampleSize;\n"
      "#endif\n"
      "#endif\n"},
    {"screenSize","uniform vec2 screenSize;\n"}
  };
  this->vertexChunk = {
    {"homogenizeVertex","  vec4 pos = vec4(position,1.0);\n"},
    {"modelSpace","  vec4 modelSpace = modelMatrix * pos;\n"},
    {"worldSpace","  vec4 worldSpace = worldMatrix * modelSpace;\n"},
    {"nDCSpace","  vec4 nDCSpace = projectionMatrix * worldSpace;\n"},
    {"gl_Position_h","  gl_Position = pos;\n"},
    {"gl_Position_m","  gl_Position = modelSpace;\n"},
    {"gl_Position_w","  gl_Position = worldSpace;\n"},
    {"gl_Position_n","  gl_Position = nDCSpace;\n"},
    {"outDepthPosition",
      "  #ifdef SHADOWMAP\n"
      "  depthPosition = depthWorldMatrix * modelMatrix *pos;\n"
      "  #endif\n"},
    {"outVertexPosition","  vertexPosition = pos;\n"},
    {"outVertexNormal", "  vertexNormal = normalize(worldMatrix * modelMatrix * vec4(normal,0.0));\n"},
    {"outWorldSpacePosition", "  worldSpacePosition = worldSpace;\n"},
    {"outVertexUV", "  vertexUV = uv;\n"},
    {"outVertexTangent", "  vertexTangent = normalize(worldMatrix * modelMatrix * vec4(tangent,0.0));\n"}
  };
  this->helpers = {
    {"attenuateLightFunc",
      "#if MAX_P_LIGHTS > 0\n"
      "vec4 attenuateLight(in vec4 color, in float attenuation, in vec4 vectorToLight){\n"
      "  float distSqr = dot(vectorToLight,vectorToLight);\n"
      "  vec4 attenLightIntensity = color * (1/(1.0 + attenuation * sqrt(distSqr)));\n"
      "  return attenLightIntensity;\n"
      "}\n"
      "#endif\n"},
    {"PCFShadowFactorFunc",
      "#ifdef SHADOWMAP\n"
      "#ifdef PCFSHADOW\n"
      "float calculateShadowFactorPCF(in vec4 lightSpacePos){\n"
      "  vec3 projCoords = lightSpacePos.xyz / lightSpacePos.w;\n"
      "  vec3 uv = 0.5 * projCoords + 0.5;\n"
      "  if(uv.x > 1.0 || uv.y > 1.0 || uv.x < 0.0 || uv.y < 0.0) return 1.0;\n"
      "  vec2 offset = vec2(1.0/shadowMapSize.x,1.0/shadowMapSize.y);\n"
      "  float result = 0.0;\n"
      "  for(int i = -sampleSize; i< sampleSize;i++){\n"
      "    for(int j = -sampleSize; j < sampleSize; j++){\n"
      "      vec2 offsets = vec2(i*offset.x , j*offset.y);\n"
      "      vec3 uvc = vec3(uv.xy +offsets, uv.z + 0.00001);\n"
      "      float mapDepth = texture(shadowMap,uv.xy+offsets).x;\n"
      "      if( mapDepth < uv.z + 0.00001){\n"
      "        result += 0.0;\n"
      "      }\n"
      "      else{\n"
      "        result += 1.0;\n"
      "      }\n"
      "    }\n"
      "  }\n"
      "  float numSamples = (1 + 2*sampleSize)*(1 + 2*sampleSize);\n"
      "  return 0.5 + (result/numSamples);\n"
      "}\n"
      "#endif\n"
      "#endif\n"},
    {"shadowFactorFunc",
      "#ifdef SHADOWMAP\n"
      "float calculateShadowFactor(in vec4 lightSpacePos){\n"
      "  vec3 projCoords = lightSpacePos.xyz / lightSpacePos.w;\n"
      "  vec3 uv = 0.5 * projCoords + 0.5;\n"
      "  if(uv.x > 1.0 || uv.y > 1.0 || uv.x < 0.0 || uv.y < 0.0) return 1.0;\n"
      "  float depth = texture(shadowMap,uv.xy).x;\n"
      "  if (depth < uv.z + 0.00001){\n"
      "    return 0.5;\n"
      "  }\n"
      "  return 1.0;\n"
      "}\n"
      "#endif\n"},
    {"calculateNormalFunc",
      "#ifdef NORMALMAP\n"
      "vec4 calculateNormal(in vec4 vNormal,in vec4 vTangent, in vec2 uv ,in sampler2D normalMap){\n"
      "  vec3 normal = normalize(vNormal).xyz;\n"
      "  vec3 tangent = normalize(vTangent).xyz;\n"
      "  tangent = normalize(tangent - dot(tangent,normal) * normal);\n"
      "  vec3 bitangent = -cross(tangent,normal);\n"
      "  vec3 texNormal = 255 * texture(normalMap,uv).xyz /128 - vec3(1.0,1.0,1.0);\n"
      "  mat3 TBN = mat3(tangent, bitangent, normal);\n"
      "  texNormal = TBN * texNormal;\n"
      "  texNormal = normalize(texNormal);\n"
      "  return vec4(texNormal,0.0);\n"
      "}\n"
      "#endif\n"},
    {"warpFunc",
      "float warp (in float value,in float factor){\n"
      "  return (value + factor ) / (1+ clamp(factor,0,1));\n"
      "}\n"},
    {"calculateCosAngIncidenceFunc",
      "float calculateCosAngIncidence(in vec4 direction, in vec4 normal,in float warpFactor){\n"
      "  //use warpFactor = 0 for no warp\n"
      "  float cosAngIncidence = dot(normal,direction);\n"
      "  cosAngIncidence = warp(cosAngIncidence,warpFactor);\n"
      "  cosAngIncidence = clamp(cosAngIncidence,0,1);\n"
      "  return cosAngIncidence;\n"
      "}\n"},
    {"calculateBlinnPhongTermFunc",
      "float calculateBlinnPhongTerm(in vec4 direction,vec4 normal, in vec4 viewDirection, in float shininess, out float cosAngIncidence){\n"
      "  cosAngIncidence = calculateCosAngIncidence(normal,direction,0);\n"
      "  vec4 halfAngle = normalize(direction + viewDirection);\n"
      "  float blinnPhongTerm = dot(normal, halfAngle);\n"
      "  blinnPhongTerm = clamp(blinnPhongTerm, 0, 1);\n"
      "  blinnPhongTerm = cosAngIncidence != 0.0 ? blinnPhongTerm : 0.0;\n"
      "  blinnPhongTerm = pow(blinnPhongTerm, shininess);\n"
      "  return blinnPhongTerm;\n"
      "}\n"}
  };
  this->fragmentChunk ={
    {"viewDirection","  vec4 viewDirection = normalize(-worldSpacePosition);\n"},
    {"texCoord","  vec2 texCoord = gl_FragCoord.xy / screenSize;\n"},
    {"mappedDepthPosition","  vec4 depthPosition = depthWorldMatrix * texture(positionMap,texCoord);\n"},
    {"mappedViewDirection","  vec4 viewDirection = normalize(-texture(worldPositionMap,texCoord));\n"},
    {"mappedDiffuseColor","  vec4 diffuseColor = texture(colorMap,texCoord);\n"},
    {"mappedNormal","  vec4 normal = normalize(texture(normalMap,texCoord));"},
    {"directionalLightFactor",
      "  vec4 normDirection = normalize(dirLightVectorToLight);\n"
      "  float cosAngIncidence;\n"
      "  float blinnPhongTerm = calculateBlinnPhongTerm(normDirection,normal,viewDirection,material.shininess,cosAngIncidence);\n"
      "  outputColor = outputColor + (shadowFactor * dirLightColor * diffuseColor * cosAngIncidence * dirLightIntensity / maxLightIntensity);\n"
      "  outputColor = outputColor + (shadowFactor * dirLightColor * specularColor * blinnPhongTerm * dirLightIntensity / maxLightIntensity);\n"},
    {"initOutputColor","  outputColor = vec4(0.0,0.0,0.0,1.0);\n"},
    {"materialDiffuseColor","  vec4 diffuseColor = material.diffuseColor;\n"},
    {"materialSpecularColor","  vec4 specularColor = material.specularColor;\n"},
    {"interpolatedNormal","  vec4 normal = normalize(vertexNormal);\n"},
    {"mapDiffuseColor",
      "  #ifdef MAP\n"
      "  diffuseColor = texture(colorMap,texCoord);\n"
      "  #endif\n"},
    {"mapNormal",
      "  #ifdef NORMALMAP\n"
      "  normal = calculateNormal(vertexNormal,vertexTangent,vertexUV,normalMap);\n"
      "  #endif\n"},
    {"initShadowFactor","float shadowFactor = 1.0;\n"},
    {"PFCshadowFactor",
      "  #ifdef SHADOWMAP\n"
      "  #ifdef PCFSHADOW\n"
      "  shadowFactor = calculateShadowFactorPCF(depthPosition);\n"
      "  #endif\n"
      "  #endif\n"},
    {"shadowFactor",
      "  #ifdef SHADOWMAP\n"
      "  shadowFactor = calculateShadowFactor(depthPosition);\n"
      "  #endif\n"},
    {"addDirectionalLightsFactor",
      "  #if MAX_DIR_LIGHTS > 0\n"
      "  for(int i=0; i< MAX_DIR_LIGHTS ;i++){\n"
      "    vec4 normDirection = normalize(dirLightVectorToLight[i]);\n"
      "    float cosAngIncidence;\n"
      "    float blinnPhongTerm = calculateBlinnPhongTerm(normDirection,normal,viewDirection,material.shininess,cosAngIncidence);\n"
      "    outputColor = outputColor + (shadowFactor * dirLightColor[i] * diffuseColor * cosAngIncidence * dirLightIntensity[i] / maxLightIntensity);\n"
      "    outputColor = outputColor + (shadowFactor * dirLightColor[i] * specularColor * blinnPhongTerm * dirLightIntensity[i] / maxLightIntensity);\n"
      "  }\n"
      "  #endif\n"},
    {"addPointLightsFactor",
      "  #if MAX_P_LIGHTS > 0\n"
      "  for(int i=0; i< MAX_P_LIGHTS ;i++){\n"
      "    vec4 difference = pointLightPosition[i] - worldSpacePosition;\n"
      "    vec4 normDirection = normalize(difference);\n"
      "    vec4 attenLightIntensity = attenuateLight(pointLightColor[i],pointLightAttenuation[i],difference);\n"
      "    float cosAngIncidence;\n"
      "    float blinnPhongTerm = calculateBlinnPhongTerm(normDirection,normal,viewDirection,material.shininess,cosAngIncidence);\n"
      "    \n"
      "    outputColor = outputColor + (attenLightIntensity * diffuseColor * cosAngIncidence * pointLightIntensity[i] / maxLightIntensity);\n"
      "    outputColor = outputColor + (specularColor * attenLightIntensity * blinnPhongTerm * pointLightIntensity[i] / maxLightIntensity);\n"
      "  }\n"
      "  #endif\n"},
    {"addAmbientLightFactor", "outputColor = outputColor + (diffuseColor * ambientLight);\n"},
    {"gammaCorrection",
      "  vec4 gamma = vec4(invGamma);\n"
      "  gamma.w = 1.0;\n"
      "  outputColor = pow(outputColor,gamma);\n"},
    {"outputMapColor","   outputColor= texture2D(colorMap,vertexPosition.xy * 0.5 + 0.5);\n"},
    {"outputDepth",
      "  float depth = texture(colorMap,vertexPosition.xy).x;\n"
      "  depth = 1.0 - (1.0 - depth) *25.0;\n"
      "  outputColor= vec4(depth);\n"},
    {"outputColorTarget","outputColor = diffuseColor;"},
    {"outputNormalTarget","outputNormal = normal;"},
    {"outputPositionTarget","outputPosition = worldSpacePosition;"},
    {"outputUVTarget","outputUV = vec4(vertexUV,0.0,0.0);"}
  };
}

string get_file_contents(const char *filename){
  ifstream in(filename, ios::in | ios::binary);
  if (in){
    return(std::string((std::istreambuf_iterator<char>(in)), std::istreambuf_iterator<char>()));
  }
  throw(errno);
}

}