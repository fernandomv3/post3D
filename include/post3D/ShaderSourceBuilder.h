#ifndef SHADERSOURCEBUILDER_H
#define SHADERSOURCEBUILDER_H

#include <unordered_map>
#include <string>

using namespace std;

namespace material{
class ShaderSourceBuilder{
public:
  string version;
  unordered_map<string,string> attribute;
  unordered_map<string,string> uniform;
  unordered_map<string,string> output;
  unordered_map<string,string> vertexChunk;
  unordered_map<string,string> fragmentChunk;
  unordered_map<string,string> vertexParams;
  unordered_map<string,string> fragmentParams;
  unordered_map<string,string> helpers;
  ShaderSourceBuilder();
  ShaderSourceBuilder(const ShaderSourceBuilder& ssb)=default;
  ShaderSourceBuilder(ShaderSourceBuilder&& ssb)=default;
  ShaderSourceBuilder& operator=(const ShaderSourceBuilder& ssb)=default;
  ShaderSourceBuilder& operator=(ShaderSourceBuilder&& ssb)=default;
  ~ShaderSourceBuilder()=default;
};
}

#endif