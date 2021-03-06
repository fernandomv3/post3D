﻿#include "post3D/DepthMaterial.h"
#include "post3D/GLProgram.h"
#include <GL/glew.h>
#include <memory>
#include <string>

using namespace std;
namespace material{
	DepthMaterial::DepthMaterial():Material(){
		this->setVertexShaderSource(
			"#version 330 core\n"
	        "layout(location = 1)in vec3 position;\n"
	        "out vec4 vertexPosition;\n"
	        "uniform mat4 worldMatrix;\n"
	        "uniform mat4 projectionMatrix;\n"
	        "uniform mat4 modelMatrix;\n"
	        "void main(){\n"
	        "    vec4 pos = vec4(position,1.0);\n"
	        "    vertexPosition = pos;\n"
	        "    vec4 modelSpace = modelMatrix * pos;\n"
	        "    vec4 worldSpace = worldMatrix * modelSpace;\n"
	        "    gl_Position = projectionMatrix * worldSpace;\n"
	        "}\n"
		);
		this->setFragmentShaderSource(
			"#version 330 core\n"
			"in vec4 vertexPosition;\n"
	        "layout(location = 0)out vec4 outputColor;\n"
	        "uniform vec4 diffuseColor;\n"
	        "#define MAP\n"
	        "#ifdef MAP\n"
	        "   uniform sampler2D colorMap;\n"
	        "#endif\n"
	        "\n"
	        "void main(){\n"
	        "    float depth = texture(colorMap,vertexPosition.xy).x;\n"
	        "    depth = 1.0 - (1.0 - depth) *25.0;\n"
	        "    outputColor= vec4(depth);\n"
	        "}\n"
		);
	}

	void DepthMaterial::makePrograms(const Scene& scene, bool deferred){
		this->setProgram(shared_ptr<GLProgram>(new GLProgram()));
		int vertexShader = this->getProgram()->compileShader(GL_VERTEX_SHADER,this->getVertexShaderSource());
		this->getProgram()->setVertexShader(vertexShader);
		int fragmentShader = this->getProgram()->compileShader(GL_FRAGMENT_SHADER,this->getFragmentShaderSource());
		this->getProgram()->setFragmentShader(fragmentShader);
		int prog = this->getProgram()->linkProgram(vertexShader,fragmentShader);
		this->getProgram()->setProgram(prog);
		this->getProgram()->setAttrPosition(1);
		glBindAttribLocation(prog,1,"position");
		this->getProgram()->getUniforms()->unifModelMatrix = glGetUniformLocation(prog,"modelMatrix");
		this->getProgram()->getUniforms()->unifWorldMatrix = glGetUniformLocation(prog,"worldMatrix");
		this->getProgram()->getUniforms()->unifProjectionMatrix = glGetUniformLocation(prog,"projectionMatrix");
		if(this->map){
			this->getProgram()->getUniforms()->unifMapSampler = glGetUniformLocation(prog, "colorMap");
		}
	}
}