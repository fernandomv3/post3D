#include "post3D/Texture.h"
#include "post3D/MathUtils.h"
#include "SDL2/SDL_image.h"

using namespace math3D;

namespace texture{
Texture::Texture(){
  this->uuid= generateUUID();
  this->sourceFile= "";
  this->image= std::unique_ptr<char[]>();
  this->height= 0;
  this->width= 0;
  this->gamma= true;
  this->target= GL_TEXTURE_2D;
  this->innerFormat= GL_SRGB8;
  this->format= GL_RGBA;
  this->type= GL_UNSIGNED_BYTE;

  this->texture = 0;
  this->samplerObject = 0;
}

/*Texture::Texture(const Texture& texture){
  this->uuid= generateUUID();
  this->sourceFile= texture.sourceFile;
  this->height= texture.height;
  this->width= texture.width;
  this->gamma= texture.gamma;
  this->target= texture.target;
  this->innerFormat= texture.innerFormat;
  this->format= texture.format;
  this->type= texture.type;
  for(int i = 0 ; i < texture.width * texture.height; i++){
    this->image[i] = texture.image[i];
  }
}*/

Texture::Texture(Texture&& texture){
  this->uuid= generateUUID();
  this->sourceFile= texture.sourceFile;
  this->height= texture.height;
  this->width= texture.width;
  this->gamma= texture.gamma;
  this->target= texture.target;
  this->innerFormat= texture.innerFormat;
  this->format= texture.format;
  this->type= texture.type;
  this->image = std::move(texture.image);

  this->texture = texture.texture;
  this->samplerObject = texture.samplerObject;
}

/*Texture& Texture::operator=(const Texture& texture){
  this->uuid= generateUUID();
  this->sourceFile= texture.sourceFile;
  this->height= texture.height;
  this->width= texture.width;
  this->gamma= texture.gamma;
  this->target= texture.target;
  this->innerFormat= texture.innerFormat;
  this->format= texture.format;
  this->type= texture.type;
  for(int i = 0 ; i < texture.width * texture.height; i++){
    this->image[i] = texture.image[i];
  }
  return *this;
}*/

Texture& Texture::operator=(Texture&& texture){
  this->uuid= generateUUID();
  this->sourceFile= texture.sourceFile;
  this->height= texture.height;
  this->width= texture.width;
  this->gamma= texture.gamma;
  this->target= texture.target;
  this->innerFormat= texture.innerFormat;
  this->format= texture.format;
  this->type= texture.type;
  this->image = std::move(texture.image);

  this->texture = texture.texture;
  this->samplerObject = texture.samplerObject;
  return *this;
}

Texture::~Texture(){
}

const std::string& Texture::getUUID()const{
  return this->uuid;
}

const std::string& Texture::getSourceFile()const{
  return this->sourceFile;
}

const char* Texture::getImage()const{
  return this->image.get();
}

int Texture::getWidth()const{
  return this->width;
}

int Texture::getHeight()const{
  return this->height;
}

bool Texture::getGamma()const{
  return this->gamma;
}

int Texture::getTarget()const{
  return this->target;
}

int Texture::getInnerFormat()const{
  return this->innerFormat;
}

int Texture::getFormat()const{
  return this->format;
}

int Texture::getType()const{
  return this->type;
}

Texture& Texture::setSourceFile(std::string sourceFile){
  this->sourceFile = sourceFile;
  return *this;
}

Texture& Texture::setHeight(int height){
  this->height = height;
  return *this;
}

Texture& Texture::setWidth(int width){
  this->width = width;
  return *this;
}

Texture& Texture::setGamma(bool gamma){
  this->gamma = gamma;
  return *this;
}

Texture& Texture::setTarget(int target){
  this->target = target;
  return *this;
}

Texture& Texture::setInnerFormat(int innerFormat){
  this->innerFormat = innerFormat;
  return *this;
}

Texture& Texture::setFormat(int format){
  this->format = format;
  return *this;
}

Texture& Texture::setType(int type){
  this->type = type;
  return *this;
}

Texture& Texture::loadFile(){
  SDL_Surface* image = IMG_Load(this->sourceFile.c_str());
  this->width = image->w;
  this->height = image->h;
  void* pixels = image->pixels;

  if(image->format->BytesPerPixel == 3) {this->format = GL_RGB;}
  else if(image->format->BytesPerPixel == 4) {this->format = GL_RGBA;}
  char* data = new char[image->pixels,image->w * image->h * image->format->BytesPerPixel];
  memcpy(data,image->pixels,image->w * image->h * image->format->BytesPerPixel);
  this->image = std::unique_ptr<char[]>(data);

  this->innerFormat = this->gamma ? GL_SRGB8 : GL_RGB8;
  
  if(image){
    SDL_FreeSurface(image);
  }
  
  return *this;
}


Texture& Texture::loadFile(const std::string& src, bool gamma){
    SDL_Surface* image = IMG_Load(src.c_str());
    this->width = image->w;
    this->height = image->h;
    void* pixels = image->pixels;
    GLenum imgFormat;
    if(image->format->BytesPerPixel == 3) {imgFormat = GL_RGB;}
    else if(image->format->BytesPerPixel == 4) {imgFormat = GL_RGBA;}

    this->texture = this->createTexture(
      GL_TEXTURE_2D,
      this->width,
      this->height,
      gamma ? GL_SRGB8 : GL_RGB8,
      imgFormat,
      reinterpret_cast<char*>(pixels)
    );

    if(image){
      SDL_FreeSurface(image);
    }

    return *this;
  }

  int Texture::makeSampler(){
    int sampler;
    glGenSamplers(1,(GLuint*)&sampler);
        glSamplerParameteri(sampler, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glSamplerParameteri(sampler, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glSamplerParameteri(sampler, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glSamplerParameteri(sampler, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        return sampler;
  }

  int Texture::createTexture(GLenum target,int width, int height,GLenum innerFormat,GLenum format,char* data,GLenum type){
        int tex;
        glGenTextures(1,(GLuint*)&tex);
        glBindTexture(target,tex);
        glTexImage2D(
            target, 0,           //target, level of detail
            innerFormat,                    //internal format
            width, height, 0,           //width, height, border
            format, type,   //external format, type 
            data                      //pixels
        );
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S,GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T,GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_BASE_LEVEL, 0);
        glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MAX_LEVEL, 0);
        if (innerFormat == GL_DEPTH_COMPONENT){
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_REF_TO_TEXTURE);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_FUNC, GL_LEQUAL);
        }
        glBindTexture(target,0);
        return tex;
  }
  Texture& Texture::setTexture(int texture){
    this->texture = texture;
    return *this;
  }

  Texture& Texture::setSampler(int sampler){
    this->samplerObject = sampler;
    return *this;
  }
  int Texture::getTexture()const{
    return this->texture;
  }

  int Texture::getSampler()const{
    return this->samplerObject;
  }

}