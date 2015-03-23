#ifndef GBUFFER_H
#define GBUFFER_H

#include "post3D/Texture.h"
#include <vector>
#include <memory>

using namespace std;
using namespace texture;

namespace render{

enum {POSITION_TARGET,NORMAL_TARGET,UV_TARGET,DIFFUSE_TARGET};

/** 
 *  Represents a GBuffer containing all render targets.
 *  
 */
class GBuffer{
private:
  int fbo;/**< Framebuffer Object handle. Stores the OpenGL handle to the frame buffer object */
  vector<shared_ptr<Texture>> textures;/**< Texture render targets. The vector contains Texture objects for each of the render targets*/
  int height;/**< Height in px. Texture height in pixels*/
  int width;/**< Widht in px. Texture width in pixels*/
  shared_ptr<Texture> depthTexture;/**< Texture for depth map. Texture render target for depth values*/
public:
  /**
   * Gbuffer constructor.
   * Creates a GBuffer that contains 4 render targets and a depthmap
   */
  GBuffer();
  /**
   * Gbuffer constructor.
   * Creates a GBuffer that contains 4 render targets and a depthmap with the specified witdh and height
   */
  GBuffer(int width, int height);
  /**
   * Gbuffer copy constructor.
   * Deleted, to create a copy must be done manually and creating new FBO's
   */
  GBuffer(const GBuffer& gbuffer)=delete;
  /**
   * Gbuffer move constructor.
   */
  GBuffer(GBuffer&& gbuffer);
  /**
   * Gbuffer copy assignment.
   */
  GBuffer& operator=(const GBuffer& gbuffer)=delete;
  /**
   * Gbuffer move assignment.
   * Deleted, to create a copy must be done manually and creating new FBO's
   */
  GBuffer& operator=(GBuffer&& gbuffer);
  /**
   * Gbuffer destructor.
   * Deletes all OpenGL buffers.
   */
  ~GBuffer();
  /**
   * Gets the FBO handle.
   * Originally set to 0 by the constructor.
   */
  int getFbo()const;
  /**
   * Gets the Framebuffer height.
   */
  int getHeight()const;
  /**
   * Gets the Framebuffer width.
   */
  int getWidth()const;
  /**
   * Gets the DepthTexture object.
   * @return A Texture object that stores the depthMap wrapped in a shared_ptr for easy resource managment.
   */
  shared_ptr<Texture> getDepthTexture()const;
  /**
   * Gets all the Textures used as render targets.
   * @return Returns a std::vector<Texture> 
   */
  const vector<shared_ptr<Texture>>& getTextures() const;

  /**
   * Sets the Framebuffer object handle.
   */
  GBuffer& setFbo(int fbo);
  /**
   * Sets the Framebuffer height.
   */
  GBuffer& setHeight(int height);
  /**
   * Sets the Framebuffer width.
   */
  GBuffer& setWidth(int width);
  /**
   * Sets the depth Texture.
   * @param depthTexture A Texture object wrapped in a shared_ptr.
   */
  GBuffer& setDepthTexture(shared_ptr<Texture> depthTexture);

  /**
   * Creates the FBO and initializes the Textures. 
   */
  GBuffer& create();
  /**
   * Configures the textures.
   * Sets the formats of the textures and the type of FrameBuffers.
   * @todo Refactor this method and recieve parameters as configuration values
   */
  GBuffer& configure();
  /**
   * Binds the FBO as render target.
   * All render commands will output the results to the Gbuffer.
   */
  GBuffer& bindForWriting();
  /**
   * Binds each texture.
   * Each Texture will be bound to be used by the next rendering stage.
   * @todo Add parameters to this method as configuration values
   */
  GBuffer& bindForReading();
  /**
   * Deactivates the render target.
   */
  GBuffer& deactivate();
};
}
#endif