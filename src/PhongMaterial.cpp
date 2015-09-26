#include "post3D/PhongMaterial.h"
#include "post3D/GLProgram.h"
#include "post3D/ShaderSourceBuilder.h"
#include <GL/glew.h>
#include <memory>
#include <string>
#include <iostream>

using namespace std;
namespace material{
PhongMaterial::PhongMaterial():Material(){

}

void PhongMaterial::makeForwardShader(){
	auto ssb = ShaderSourceBuilder();
	this->setVertexShaderSource(
		ssb.attribute["normal"] +
		ssb.attribute["position"]+
        ssb.attribute["uv"] +
        ssb.attribute["tangent"] +
		"\n" + 
		ssb.output["vertexNormal"] +
		ssb.output["worldSpacePosition"] +
		ssb.output["vertexUV"] +
		ssb.output["vertexTangent"] +
		"\n" +
		ssb.output["depthPosition"] +
		"\n" +
		ssb.uniform["modelMatrix"] +
		ssb.uniform["worldMatrix"] +
		ssb.uniform["projectionMatrix"] +
		"\n" + 
		ssb.uniform["depthWorldMatrix"] +
		"\n"
        "void main(){\n" +
        	ssb.vertexChunk["homogenizeVertex"] +
        	ssb.vertexChunk["modelSpace"] +
        	ssb.vertexChunk["worldSpace"] +
        	ssb.vertexChunk["nDCSpace"] +
        	ssb.vertexChunk["gl_Position_n"] +
        	ssb.vertexChunk["outDepthPosition"]+
        	ssb.vertexChunk["outWorldSpacePosition"] +
        	ssb.vertexChunk["outVertexNormal"] +
        	ssb.vertexChunk["outVertexUV"] +
        	ssb.vertexChunk["outVertexTangent"] +
        "}\n"
	);
	this->setFragmentShaderSource(
		ssb.uniform["material"] +
		ssb.uniform["dirLights"] +
		ssb.uniform["pLights"] +
		ssb.uniform["ambientLight"] +
		ssb.uniform["colorMap"] +
		ssb.uniform["normalMap"] +
		ssb.uniform["maxLightIntensity"]+
		ssb.uniform["invGamma"] +
		ssb.uniform["shadowMap"] +
		ssb.uniform["PCFShadow"] +
		"\n" +
		ssb.attribute["depthPosition"] +
		ssb.attribute["vertexNormal"] +
		ssb.attribute["worldSpacePosition"] +
		ssb.attribute["vertexUV"] +
		ssb.attribute["vertexTangent"] +
		"\n" +
		ssb.output["color"] +
		"\n" +
		ssb.helpers["attenuateLightFunc"] +
		"\n"+
		ssb.helpers["PCFShadowFactorFunc"] +
		"\n" +
        ssb.helpers["shadowFactorFunc"] +
		"\n" +
		ssb.helpers["calculateNormalFunc"] +
		"\n" +
		ssb.helpers["warpFunc"] +
        "\n" +
        ssb.helpers["calculateCosAngIncidenceFunc"] +
        "\n" +
        ssb.helpers["calculateBlinnPhongTermFunc"] +
		"\n" +
        "void main(){\n" +
            ssb.fragmentChunk["viewDirection"] +
            ssb.fragmentChunk["initOutputColor"] +
            ssb.fragmentChunk["materialDiffuseColor"] +
            ssb.fragmentChunk["materialSpecularColor"] +
            ssb.fragmentChunk["interpolatedNormal"] +
			"\n" +
			ssb.fragmentChunk["mapDiffuseColor"] +
			ssb.fragmentChunk["mapNormal"] +
			ssb.fragmentChunk["initShadowFactor"] +
            "#ifdef SHADOWMAP\n"
            "#ifdef PCFSHADOW\n"
            "shadowFactor = calculateShadowFactorPCF(depthPosition);\n"
            "#else\n"
            "shadowFactor = calculateShadowFactor(depthPosition);\n"
            "#endif\n"
            "#endif\n"
			"\n" +
			ssb.fragmentChunk["addDirectionalLightsFactor"] +
			ssb.fragmentChunk["addPointLightsFactor"] +
            ssb.fragmentChunk["addAmbientLightFactor"] +
			ssb.fragmentChunk["gammaCorrection"] +
        "}\n"
	);
}

void PhongMaterial::makeDeferredShader(){
	auto ssb = ShaderSourceBuilder();
	this->setVertexShaderSource(
		ssb.attribute["normal"] +
		ssb.attribute["position"]+
        ssb.attribute["uv"] +
        ssb.attribute["tangent"] +
		"\n" + 
		ssb.output["vertexNormal"] +
		ssb.output["worldSpacePosition"] +
		ssb.output["vertexUV"] +
		ssb.output["vertexTangent"] +
		"\n" +
		ssb.uniform["modelMatrix"] +
		ssb.uniform["worldMatrix"] +
		ssb.uniform["projectionMatrix"] +
        "void main(){\n" +
        	ssb.vertexChunk["homogenizeVertex"] +
        	ssb.vertexChunk["modelSpace"] +
        	ssb.vertexChunk["worldSpace"] +
        	ssb.vertexChunk["nDCSpace"] +
        	ssb.vertexChunk["gl_Position_n"] +
        	ssb.vertexChunk["outWorldSpacePosition"] +
        	ssb.vertexChunk["outVertexNormal"] +
        	ssb.vertexChunk["outVertexUV"] +
        	ssb.vertexChunk["outVertexTangent"] +
        "}\n"
	);
	this->setFragmentShaderSource(
		ssb.uniform["material"] +
		ssb.uniform["colorMap"] +
		ssb.uniform["normalMap"] +
		"\n" +
		ssb.attribute["vertexNormal"] +
		ssb.attribute["worldSpacePosition"] +
		ssb.attribute["vertexUV"] +
		ssb.attribute["vertexTangent"] +
		"\n" +
		ssb.output["color"] +
		ssb.output["normal"]+
		ssb.output["position"]+
		ssb.output["uv"]+
		"\n" +
		ssb.helpers["calculateNormalFunc"] +
		"\n" +
        "void main(){\n" +
            ssb.fragmentChunk["viewDirection"] +
            ssb.fragmentChunk["initOutputColor"] +
            ssb.fragmentChunk["materialDiffuseColor"] +
            ssb.fragmentChunk["interpolatedNormal"] +
			"\n" +
			ssb.fragmentChunk["mapDiffuseColor"] +
			ssb.fragmentChunk["mapNormal"] +
			ssb.fragmentChunk["outputColorTarget"]+
			ssb.fragmentChunk["outputNormalTarget"]+
			ssb.fragmentChunk["outputPositionTarget"]+
			ssb.fragmentChunk["outputUVTarget"]+
        "}\n"
	);
}

void PhongMaterial::makePrograms(const Scene& scene, bool deferred){
	if(deferred){
		this->makeDeferredShader();
	}else{
		this->makeForwardShader();
	}
	this->setProgram(shared_ptr<GLProgram>(new GLProgram()));
	string vs = this->configureSource(
		this->getVertexShaderSource(),
		scene.getDirectionalLights().size(),
		scene.getPointLights().size(),
		false,
		false,
		scene.getUsesShadows(),
		false
	);
	string fs = this->configureSource(
		this->getFragmentShaderSource(),
		scene.getDirectionalLights().size(),
		scene.getPointLights().size(),
		(bool)this->map,
		(bool)this->normalMap,
		scene.getUsesShadows(),
		scene.getPCFShadows()
	);
	cout << vs << endl << fs << endl;
	int vertexShader = this->getProgram()->compileShader(GL_VERTEX_SHADER,vs);
	this->getProgram()->setVertexShader(vertexShader);
	int fragmentShader = this->getProgram()->compileShader(GL_FRAGMENT_SHADER,fs);
	this->getProgram()->setFragmentShader(fragmentShader);
	int prog = this->getProgram()->linkProgram(vertexShader,fragmentShader);
	this->program->setProgram(prog);
	this->getProgram()->setAttrPosition(1);
	glBindAttribLocation(prog,1,"position");
	this->getProgram()->setAttrNormal(0);
	glBindAttribLocation(prog,0,"normal");
	this->getProgram()->setAttrUV(2);
	glBindAttribLocation(prog,2,"uv");
	this->getProgram()->setAttrTangent(3);
	glBindAttribLocation(prog,3,"tangent");
	this->getProgram()->getUniforms()->unifModelMatrix = glGetUniformLocation(prog,"modelMatrix");
	this->getProgram()->getUniforms()->unifDiffuseColor = glGetUniformLocation(prog,"material.diffuseColor");
	this->getProgram()->getUniforms()->unifSpecularColor = glGetUniformLocation(prog,"material.specularColor");
	this->getProgram()->getUniforms()->unifMaxLightIntensity = glGetUniformLocation(prog,"maxLightIntensity");
	this->getProgram()->getUniforms()->unifInvGamma = glGetUniformLocation(prog,"invGamma");
	this->getProgram()->getUniforms()->unifShininess = glGetUniformLocation(prog,"material.shininess");
	this->getProgram()->getUniforms()->unifWorldMatrix = glGetUniformLocation(prog,"worldMatrix");
	this->getProgram()->getUniforms()->unifProjectionMatrix = glGetUniformLocation(prog,"projectionMatrix");
	if(scene.getDirectionalLights().size() > 0){
		this->getProgram()->getUniforms()->unifDirLightColor = glGetUniformLocation(prog,"dirLightColor");
        this->getProgram()->getUniforms()->unifDirLightVectorToLight = glGetUniformLocation(prog,"dirLightVectorToLight");
        this->getProgram()->getUniforms()->unifDirLightIntensity = glGetUniformLocation(prog,"dirLightIntensity");
	}
	this->getProgram()->getUniforms()->unifAmbientLight = glGetUniformLocation(prog,"ambientLight");
	if(scene.getPointLights().size()){
        this->getProgram()->getUniforms()->unifPointLightColor = glGetUniformLocation(prog,"pointLightColor");
        this->getProgram()->getUniforms()->unifPointLightPosition = glGetUniformLocation(prog,"pointLightPosition");
        this->getProgram()->getUniforms()->unifPointLightIntensity = glGetUniformLocation(prog,"pointLightIntensity");
        this->getProgram()->getUniforms()->unifPointLightAttenuation = glGetUniformLocation(prog,"pointLightAttenuation");
	}
	if(scene.getUsesShadows()){
        this->getProgram()->getUniforms()->unifDepthWorldMatrix = glGetUniformLocation(prog,"depthWorldMatrix");
        this->getProgram()->getUniforms()->unifShadowMapSampler = glGetUniformLocation(prog, "shadowMap");
        this->getProgram()->getUniforms()->unifShadowMapSize = glGetUniformLocation(prog,"shadowMapSize");
        this->getProgram()->getUniforms()->unifSampleSize = glGetUniformLocation(prog,"sampleSize");
	}
	if(this->map){
		this->getProgram()->getUniforms()->unifMapSampler = glGetUniformLocation(prog, "colorMap");
	}
	if(this->normalMap){
		this->getProgram()->getUniforms()->unifNormalMapSampler = glGetUniformLocation(prog, "normalMap");
	}
}
}