#ifndef GLPROGRAM_H
#define GLPROGRAM_H

#include <GL/glew.h>
#include <string>
#include <memory>

using namespace std;

namespace material{
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
		shared_ptr<Uniforms> uniforms;
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
		shared_ptr<Uniforms> getUniforms()const;
		GLProgram& setVertexShader(int vertexShader);
		GLProgram& setFragmentShader(int fragmentShader);
		GLProgram& setTessEvaluationShader(int tessEvaluationShader);
		GLProgram& setTessControlShader(int tessControlShader);
		GLProgram& setProgram(int program);
		GLProgram& setAttrPosition(int attrPosition);
		GLProgram& setAttrUV(int attrUV);
		GLProgram& setAttrTangent(int attrTangent);
		GLProgram& setAttrNormal(int attrNormal);
		GLProgram& setUniforms(shared_ptr<Uniforms> uniforms);
		string getSourceFromFile(string filename);
		int compileShader(GLenum type, string source);
		int linkProgram(int vertexShader, int fragmentShader);
		//int linkProgramTessellation(int vertexShader, int fragmentShader, int tessControlShader, int tessEvaluationShader);
		~GLProgram();
	};
}
#endif