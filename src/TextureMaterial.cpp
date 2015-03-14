#include "TextureMaterial.h"
#include "GLProgram.h"
#include <GL/glew.h>
#include <memory>
#include <string>
#include <iostream>

using namespace std;
namespace material{
	TextureMaterial::TextureMaterial():Material(){
		this->setVertexShaderSource(
			"#version 330 core\n"
	        "layout(location = 1)in vec3 position;\n"
	        "out vec4 vertexPosition;\n"
	        "\n"
	        "void main(){\n"
	        "    vertexPosition = vec4(position,1.0);\n"
	        "    \n"
	        "    gl_Position = vertexPosition;\n"
	        "}\n"
		);
		this->setFragmentShaderSource(
			"#version 330 core\n"
	        "in vec4 vertexPosition;\n"
	        "layout(location = 0)out vec4 outputColor;\n"
	        "uniform vec4 diffuseColor;\n"
	        "#define MAP\n"
	        "#ifdef MAP\n"
	        "    uniform sampler2D colorMap;\n"
	        "#endif\n"
	        "    \n"
	        "void main(){\n"
	        "	outputColor= vec4(0.0,0.0,0.0,0.0);//diffuseColor;\n"
	        "	//outputColor= texture2D(colorMap,vertexPosition.xy * 0.5 + 0.5);\n"
	        "}\n"
		);
	}

	void TextureMaterial::makePrograms(const Scene& scene){
		this->setProgram(shared_ptr<GLProgram>(new GLProgram()));
		int vertexShader = this->getProgram()->compileShader(GL_VERTEX_SHADER,this->getVertexShaderSource());
		this->getProgram()->setVertexShader(vertexShader);
		int fragmentShader = this->getProgram()->compileShader(GL_FRAGMENT_SHADER,this->getFragmentShaderSource());
		this->getProgram()->setFragmentShader(fragmentShader);
		int prog = this->getProgram()->linkProgram(vertexShader,fragmentShader);
		this->getProgram()->setAttrPosition(1);
		glBindAttribLocation(prog,1,"position");
		this->getProgram()->getUniforms()->unifDiffuseColor = glGetUniformLocation(prog,"diffuseColor");
		if(this->map){
			this->getProgram()->getUniforms()->unifMapSampler = glGetUniformLocation(prog, "colorMap");
		}
	}
}