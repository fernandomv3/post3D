#ifndef SCENE_H
#define SCENE_H

#include <memory>
#include <vector>
#include "Camera.h"
#include "PointLight.h"
#include "Light.h"
#include "DirectionalLight.h"
#include "Object3D.h"

using namespace std;
using namespace light;
using namespace object3D;

namespace scene{
	class Scene{
	private:
		shared_ptr<Camera> camera;
		shared_ptr<Light> ambientLight;
		vector<shared_ptr<Object3D>> objects;
		vector<shared_ptr<Object3D>> addedObjects;
		vector<shared_ptr<Object3D>> removedObjects;
		vector<shared_ptr<DirectionalLight>> directionalLights;
		vector<shared_ptr<PointLight>> pointLights;
		float maxLightIntensity;
		bool usesShadows;
		bool PCFShadows;
	public:
		Scene();
		Scene(const Scene& scene);
		Scene(Scene&& scene);
		Scene& operator=(const Scene& scene);
		Scene& operator=(Scene&& scene);
		~Scene()=default;
		const vector<shared_ptr<Object3D>>& getObjects()const;
		Scene& addObject(shared_ptr<Object3D> object);
		Scene& removeObject(shared_ptr<Object3D> object);
		shared_ptr<Light> getAmbientLight()const;
		Scene& setAmbientLight(shared_ptr<Light> ambientLight);
		shared_ptr<Camera> getCamera()const;
		Scene& setCamera(shared_ptr<Camera> camera);
		const vector<shared_ptr<DirectionalLight>>& getDirectionalLights()const;
		const vector<shared_ptr<PointLight>>& getPointLights()const;
		Scene& addPointLight(shared_ptr<PointLight> pointLight);
		Scene& addDirectionalLight(shared_ptr<DirectionalLight> directionalLight);
		float getMaxLightIntensity()const;
		bool getUsesShadows()const;
		bool getPCFShadows()const;
		Scene& setMaxLightIntensity(float maxLightIntensity);
		Scene& setUsesShadows(bool usesShadows);
		Scene& setPCFShadows(bool PCFShadows);
	};
}

#endif