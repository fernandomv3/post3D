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

using namespace scene;
using namespace math3D;
using namespace object3D;
using namespace std;

namespace render{
	class Renderer{
	private:
		int vao;
		unique_ptr<Framebuffer> fb;
		unique_ptr<ShadowMapFramebuffer> shadowMap;
		unique_ptr<GBuffer> gBuffer;
		bool renderShadows;
		bool softShadows;
		bool renderDeferred;
		Renderer& activateFramebuffer();
		Renderer& deactivateFramebuffer();
		Renderer& activateShadowFramebuffer();
		Renderer& deactivateShadowFramebuffer();
		vector<unique_ptr<float[]>> calculateDirectionalLights(Scene& scene, shared_ptr<Mesh> mesh = nullptr);
		vector<unique_ptr<float[]>> calculatePointLights(Scene& scene, shared_ptr<Mesh> mesh = nullptr);
		Renderer& calculateAmbientLights(Scene& scene);
		vector<unique_ptr<float[]>> calculateGlobalMatrices(Scene& scene);
		Renderer& setMaterialUniforms(shared_ptr<Uniforms> uniforms, shared_ptr<Material> material);
		Mat4 shadowPassRender(Scene& scene);
		Renderer& geometryPassRender(Scene& scene);
		Renderer& initializeGeometryBuffers(shared_ptr<Geometry> geom);
		Renderer& setUpVertexAttributes(shared_ptr<Geometry> geom, shared_ptr<GLProgram> prog, bool shadowPass = false);
		Renderer& renderForward(Scene& scene);
		Renderer& createVAO();
		Renderer& drawGeometry(shared_ptr<Geometry> geom);
		Renderer& setUpObjectUniforms(shared_ptr<Uniforms>,shared_ptr<Mesh> mesh);
		Renderer& setUpPointLightsUniforms(shared_ptr<Uniforms> uniforms,Scene& scene, vector<unique_ptr<float[]>> & pLightData);
		Renderer& setUpDirectionalLightsUniforms(shared_ptr<Uniforms> uniforms,Scene& scene, vector<unique_ptr<float[]>> & dLightData);
		Renderer& setUpAmbientLightUniforms(shared_ptr<Uniforms> uniforms,Scene& scene, shared_ptr<Light> pLightData);
		Renderer& setUpGlobalUniforms(shared_ptr<Uniforms> uniforms,Scene& scene,vector<unique_ptr<float[]>> &matrices,shared_ptr<Mat4> lightWorldMatrix);
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