#include "post3D/Scene.h"
#include "post3D/Mat4.h"
#include <algorithm>

namespace scene{

Scene::Scene(){
  this->camera = std::shared_ptr<Camera>(new Camera());
  std::shared_ptr<Mat4> matrix = std::shared_ptr<Mat4>(new Mat4(Mat4::perspectiveMatrix(30.0f, 1280.0f/720.0f, 0.1f, 100.0f)));
  this->camera->setProjectionMatrix(matrix);
  //position camera (this should not go here)
  this->camera->getPosition()->setZ(12.0f);
  this->ambientLight = std::shared_ptr<Light>(new Light());
  this->ambientLight->getColor()->setRGBA(0.01f,0.01f,0.01f);
  this->usesShadows = false;
  this->PCFShadows = true;
  this->maxLightIntensity = 0.0f;

}

Scene::Scene(const Scene& scene){
  this->camera = scene.camera;
  this->ambientLight = scene.ambientLight;
  this->objects = scene.objects;
  this->addedObjects = scene.addedObjects;
  this->removedObjects = scene.removedObjects;
  this->directionalLights = scene.directionalLights;
  this->pointLights = scene.pointLights;
  this->maxLightIntensity = scene.maxLightIntensity;
  this->usesShadows = scene.usesShadows;
  this->PCFShadows = scene.PCFShadows;
}

Scene::Scene(Scene&& scene){
  this->camera = std::move(scene.camera);
  this->ambientLight = std::move(scene.ambientLight);
  this->objects = std::move(scene.objects);
  this->addedObjects = std::move(scene.addedObjects);
  this->removedObjects = std::move(scene.removedObjects);
  this->directionalLights = std::move(scene.directionalLights);
  this->pointLights = std::move(scene.pointLights);
  this->maxLightIntensity = scene.maxLightIntensity;
  this->usesShadows = scene.usesShadows;
  this->PCFShadows = scene.PCFShadows;
}

Scene& Scene::operator=(const Scene& scene){
  this->camera = scene.camera;
  this->ambientLight = scene.ambientLight;
  this->objects = scene.objects;
  this->addedObjects = scene.addedObjects;
  this->removedObjects = scene.removedObjects;
  this->directionalLights = scene.directionalLights;
  this->pointLights = scene.pointLights;
  this->maxLightIntensity = scene.maxLightIntensity;
  this->usesShadows = scene.usesShadows;
  this->PCFShadows = scene.PCFShadows;
  return *this;
}

Scene& Scene::operator=(Scene&& scene){
  this->camera = std::move(scene.camera);
  this->ambientLight = std::move(scene.ambientLight);
  this->objects = std::move(scene.objects);
  this->addedObjects = std::move(scene.addedObjects);
  this->removedObjects = std::move(scene.removedObjects);
  this->directionalLights = std::move(scene.directionalLights);
  this->pointLights = std::move(scene.pointLights);
  this->maxLightIntensity = scene.maxLightIntensity;
  this->usesShadows = scene.usesShadows;
  this->PCFShadows = scene.PCFShadows;
  return *this;
}

const vector<std::shared_ptr<Object3D>>& Scene::getObjects()const{
  return this->objects;
}

Scene& Scene::addObject(std::shared_ptr<Object3D> object){
  if(find(this->objects.begin(),this->objects.end(), object) == this->objects.end()){
    this->objects.push_back(object);
  }
  return *this;
}

Scene& Scene::removeObject(std::shared_ptr<Object3D> object){
  auto it = find(this->objects.begin(),this->objects.end(), object);
  if(it != this->objects.end()){
    this->objects.erase(it);
  }
  return *this;
}

std::shared_ptr<Light> Scene::getAmbientLight()const{
  return this->ambientLight;
}

Scene& Scene::setAmbientLight(std::shared_ptr<Light> ambientLight){
  this->ambientLight = ambientLight;
  return *this;
}

std::shared_ptr<Camera> Scene::getCamera()const{
  return this->camera;
}

Scene& Scene::setCamera(std::shared_ptr<Camera> camera){
  this->camera = camera;
  return *this;
}


const vector<std::shared_ptr<DirectionalLight>>& Scene::getDirectionalLights()const{
  return this->directionalLights;
}

const vector<std::shared_ptr<PointLight>>& Scene::getPointLights()const{
  return this->pointLights;
}

Scene& Scene::addPointLight(std::shared_ptr<PointLight> pointLight){
  this->pointLights.push_back(pointLight);
  return *this;
}

Scene& Scene::addDirectionalLight(std::shared_ptr<DirectionalLight> directionalLight){
  this->directionalLights.push_back(directionalLight);
  return *this;
}

float Scene::getMaxLightIntensity()const{
  return this->maxLightIntensity;
}

bool Scene::getUsesShadows()const{
  return this->usesShadows;
}

bool Scene::getPCFShadows()const{
  return this->PCFShadows;
}

Scene& Scene::setMaxLightIntensity(float maxLightIntensity){
  this->maxLightIntensity = maxLightIntensity;
  return *this;
}

Scene& Scene::setUsesShadows(bool usesShadows){
  this->usesShadows = usesShadows;
  return *this;
}

Scene& Scene::setPCFShadows(bool PCFShadows){
  this->PCFShadows = PCFShadows;
  return *this;
}

}