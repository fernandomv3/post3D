#ifndef RENDERER_H
#define RENDERER_H

#include "post3D/Framebuffer.h"
#include "post3D/ShadowMapFramebuffer.h"
#include "post3D/GBuffer.h"
#include "post3D/Scene.h"
#include "post3D/Mesh.h"
#include "post3D/Mat4.h"
#include <GL/glew.h>
#include <memory>
#include <unordered_map>

using namespace scene;
using namespace math3D;
using namespace object3D;
using namespace std;

namespace render{

struct attribute{
	int buffer;
	int location;
	std::string type;
	void* data;
};
typedef struct attribute Attribute;

struct gltexture{
	int texture;
	int sampler;
};
typedef struct gltexture GLTexture;


class Renderer{
private:
	int vao;
	unique_ptr<Framebuffer> fb;
	unique_ptr<ShadowMapFramebuffer> shadowMap;
	unique_ptr<GBuffer> gBuffer;
	std::unordered_map<std::string,std::unordered_map<std::string,Attribute>> buffers;
	std::unordered_map<std::string,GLTexture> textures;
	bool renderShadows;
	bool softShadows;
	bool deferred;
	Renderer& initTexture(Texture& texture);
	Renderer& activateGBuffer();
	Renderer& deactivateGBuffer();
	Renderer& activateFramebuffer();
	Renderer& deactivateFramebuffer();
	Renderer& activateShadowFramebuffer();
	Renderer& deactivateShadowFramebuffer();
	vector<unique_ptr<float[]>> calculateDirectionalLights(Scene& scene);
	vector<unique_ptr<float[]>> calculatePointLights(Scene& scene);
	Renderer& calculateAmbientLights(Scene& scene);
	vector<unique_ptr<float[]>> calculateGlobalMatrices(Scene& scene);
	Renderer& setMaterialUniforms(Uniforms& uniforms, Material& material);
	Mat4 shadowPassRender(Scene& scene);
	Renderer& geometryPassRender(Scene& scene);
	Renderer& lightPassRender(Scene& scene);
	Renderer& initializeGeometryBuffers(Geometry& geom);
	Renderer& setUpVertexAttributes(Geometry& geom, GLProgram& prog, bool shadowPass = false);
	Renderer& renderForward(Scene& scene);
	Renderer& renderDeferred(Scene& scene);
	Renderer& createVAO();
	Renderer& drawGeometry(Geometry& geom);
	Renderer& setUpObjectUniforms(Uniforms& uniforms,Mesh& mesh);
	Renderer& setUpPointLightsUniforms(Uniforms& uniforms,Scene& scene, vector<unique_ptr<float[]>> & pLightData);
	Renderer& setUpDirectionalLightsUniforms(Uniforms& uniforms,Scene& scene, vector<unique_ptr<float[]>> & dLightData);
	Renderer& setUpAmbientLightUniforms(Uniforms& uniforms,Scene& scene, Light& ambLight);
	Renderer& setUpGlobalUniforms(Uniforms& uniforms,Scene& scene,vector<unique_ptr<float[]>> &matrices,Mat4& lightWorldMatrix);
public:
	Renderer();
	Renderer(const Renderer& renderer)=delete;
	Renderer(Renderer&& renderer)=default;
	Renderer& operator=(const Renderer& renderer)=delete;
	Renderer& operator=(Renderer&& renderer)=default;
	~Renderer();
	Renderer& render(Scene& scene);

	bool getRenderShadows()const;
	bool getSoftShadows()const;
	bool getRenderDeferred()const;
	Renderer& setRenderShadows(bool renderShadows);
	Renderer& setSoftShadows(bool softShadows);
	Renderer& setRenderDeferred(bool renderDeferred);
};

int makeBuffer(GLenum target, void* data, int size, GLenum usage = GL_STATIC_DRAW);

}
#endif