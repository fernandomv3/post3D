#ifndef SCENE_H
#define SCENE_H

#include "post3D/Camera.h"
#include "post3D/PointLight.h"
#include "post3D/Light.h"
#include "post3D/DirectionalLight.h"
#include "post3D/Object3D.h"
#include <memory>
#include <vector>
#include <string>

using namespace light;
using namespace object3D;

namespace scene{
class Scene{
private:
  std::string uuid;
  std::shared_ptr<Camera> camera;
  std::shared_ptr<Light> ambientLight;
  std::vector<std::shared_ptr<Object3D>> objects;
  std::vector<std::shared_ptr<Object3D>> addedObjects;
  std::vector<std::shared_ptr<Object3D>> removedObjects;
  std::vector<std::shared_ptr<DirectionalLight>> directionalLights;
  std::vector<std::shared_ptr<PointLight>> pointLights;
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
  std::string getUUID()const;
  const std::vector<std::shared_ptr<Object3D>>& getObjects()const;
  Scene& addObject(std::shared_ptr<Object3D> object);
  Scene& removeObject(std::shared_ptr<Object3D> object);
  std::shared_ptr<Light> getAmbientLight()const;
  Scene& setAmbientLight(std::shared_ptr<Light> ambientLight);
  std::shared_ptr<Camera> getCamera()const;
  Scene& setCamera(std::shared_ptr<Camera> camera);
  const std::vector<std::shared_ptr<DirectionalLight>>& getDirectionalLights()const;
  const std::vector<std::shared_ptr<PointLight>>& getPointLights()const;
  Scene& addPointLight(std::shared_ptr<PointLight> pointLight);
  Scene& addDirectionalLight(std::shared_ptr<DirectionalLight> directionalLight);
  float getMaxLightIntensity()const;
  bool getUsesShadows()const;
  bool getPCFShadows()const;
  Scene& setMaxLightIntensity(float maxLightIntensity);
  Scene& setUsesShadows(bool usesShadows);
  Scene& setPCFShadows(bool PCFShadows);
};
}

#endif