#ifndef TEXTURE_H
#define TEXTURE_H

#include <string>
#include <GL/glew.h>
#include <memory>

namespace texture{
class Texture{
private:
  std::string uuid;
  std::string sourceFile;
  std::unique_ptr<char[]>image;
  int height;
  int width;
  bool gamma;
  int target;
  int innerFormat;
  int format;
  int type;
public:
  Texture();
  Texture(const Texture& texture)=delete;
  Texture(Texture&& texture);
  Texture& operator=(const Texture& texture)=delete;
  Texture& operator=(Texture&& texture);
  ~Texture();

  const std::string& getUUID()const;
  const std::string& getSourceFile()const;
  const char* getImage()const;
  int getHeight()const;
  int getWidth()const;
  bool getGamma()const;
  int getTarget()const;
  int getInnerFormat()const;
  int getFormat()const;
  int getType()const;

  Texture& setSourceFile(std::string sourceFile);
  Texture& setHeight(int height);
  Texture& setWidth(int widht);
  Texture& setGamma(bool gamma);
  Texture& setTarget(int target);
  Texture& setInnerFormat(int innerFormat);
  Texture& setFormat(int format);
  Texture& setType(int type);

  Texture& loadFile();
};
}
#endif