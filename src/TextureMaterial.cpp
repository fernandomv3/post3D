#include "post3D/TextureMaterial.h"
#include "post3D/GLProgram.h"
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
	        "	outputColor= texture2D(colorMap,vertexPosition.xy * 0.5 + 0.5);\n"
	        "}\n"
		);
	}

	void TextureMaterial::makePrograms(const Scene& scene, bool deferred){
		this->setProgram(shared_ptr<GLProgram>(new GLProgram()));
		Shader vertex, fragment;
		vertex.type = "vertex";
		fragment.type = "fragment";
		vertex.source = this->getVertexShaderSource();
		fragment.source = this->getFragmentShaderSource();

		this->program->getShaders().push_back(vertex);
		this->program->getShaders().push_back(fragment);

		this->program->makeProgram();
		int prog = this->program->getProgram();

		this->getProgram()->setAttrPosition(1);
		glBindAttribLocation(prog,1,"position");
		auto &uniforms = this->program->getpUniforms();
		uniforms["diffuseColor"].location = glGetUniformLocation(prog,"diffuseColor");
		if(this->map){
			uniforms["mapSampler"].location = glGetUniformLocation(prog, "colorMap");
		}
	}
}