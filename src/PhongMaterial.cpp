#include "post3D/PhongMaterial.h"
#include "post3D/GLProgram.h"
#include <GL/glew.h>
#include <memory>
#include <string>
#include <iostream>

using namespace std;
namespace material{
	PhongMaterial::PhongMaterial():Material(){
		this->setVertexShaderSource(
	        "layout(location = 0)in vec3 normal;\n"
	        "layout(location = 1)in vec3 position;\n"
	        "layout(location = 2)in vec2 uv;\n"
	        "layout(location = 3)in vec3 tangent;\n"
			"\n"
	        "out vec4 vertexNormal;\n"
	        "out vec4 worldSpacePosition;\n"
	        "out vec2 vertexUV;\n"
	        "out vec4 vertexTangent;\n"
			"\n"
	        "#ifdef SHADOWMAP\n"
	        "out vec4 depthPosition;\n"
	        "#endif\n"
			"\n"
	        "uniform mat4 modelMatrix;\n"
	        "uniform mat4 worldMatrix;\n"
	        "uniform mat4 projectionMatrix;\n"
			"\n"
	        "#ifdef SHADOWMAP\n"
	        "uniform mat4 depthWorldMatrix;\n"
	        "#endif\n"
			"\n"
	        "void main(){\n"
	            "vec4 pos = vec4(position,1.0);\n"
	            "vec4 modelSpace = modelMatrix * pos;\n"
	            "vec4 worldSpace = worldMatrix * modelSpace;\n"
	            "gl_Position = projectionMatrix * worldSpace;\n"
	            "#ifdef SHADOWMAP\n"
	            "depthPosition = depthWorldMatrix * modelMatrix *pos;\n"
	            "#endif\n"
	            "worldSpacePosition = worldSpace;\n"
	            "vertexNormal = normalize(worldMatrix * modelMatrix * vec4(normal,0.0));\n"
	            "vertexUV = uv;\n"
	            "vertexTangent = normalize(worldMatrix * modelMatrix * vec4(tangent,0.0));\n"
	        "}\n"
		);
		this->setFragmentShaderSource(
	        "struct Material{\n"
	            "vec4 diffuseColor;\n"
	            "vec4 specularColor;\n"
	            "float shininess;\n"
	        "};\n"
			"\n"
	        "#if MAX_DIR_LIGHTS > 0\n"
	        "uniform vec4 dirLightColor[MAX_DIR_LIGHTS];\n"
	        "uniform vec4 dirLightVectorToLight[MAX_DIR_LIGHTS];\n"
	        "uniform float dirLightIntensity[MAX_DIR_LIGHTS];\n"
	        "#endif\n"
			"\n"
	        "#if MAX_P_LIGHTS > 0\n"
	        "uniform vec4 pointLightColor[MAX_P_LIGHTS];\n"
	        "uniform vec4 pointLightPosition[MAX_P_LIGHTS];\n"
	        "uniform float pointLightIntensity[MAX_P_LIGHTS];\n"
	        "uniform float pointLightAttenuation[MAX_P_LIGHTS];\n"
	        "#endif\n"
			"\n"
	        "uniform vec4 ambientLight;\n"
			"\n"
	        "#ifdef MAP\n"
	            "uniform sampler2D colorMap;\n"
	        "#endif\n"
			"\n"
	        "#ifdef NORMALMAP\n"
	        "uniform sampler2D normalMap;\n"
	        "#endif\n"
			"\n"
	        "uniform Material material;\n"
	        "uniform float maxLightIntensity;\n"
	        "uniform float invGamma;\n"
			"\n"
	        "#ifdef SHADOWMAP\n"
	        "uniform sampler2D shadowMap;\n"
	        "uniform vec2 shadowMapSize;\n"
	        "uniform int sampleSize;\n"
	        "in vec4 depthPosition;\n"
	        "#endif\n"
	        "in vec4 vertexNormal;\n"
	        "in vec4 worldSpacePosition;\n"
	        "in vec2 vertexUV;\n"
	        "in vec4 vertexTangent;\n"
	        "out vec4 outputColor;\n"
			"\n"
	        "#if MAX_P_LIGHTS > 0\n"
	        "vec4 attenuateLight(in vec4 color, in float attenuation, in vec4 vectorToLight){\n"
	            "float distSqr = dot(vectorToLight,vectorToLight);\n"
	            "vec4 attenLightIntensity = color * (1/(1.0 + attenuation * sqrt(distSqr)));\n"
	            "return attenLightIntensity;\n"
	        "}\n"
	        "#endif\n"
			"\n"
	        "#ifdef SHADOWMAP\n"
	        "#ifdef PCFSHADOW\n"
	        "float calculateShadowFactorPCF(in vec4 lightSpacePos){\n"
	            "vec3 projCoords = lightSpacePos.xyz / lightSpacePos.w;\n"
	            "vec3 uv = 0.5 * projCoords + 0.5;\n"
	            "vec2 mapSize;\n"
	            "vec2 offset = vec2(1.0/shadowMapSize.x,1.0/shadowMapSize.y);            \n"
	            "float result = 0.0;\n"
	            "for(int i = -sampleSize; i< sampleSize;i++){\n"
	                "for(int j = -sampleSize; j < sampleSize; j++){\n"
	                    "vec2 offsets = vec2(i*offset.x , j*offset.y);\n"
	                    "vec3 uvc = vec3(uv.xy +offsets, uv.z + 0.00001);\n"
	                    "float mapDepth = texture(shadowMap,uv.xy+offsets).x;\n"
	                    "if( mapDepth < uv.z + 0.00001){\n"
	                        "result += 0.0;\n"
	                    "}\n"
	                    "else{\n"
	                        "result += 1.0;\n"
	                    "}\n"
	                "}\n"
	            "}\n"
	            "float numSamples = (1 + 2*sampleSize)*(1 + 2*sampleSize);\n"
	            "return 0.5 + (result/numSamples);\n"
	        "}\n"
	        "#else\n"
	        "float calculateShadowFactor(in vec4 lightSpacePos){\n"
	            "vec3 projCoords = lightSpacePos.xyz / lightSpacePos.w;\n"
	            "vec3 uv = 0.5 * projCoords + 0.5;\n"
	            "float depth = texture(shadowMap,uv.xy).x;\n"
	            "if (depth < uv.z + 0.00001){\n"
	                "return 0.5;\n"
	            "}\n"
	            "return 1.0;\n"
	        "}\n"
	        "#endif\n"
	        "#endif\n"
			"\n"
			"\n"
	        "vec4 calculateNormal(in vec4 vNormal,in vec4 vTangent, in vec2 uv ,in sampler2D normalMap){\n"
	            "vec3 normal = normalize(vNormal).xyz;\n"
	            "vec3 tangent = normalize(vTangent).xyz;\n"
	            "tangent = normalize(tangent - dot(tangent,normal) * normal);\n"
	            "vec3 bitangent = -cross(tangent,normal);\n"
	            "vec3 texNormal = 255 * texture(normalMap,uv).xyz /128 - vec3(1.0,1.0,1.0);\n"
	            "mat3 TBN = mat3(tangent, bitangent, normal);\n"
	            "texNormal = TBN * texNormal;\n"
	            "texNormal = normalize(texNormal);\n"
	            "return vec4(texNormal,0.0);\n"
	        "}\n"
			"\n"
	        "float warp (in float value,in float factor){\n"
	            "return (value + factor ) / (1+ clamp(factor,0,1));\n"
	        "}\n"
	        "\n"
	        "float calculateCosAngIncidence(in vec4 direction, in vec4 normal,in float warpFactor){\n"
	        	"//use warpFactor = 0 for no warp\n"
	        	"float cosAngIncidence = dot(normal,direction);\n"
	        	"cosAngIncidence = warp(cosAngIncidence,warpFactor);\n"
	        	"cosAngIncidence = clamp(cosAngIncidence,0,1);\n"
	        	"return cosAngIncidence;\n"
	        "}\n"
	        "\n"
	        "float calculateBlinnPhongTerm(in vec4 direction,vec4 normal, in vec4 viewDirection, in float shininess, out float cosAngIncidence){\n"
	            "cosAngIncidence = calculateCosAngIncidence(normal,direction,0);\n"
	            "vec4 halfAngle = normalize(direction + viewDirection);\n"
	            "float blinnPhongTerm = dot(normal, halfAngle);\n"
	            "blinnPhongTerm = clamp(blinnPhongTerm, 0, 1);\n"
	            "blinnPhongTerm = cosAngIncidence != 0.0 ? blinnPhongTerm : 0.0;\n"
	            "blinnPhongTerm = pow(blinnPhongTerm, shininess);\n"
	            "return blinnPhongTerm;\n"
	        "}\n"
			"\n"
	        "void main(){\n"
	            "vec4 viewDirection = normalize(-worldSpacePosition);\n"
	            "outputColor = vec4(0.0,0.0,0.0,1.0);\n"
	            "vec4 diffuseColor = material.diffuseColor;\n"
	            "vec4 specularColor = material.specularColor;\n"
	            "vec4 normal = normalize(vertexNormal);\n"
				"\n"
	            "#ifdef MAP\n"
	            "diffuseColor = texture(colorMap,texCoord);\n"
	            "#endif\n"
	            "#ifdef NORMALMAP\n"
	            "normal = calculateNormal(vertexNormal,vertexTangent,vertexUV,normalMap);\n"
	            "#endif\n"
	            "float shadowFactor = 1.0;\n"
	            "#ifdef SHADOWMAP\n"
	            "#ifdef PCFSHADOW\n"
	            "shadowFactor = calculateShadowFactorPCF(depthPosition);\n"
	            "#else\n"
	            "shadowFactor = calculateShadowFactor(depthPosition);\n"
	            "#endif\n"
	            "#endif\n"
				"\n"
	            "#if MAX_DIR_LIGHTS > 0\n"
	            "for(int i=0; i< MAX_DIR_LIGHTS ;i++){\n"
	                "vec4 normDirection = normalize(dirLightVectorToLight[i]);\n"
	                "float cosAngIncidence;\n"
	                "float blinnPhongTerm = calculateBlinnPhongTerm(normDirection,normal,viewDirection,material.shininess,cosAngIncidence);\n"
	                "outputColor = outputColor + (shadowFactor * dirLightColor[i] * diffuseColor * cosAngIncidence * dirLightIntensity[i] / maxLightIntensity);\n"
	                "outputColor = outputColor + (shadowFactor * dirLightColor[i] * specularColor * blinnPhongTerm * dirLightIntensity[i] / maxLightIntensity);\n"
	            "}\n"
	            "#endif\n"
	            "#if MAX_P_LIGHTS > 0\n"
	            "for(int i=0; i< MAX_P_LIGHTS ;i++){\n"
	                "vec4 difference = pointLightPosition[i] - worldSpacePosition;\n"
	                "vec4 normDirection = normalize(difference);\n"
	                "vec4 attenLightIntensity = attenuateLight(pointLightColor[i],pointLightAttenuation[i],difference);\n"
	                "float cosAngIncidence;\n"
	                "float blinnPhongTerm = calculateBlinnPhongTerm(normDirection,normal,viewDirection,material.shininess,cosAngIncidence);\n"
					"\n"
	                "outputColor = outputColor + (attenLightIntensity * diffuseColor * cosAngIncidence * pointLightIntensity[i] / maxLightIntensity);\n"
	                "outputColor = outputColor + (specularColor * attenLightIntensity * blinnPhongTerm * pointLightIntensity[i] / maxLightIntensity);\n"
	            "}\n"
	            "#endif\n"
	            "outputColor = outputColor + (diffuseColor * ambientLight);\n"
	            "vec4 gamma = vec4(invGamma);\n"
	            "gamma.w = 1.0;\n"
	            "outputColor = pow(outputColor,gamma);\n"
	        "}\n"
		);
	}

	void PhongMaterial::makePrograms(const Scene& scene){
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
		int vertexShader = this->getProgram()->compileShader(GL_VERTEX_SHADER,vs);
		this->getProgram()->setVertexShader(vertexShader);
		int fragmentShader = this->getProgram()->compileShader(GL_FRAGMENT_SHADER,fs);
		this->getProgram()->setFragmentShader(fragmentShader);
		//cout << fs << endl;
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