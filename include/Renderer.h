#ifndef RENDERER_H
#define RENDERER_H

#include <GL/glew.h>
#include "Framebuffer.h"
#include "ShadowMapFramebuffer.h"
#include "Scene.h"
#include "Mesh.h"
#include "Mat4.h"
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
		bool renderShadows;
		bool softShadows;
		Renderer& activateFramebuffer();
		Renderer& deactivateFramebuffer();
		Renderer& activateShadowFramebuffer();
		Renderer& deactivateShadowFramebuffer();
		vector<unique_ptr<float[]>> calculateDirectionalLights(Scene& scene, shared_ptr<Mesh> mesh = nullptr);
		vector<unique_ptr<float[]>> calculatePointLights(Scene& scene, shared_ptr<Mesh> mesh = nullptr);
		Renderer& calculateAmbientLights(Scene& scene);
		vector<unique_ptr<float[]>> calculateGlobalMatrices(Scene& scene);
		Renderer& setMaterialUniforms(shared_ptr<Material> material);
		Mat4 shadowPassRender(Scene& scene);
		Renderer& initializeGeometryBuffers(shared_ptr<Geometry> geom,bool shadowPass = false);
		Renderer& setUpVertexAttributes(shared_ptr<Geometry> geom, shared_ptr<GLProgram> prog, bool shadowPass = false);
	public:
		Renderer();
		Renderer(const Renderer& renderer)=default;
		Renderer(Renderer&& renderer)=default;
		Renderer& operator=(const Renderer& renderer)=default;
		Renderer& operator=(Renderer&& renderer)=default;
		~Renderer()=default;
		Renderer& render(Scene& scene);

		bool getRenderShadows()const;
		bool getSoftShadows()const;
		Renderer& setRenderShadows(bool renderShadows);
		Renderer& setSoftShadows(bool softShadows);
	};

	int makeBuffer(GLenum target, void* data, int size, GLenum usage = GL_STATIC_DRAW);

}
#endif