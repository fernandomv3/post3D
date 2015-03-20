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
	ShaderSourceBuilder(string version);
	ShaderSourceBuilder(const ShaderSourceBuilder& ssb);
	ShaderSourceBuilder(ShaderSourceBuilder&& ssb);
	ShaderSourceBuilder& operator=(const ShaderSourceBuilder& ssb);
	ShaderSourceBuilder& operator=(ShaderSourceBuilder&& ssb);
	~ShaderSourceBuilder();
};
}

#endif