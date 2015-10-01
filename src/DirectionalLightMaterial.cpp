#include "post3D/DirectionalLightMaterial.h"
#include "post3D/GLProgram.h"
#include "post3D/ShaderSourceBuilder.h"
#include <GL/glew.h>
#include <memory>
#include <string>
#include <iostream>

using namespace std;
namespace material{
DirectionalLightMaterial::DirectionalLightMaterial():Material(){

}

void DirectionalLightMaterial::makeForwardShader(){
	auto ssb = ShaderSourceBuilder();
	this->setVertexShaderSource(
		ssb.attribute["position"] +
		"\n" +
        "void main(){\n" +
        	ssb.vertexChunk["homogenizeVertex"] +
        	ssb.vertexChunk["gl_Position_h"] +
        "}\n"
	);
	this->setFragmentShaderSource(
		ssb.uniform["material"] +
		ssb.uniform["dirLight"] +
		"\n" +
		ssb.uniform["deferredPositionMap"]+
		ssb.uniform["deferredWorldPositionMap"]+
		ssb.uniform["deferredColorMap"]+
		ssb.uniform["deferredNormalMap"]+
		ssb.uniform["deferredUvMap"]+
		"\n" +
		ssb.uniform["maxLightIntensity"]+
		ssb.uniform["invGamma"] +
		ssb.uniform["depthWorldMatrix"] +
		ssb.uniform["shadowMap"] +
		ssb.uniform["PCFShadow"] +
		"\n" +
		ssb.attribute["screenSize"] +
		"\n" +
		ssb.output["color"] +
		"\n"+
		ssb.helpers["PCFShadowFactorFunc"] +
		"\n" +
        ssb.helpers["shadowFactorFunc"] +
		"\n" +
		ssb.helpers["warpFunc"] +
        "\n" +
        ssb.helpers["calculateCosAngIncidenceFunc"] +
        "\n" +
        ssb.helpers["calculateBlinnPhongTermFunc"] +
		"\n" +
        "void main(){\n" +
            ssb.fragmentChunk["initOutputColor"] +
            ssb.fragmentChunk["texCoord"] +
			ssb.fragmentChunk["mappedDepthPosition"] +
			ssb.fragmentChunk["mappedViewDirection"] +
			ssb.fragmentChunk["mappedDiffuseColor"] +
            ssb.fragmentChunk["materialSpecularColor"] +
            ssb.fragmentChunk["mappeddNormal"] +
			"\n" +
			ssb.fragmentChunk["initShadowFactor"] +
            "#ifdef SHADOWMAP\n"
            "#ifdef PCFSHADOW\n"
            "shadowFactor = calculateShadowFactorPCF(depthPosition);\n"
            "#else\n"
            "shadowFactor = calculateShadowFactor(depthPosition);\n"
            "#endif\n"
            "#endif\n"
			"\n" +
			ssb.fragmentChunk["directionalLightsFactor"] +
			ssb.fragmentChunk["gammaCorrection"] +
        "}\n"
	);
}

void DirectionalLightMaterial::makeDeferredShader(){
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

void DirectionalLightMaterial::makePrograms(const Scene& scene, bool deferred){
	this->makeForwardShader();
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
	//cout << vs << endl << fs << endl;
	int vertexShader = this->getProgram()->compileShader(GL_VERTEX_SHADER,vs);
	this->getProgram()->setVertexShader(vertexShader);
	int fragmentShader = this->getProgram()->compileShader(GL_FRAGMENT_SHADER,fs);
	this->getProgram()->setFragmentShader(fragmentShader);
	int prog = this->getProgram()->linkProgram(vertexShader,fragmentShader);
	this->program->setProgram(prog);
	this->getProgram()->setAttrPosition(1);
	glBindAttribLocation(prog,1,"position");
	this->getProgram()->getpUniforms()["modelMatrix"].location = glGetUniformLocation(prog,"modelMatrix");
	this->getProgram()->getpUniforms()["diffuseColor"].location = glGetUniformLocation(prog,"material.diffuseColor");
	this->getProgram()->getpUniforms()["specularColor"].location = glGetUniformLocation(prog,"material.specularColor");
	this->getProgram()->getpUniforms()["maxLightIntensity"].location = glGetUniformLocation(prog,"maxLightIntensity");
	this->getProgram()->getpUniforms()["invGamma"].location = glGetUniformLocation(prog,"invGamma");
	this->getProgram()->getpUniforms()["shininess"].location = glGetUniformLocation(prog,"material.shininess");
	this->getProgram()->getpUniforms()["worldMatrix"].location = glGetUniformLocation(prog,"worldMatrix");
	this->getProgram()->getpUniforms()["projectionMatrix"].location = glGetUniformLocation(prog,"projectionMatrix");
	if(scene.getDirectionalLights().size() > 0){
		this->getProgram()->getpUniforms()["dirLightColor"].location = glGetUniformLocation(prog,"dirLightColor");
        this->getProgram()->getpUniforms()["dirLightVectorToLight"].location = glGetUniformLocation(prog,"dirLightVectorToLight");
        this->getProgram()->getpUniforms()["dirLightIntensity"].location = glGetUniformLocation(prog,"dirLightIntensity");
	}
	this->getProgram()->getpUniforms()["ambientLight"].location = glGetUniformLocation(prog,"ambientLight");
	if(scene.getPointLights().size()){
        this->getProgram()->getpUniforms()["pointLightColor"].location = glGetUniformLocation(prog,"pointLightColor");
        this->getProgram()->getpUniforms()["pointLightPosition"].location = glGetUniformLocation(prog,"pointLightPosition");
        this->getProgram()->getpUniforms()["pointLightIntensity"].location = glGetUniformLocation(prog,"pointLightIntensity");
        this->getProgram()->getpUniforms()["pointLightAttenuation"].location = glGetUniformLocation(prog,"pointLightAttenuation");
	}
	if(scene.getUsesShadows()){
        this->getProgram()->getpUniforms()["depthWorldMatrix"].location = glGetUniformLocation(prog,"depthWorldMatrix");
        this->getProgram()->getpUniforms()["shadowMapSampler"].location = glGetUniformLocation(prog, "shadowMap");
        this->getProgram()->getpUniforms()["shadowMapSize"].location = glGetUniformLocation(prog,"shadowMapSize");
        this->getProgram()->getpUniforms()["sampleSize"].location = glGetUniformLocation(prog,"sampleSize");
	}
	if(this->map){
		this->getProgram()->getpUniforms()["mapSampler"].location = glGetUniformLocation(prog, "colorMap");
	}
	if(this->normalMap){
		this->getProgram()->getpUniforms()["normalMapSampler"].location = glGetUniformLocation(prog, "normalMap");
	}
}
}