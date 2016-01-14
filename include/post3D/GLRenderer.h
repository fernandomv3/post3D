#ifndef GLRENDERER_H
#define GLRENDERER_H

#include "post3D/Framebuffer.h"
#include "post3D/Renderer.h"
#include <GL/glew.h>
#include <memory>

namespace render{

/*struct gltexture{
  int texture;
  int sampler;
  int binding;
  std::string name;
};
typedef struct gltexture GLTexture;
*/
struct geometryBuffers{
  int vertex = -1;
  int normal = -1;
  int uv = -1;
  int tangent = -1;
};
typedef struct geometryBuffers GLBuffers;

class GLRenderer:public Renderer{
private:
  unique_ptr<Framebuffer> fb;
  std::unordered_map<std::string,GLTexture> shadowMaps;
  std::unordered_map<std::string,GLTexture> renderChannels;
  std::unordered_map<std::string,GLProgram> programs;
  std::unordered_map<std::string,GLBuffers> buffers;
  std::unordered_map<std::string,GLTexture> textures;
  std::shared_ptr<Camera> activeCamera;
public:
  GLRenderer();
  GLRenderer(const GLRenderer& renderer)=delete;
  GLRenderer(GLRenderer&& renderer)=default;
  GLRenderer& operator=(const GLRenderer& renderer)=delete;
  GLRenderer& operator=(GLRenderer&& renderer)=default;
  ~GLRenderer();
  Renderer& render(Scene& scene);
};
/*int makeBuffer(GLenum target, void* data, int size, GLenum usage = GL_STATIC_DRAW);
*/}
#endif