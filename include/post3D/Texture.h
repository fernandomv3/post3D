#ifndef TEXTURE_H
#define TEXTURE_H

#include <string>
#include <GL/glew.h>
#include <memory>

using namespace std;
namespace texture{
	class Texture{
	private:
		string uuid;
		string sourceFile;
		int height;
		int width;
		int texture;
		unique_ptr<int[]>image; //use vector
		int samplerObject;
	public:
		Texture();
		Texture(const Texture& texture)=delete;
		Texture(Texture&& texture);
		Texture& operator=(const Texture& texture)=delete;
		Texture& operator=(Texture&& texture);
		~Texture();
		string getUUID()const;
		Texture& loadFile(const string& src, bool gamma = true);
		int makeSampler();
		int createTexture(GLenum target = GL_TEXTURE_2D,int width =0, int height = 0,GLenum innerFormat=GL_SRGB8,GLenum format=GL_SRGB8,char* data = nullptr, GLenum type = GL_UNSIGNED_BYTE);
		
		int getHeight()const;
		int getWidth()const;
		int getTexture()const;
		int getSampler()const;
		string getSourceFile()const;

		Texture& setHeight(int height);
		Texture& setWidth(int widht);
		Texture& setTexture(int texture);
		Texture& setSampler(int sampler);
		Texture& setSourceFile(string sourceFile);

	};
}
#endif