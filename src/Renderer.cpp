#include "post3D/Renderer.h"
#include "post3D/Vec3.h"
#include <iostream>
////new
using namespace std;

namespace render{

Renderer::Renderer(){
  this->vao = 0;
  this->fb = unique_ptr<Framebuffer>();
  this->gBuffer = unique_ptr<GBuffer>();
  this->shadowMap = unique_ptr<ShadowMapFramebuffer>(new ShadowMapFramebuffer(1280,720));
  this->renderShadows = true;
  this->softShadows = true;
  this->deferred =  false;
}

Renderer::~Renderer(){
  glDeleteVertexArrays(1,(GLuint*)&this->vao);
}

bool Renderer::getRenderShadows()const{
  return this->renderShadows;
}

bool Renderer::getSoftShadows()const{
  return this->softShadows;
}

bool Renderer::getRenderDeferred()const{
  return this->deferred;
}

Renderer& Renderer::setRenderShadows(bool renderShadows){
  this->renderShadows = renderShadows;
  return *this;
}

Renderer& Renderer::setSoftShadows(bool softShadows){
  this->softShadows = softShadows;
  return *this;
}

Renderer& Renderer::setRenderDeferred(bool renderDeferred){
  this->deferred = renderDeferred;
  return *this;
}

Renderer& Renderer::activateFramebuffer(){
  if(this->fb){
    if(!this->fb->getFbo()){
      this->initTexture(*this->fb->getTexture());
      this->fb->create();
      GLTexture tex = this->textures[this->fb->getTexture()->getUUID()];
      this->fb->configure(tex.texture);
    }
    this->fb->bindForWriting();
  }
  return *this;
}

Renderer& Renderer::activateGBuffer(){
  if(this->gBuffer){
    if(!this->gBuffer->getFbo()){
      this->gBuffer->create();
      this->gBuffer->configure();
    }
    this->gBuffer->bindForWriting();
  }
  return *this;
}

Renderer& Renderer::deactivateGBuffer(){
  glBindBuffer(GL_FRAMEBUFFER,0);
  return *this;
}

Renderer& Renderer::deactivateFramebuffer(){
  glBindBuffer(GL_FRAMEBUFFER,0);
  return *this;
}

Renderer& Renderer::activateShadowFramebuffer(){
  if(this->shadowMap){
    if(!this->shadowMap->getFbo()){
      this->initTexture(*this->shadowMap->getTexture());
      this->shadowMap->create();
      GLTexture tex = this->textures[this->shadowMap->getTexture()->getUUID()];
      this->shadowMap->configure(tex.texture);
    }
    this->shadowMap->bindForWriting();
  }
  glClear(GL_DEPTH_BUFFER_BIT);
  return *this;
}

Renderer& Renderer::deactivateShadowFramebuffer(){
  glBindFramebuffer(GL_FRAMEBUFFER,0);
  return *this;
}

Renderer& Renderer::createVAO(){
  if(!this->vao){
    GLuint vao;
    glGenVertexArrays(1,&vao);
    this->vao = vao;
    glBindVertexArray(this->vao); 
  }
  return *this;
}

vector<unique_ptr<float[]>> Renderer::calculateDirectionalLights(Scene& scene){
  auto result = vector<unique_ptr<float[]>>();
  int size = scene.getDirectionalLights().size();
  
  auto colors = unique_ptr<float[]>(new float[size*4]);
  auto vectors = unique_ptr<float[]>(new float[size*4]);
  auto intensities = unique_ptr<float[]>(new float[size]);

  for(int i = 0; i<size ; i++){
    colors[4*i]=scene.getDirectionalLights()[i]->getColor()->getComponent('r');
    colors[4*i +1]=scene.getDirectionalLights()[i]->getColor()->getComponent('g');
    colors[4*i +2]=scene.getDirectionalLights()[i]->getColor()->getComponent('b');
    colors[4*i +3]= 0.0f;
    auto vecToLight = scene.getDirectionalLights()[i]->getVectorToLightAsArray(*scene.getCamera());
    vectors[4*i]= vecToLight[0];
    vectors[4*i +1]= vecToLight[1];
    vectors[4*i +2]= vecToLight[2];
    vectors[4*i +3]= vecToLight[3];

    intensities[i]=scene.getDirectionalLights()[i]->getIntensity();
    if (intensities[i] >= scene.getMaxLightIntensity()){
      scene.setMaxLightIntensity(intensities[i]);
    }
  }
  result.push_back(move(vectors));
  result.push_back(move(colors));
  result.push_back(move(intensities));
  return result;
}

vector<unique_ptr<float[]>> Renderer::calculatePointLights(Scene& scene){
  auto result = vector<unique_ptr<float[]>>(4);
  int size = scene.getPointLights().size();
  
  auto colors = unique_ptr<float[]>(new float[size*4]);
  auto positions = unique_ptr<float[]>(new float[size*4]);
  auto intensities = unique_ptr<float[]>(new float[size]);
  auto attenuations = unique_ptr<float[]>(new float[size]);
  for(int i = 0; i<size ; i++){
    colors[4*i]=scene.getPointLights()[i]->getColor()->getComponent('r');
    colors[4*i +1]=scene.getPointLights()[i]->getColor()->getComponent('g');
    colors[4*i +2]=scene.getPointLights()[i]->getColor()->getComponent('b');
    colors[4*i +3]= 0.0f;

    positions[4*i]=scene.getPointLights()[i]->getPosition()->getX();
    positions[4*i +1]=scene.getPointLights()[i]->getPosition()->getY();
    positions[4*i +2]=scene.getPointLights()[i]->getPosition()->getZ();
    positions[4*i +3]= 1.0f;

    attenuations[i] = scene.getPointLights()[i]->getAttenuation();
    intensities[i]=scene.getPointLights()[i]->getIntensity();
    if (intensities[i] >= scene.getMaxLightIntensity()){
      scene.setMaxLightIntensity(intensities[i]);
    }
  }
  result.push_back(move(positions));
  result.push_back(move(colors));
  result.push_back(move(intensities));
  result.push_back(move(attenuations));
  return result;
}

Renderer& Renderer::calculateAmbientLights(Scene& scene){
  return *this;
}

vector<unique_ptr<float[]>> Renderer::calculateGlobalMatrices(Scene& scene){
  auto result = vector<unique_ptr<float[]>>(0);
  scene.getCamera()->updateWorldMatrix();
  auto wordMatrix = scene.getCamera()->getWorldMatrix()->getAsArray();
  auto projectionMatrix = scene.getCamera()->getProjectionMatrix()->getAsArray();
  result.push_back(move(wordMatrix));
  result.push_back(move(projectionMatrix));
  return result;
}

Renderer& Renderer::setUpGlobalUniforms(GLProgram& prog, Scene& scene, vector<unique_ptr<float[]>> &matrices, Mat4& lightWorldMatrix){
  auto uniforms = prog.getpUniforms();

  glUniformMatrix4fv(
    uniforms["worldMatrix"].location,
    1,
    GL_TRUE,
    matrices[0].get()
  );
  glUniformMatrix4fv(
    uniforms["projectionMatrix"].location,
    1,
    GL_TRUE,
    matrices[1].get()
  );
  glUniformMatrix4fv(
    uniforms["depthWorldMatrix"].location,
    1,
    GL_TRUE,
    lightWorldMatrix.getAsArray().get()
  );
  GLfloat invGamma = 1/scene.getCamera()->getGamma();
  glUniform1f(
    uniforms["invGamma"].location,
    invGamma
  );
  GLfloat maxLightIntensity = scene.getMaxLightIntensity();
  glUniform1f(
    uniforms["maxLightIntensity"].location,
    maxLightIntensity
  );

  return *this;
}

Renderer& Renderer::setUpDirectionalLightsUniforms(GLProgram& prog,Scene& scene,vector<unique_ptr<float[]>>& dlightData){
  auto uniforms = prog.getpUniforms();
  int numDirLights = scene.getDirectionalLights().size();
  if(numDirLights > 0){
    glUniform4fv(
      uniforms["dirLightColor"].location,
      numDirLights,
      dlightData[1].get()
    );
    glUniform4fv(
      uniforms["dirLightVectorToLight"].location,
      numDirLights,
      dlightData[0].get()
    );
    glUniform1fv(
      uniforms["dirLightIntensity"].location,
      numDirLights,
      dlightData[2].get()
    );

  }
  return *this;
}

Renderer& Renderer::setUpPointLightsUniforms(GLProgram& prog,Scene& scene,vector<unique_ptr<float[]>>& plightData){
  auto uniforms = prog.getpUniforms();
  int numPLights = scene.getPointLights().size();
  if(numPLights > 0){
    glUniform4fv(
      uniforms["pointLightColor"].location,
      numPLights,
      plightData[0].get()
    );
    glUniform4fv(
      uniforms["pointLightPosition"].location,
      numPLights,
      plightData[1].get()
    );
    glUniform1fv(
      uniforms["pointLightIntensity"].location,
      numPLights,
      plightData[2].get()
    );
    glUniform1fv(
      uniforms["pointLightAttenuation"].location,
      numPLights,
      plightData[3].get()
    );
  }
  return *this;
}

Renderer& Renderer::setUpAmbientLightUniforms(GLProgram& prog,Scene& scene,Light& ambLight){
  auto uniforms = prog.getpUniforms();
  glUniform4fv(
    uniforms["ambientLight"].location,
    1,
    scene.getAmbientLight()->getColor()->getAsArray().get()
  );
  return *this;
}

Renderer& Renderer::setUpObjectUniforms(GLProgram& prog, Mesh& mesh){
  auto uniforms = prog.getpUniforms();
  mesh.updateModelMatrix();
  glUniformMatrix4fv(
    uniforms["modelMatrix"].location,
    1,
    GL_TRUE,
    mesh.getModelMatrix()->getAsArray().get()
  );
  return *this;
}

Renderer& Renderer::initTexture(Texture& texture){
  int tex, sampler;
  if(texture.getSourceFile() != "") texture.loadFile();
  glGenTextures(1,(GLuint*)&tex);
  glBindTexture(texture.getTarget(),tex);
  glTexImage2D(
    texture.getTarget(), 0,           //target, level of detail
    texture.getInnerFormat(),                    //internal format
    texture.getWidth(), texture.getHeight(), 0,           //width, height, border
    texture.getFormat(), texture.getType(),   //external format, type 
    texture.getImage()                      //pixels
  );
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S,GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T,GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_BASE_LEVEL, 0);
  glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MAX_LEVEL, 0);
  if (texture.getInnerFormat() == GL_DEPTH_COMPONENT){
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_REF_TO_TEXTURE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_FUNC, GL_LEQUAL);
  }
  
  
  glGenSamplers(1,(GLuint*)&sampler);
  glSamplerParameteri(sampler, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glSamplerParameteri(sampler, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glSamplerParameteri(sampler, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glSamplerParameteri(sampler, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

  this->textures[texture.getUUID()].texture = tex;
  this->textures[texture.getUUID()].sampler = sampler;

  glBindTexture(texture.getTarget(),0);
  return *this;
}

Renderer& Renderer::setMaterialUniforms(GLProgram& prog, Material& material){
  auto uniforms = prog.getpUniforms();
  
  auto diffuseColor = material.getDiffuseColor()->getAsArray();
  glUniform4fv(
    uniforms["diffuseColor"].location,
    1,
    diffuseColor.get()
  );

  auto specularColor = material.getSpecularColor()->getAsArray();
  glUniform4fv(
    uniforms["specularColor"].location,
    1,
    specularColor.get()
  );

  float shininess = material.getShininess();
  glUniform1fv(
    uniforms["shininess"].location,
    1,
    &shininess
  );

  glUniform1i(uniforms["mapSampler"].location,MAP);

  glUniform1i(uniforms["normalMapSampler"].location,NORMALMAP);

  glUniform1i(uniforms["shadowMapSampler"].location,SHADOWMAP);

  glUniform1i(uniforms["sampleSize"].location,this->shadowMap->getSampleSize());

  glUniform2f(uniforms["shadowMapSize"].location,this->shadowMap->getWidth(),this->shadowMap->getHeight());
  
  if(material.getMap()){
    auto it = this->textures.find(material.getMap()->getUUID());
    if(it == this->textures.end()) initTexture(*material.getMap());
    GLTexture tex = this->textures[material.getMap()->getUUID()];
    glActiveTexture(GL_TEXTURE0 + MAP);
    glBindTexture(GL_TEXTURE_2D,tex.texture);
    glBindSampler(MAP,tex.sampler);
  }

  if(material.getNormalMap()){
    auto it = this->textures.find(material.getNormalMap()->getUUID());
    if(it == this->textures.end()) initTexture(*material.getNormalMap());
    GLTexture tex = this->textures[material.getNormalMap()->getUUID()];
    glActiveTexture(GL_TEXTURE0 + NORMALMAP);
    glBindTexture(GL_TEXTURE_2D,tex.texture);
    glBindSampler(NORMALMAP,tex.sampler);
  }
  
  if(this->shadowMap && this->renderShadows){
    auto it = this->textures.find(this->shadowMap->getTexture()->getUUID());
    if(it == this->textures.end()) initTexture(*this->shadowMap->getTexture());
    glActiveTexture(GL_TEXTURE0 + SHADOWMAP);
    GLTexture tex = this->textures[this->shadowMap->getTexture()->getUUID()];
    glBindTexture(GL_TEXTURE_2D,tex.texture);
    glBindSampler(SHADOWMAP,tex.sampler);
  }
  return *this;
}

Renderer& Renderer::initializeGeometryBuffers(Geometry& geom){
  auto &buffer = this->buffers[geom.getUUID()];

  if((buffer.find("vertex")== buffer.end()) && !geom.getVertices().empty()){
    int buf = makeBuffer(
      GL_ARRAY_BUFFER,
      (void*)&geom.getVertices().front(),
      geom.getVertices().size() * sizeof(GLfloat)
    );
    buffer["vertex"].buffer = buf;
  }

  if((buffer.find("index")== buffer.end()) && !geom.getElements().empty()){
    int buf = makeBuffer(
      GL_ELEMENT_ARRAY_BUFFER,
      (void*)&geom.getElements().front(),
      geom.getElements().size() * sizeof(GLushort)
    );
    buffer["index"].buffer = buf;
  }

  if((buffer.find("normal")== buffer.end()) && !geom.getNormals().empty()){
    int buf = makeBuffer(
      GL_ARRAY_BUFFER,
      (void*)&geom.getNormals().front(),
      geom.getNormals().size() * sizeof(GLfloat)
    );
    buffer["normal"].buffer = buf;
  }
  if((buffer.find("uv")== buffer.end()) && !geom.getTexCoords().empty()){
    int buf = makeBuffer(
      GL_ARRAY_BUFFER,
      (void*)&geom.getTexCoords().front(),
      geom.getTexCoords().size() * sizeof(GLfloat)
    );
    buffer["uv"].buffer = buf;
  }

  if((buffer.find("tangent")== buffer.end()) && !geom.getTangents().empty()){
    int buf = makeBuffer(
      GL_ARRAY_BUFFER,
      (void*)&geom.getTangents().front(),
      geom.getTangents().size() * sizeof(GLfloat)
    );
    buffer["tangent"].buffer = buf;
  }

  geom.setInitialized(true);

  return *this;
}

Renderer& Renderer::setUpVertexAttributes(Geometry& geom, GLProgram& prog, bool shadowPass){
  auto &buffer = this->buffers[geom.getUUID()];
  glBindBuffer(GL_ARRAY_BUFFER,buffer["vertex"].buffer);
  glVertexAttribPointer(
    prog.getAttrPosition(),
    3,
    GL_FLOAT,
    GL_FALSE,
    0,
    (void*)0
  );
  glEnableVertexAttribArray(prog.getAttrPosition());

  if(shadowPass) return *this;

  if((buffer.find("normal")!= buffer.end())){
    glBindBuffer(GL_ARRAY_BUFFER,buffer["normal"].buffer);
    glVertexAttribPointer(
      prog.getAttrNormal(),
      3,
      GL_FLOAT,
      GL_FALSE,
      0,
      (void*)0
    );
    glEnableVertexAttribArray(prog.getAttrNormal());
  }
  if((buffer.find("tangent")!= buffer.end())){
    glBindBuffer(GL_ARRAY_BUFFER,buffer["tangent"].buffer);
    glVertexAttribPointer(
      prog.getAttrTangent(),
      3,
      GL_FLOAT,
      GL_FALSE,
      0,
      (void*)0
    );
    glEnableVertexAttribArray(prog.getAttrTangent());
  }
  if((buffer.find("uv")!= buffer.end())){
    glBindBuffer(GL_ARRAY_BUFFER,buffer["uv"].buffer);
    glVertexAttribPointer(
      prog.getAttrUV(),
      2,
      GL_FLOAT,
      GL_FALSE,
      0,
      (void*)0
    );
    glEnableVertexAttribArray(prog.getAttrUV());
  }
  return *this;
}

Renderer& Renderer::drawGeometry(Geometry& geom){
  auto &buffer = this->buffers[geom.getUUID()];
  if (!geom.getElements().empty()){
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,buffer["index"].buffer);
    glDrawElements(
      GL_TRIANGLES,
      geom.getElements().size(),
      GL_UNSIGNED_SHORT,
      (void*)0
    );
  }else{
    glBindBuffer(GL_ARRAY_BUFFER,buffer["vertex"].buffer);
    int numVertices = geom.getVertices().size() / 3;
    glDrawArrays(
      GL_TRIANGLES,
      0,
      numVertices
    );
  }
  return *this;
}

Mat4 Renderer::shadowPassRender(Scene& scene){
  this->activateShadowFramebuffer();
  glCullFace(GL_FRONT);
  auto dirLight = scene.getDirectionalLights()[0];
  auto cameraPosition = scene.getCamera()->getPosition();
  auto cameraTarget = scene.getCamera()->getTarget();
  auto cameraPerspective = scene.getCamera()->getProjectionMatrix();
  scene.getCamera()->setProjectionMatrix(shared_ptr<Mat4>(new Mat4(Mat4::perspectiveMatrix(30.0f,1280.0f/720.0f,0.1f,100.0f))));
  scene.getCamera()->setPosition(dirLight->getPosition());
  scene.getCamera()->setTarget(dirLight->getTarget()->getPosition());
  auto matrices = this->calculateGlobalMatrices(scene);

  auto material = this->shadowMap->getMaterial();

  if(!material->getProgram()){
    material->makePrograms(scene);
  }
  glUseProgram(material->getProgram()->getProgram());

  this->setUpGlobalUniforms(*material->getProgram(),scene,matrices,*shared_ptr<Mat4>(new Mat4()));

  for(auto obj : scene.getObjects() ){
    auto mesh = static_pointer_cast<Mesh>(obj);
    if(!mesh->getVisible() || ! mesh->getCastShadows()){
      continue;
    }
    if(!mesh->getGeometry()->isInitialized())
      this->initializeGeometryBuffers(*mesh->getGeometry());

    this->setUpVertexAttributes(*mesh->getGeometry(),*material->getProgram(),true);

    this->setUpObjectUniforms(*material->getProgram(),*mesh);

    this->drawGeometry(*mesh->getGeometry());
    glDisableVertexAttribArray(material->getProgram()->getAttrPosition());
  }
  auto lightWorldMatrix = *scene.getCamera()->getProjectionMatrix() * *scene.getCamera()->getWorldMatrix().get();
  scene.getCamera()->setPosition(cameraPosition);
  scene.getCamera()->setTarget(cameraTarget);
  scene.getCamera()->setProjectionMatrix(cameraPerspective);
  glCullFace(GL_BACK);
  this->deactivateShadowFramebuffer();
  dirLight->setLightWorldMatrix(shared_ptr<Mat4>(new Mat4(lightWorldMatrix)));
  return lightWorldMatrix;
}

Renderer& Renderer::renderForward(Scene& scene){
  shared_ptr<Mat4> lightWorldMatrix;
  if(this->renderShadows){
    lightWorldMatrix = scene.getDirectionalLights()[0]->getLightWorldMatrix();
  }

  scene.setMaxLightIntensity(0);

  auto matrices = this->calculateGlobalMatrices(scene);

  vector<unique_ptr<float[]>> dlightData;
  if(scene.getDirectionalLights().size()){
    dlightData = this->calculateDirectionalLights(scene);
  }

  vector<unique_ptr<float[]>> plightData;
  if(scene.getPointLights().size()){
    plightData = this->calculatePointLights(scene);
  }

  for(auto obj : scene.getObjects()){
    auto mesh = static_pointer_cast<Mesh>(obj);
    if(!mesh->getVisible()) continue;
    if(!mesh->getGeometry()->isInitialized())
      this->initializeGeometryBuffers(*mesh->getGeometry());

    auto it = this->programs.find(mesh->getMaterial()->getUUID());
    if(it != this->programs.end()){
      
    }

    if(!mesh->getMaterial()->getProgram())
      mesh->getMaterial()->makePrograms(scene);
    this->setUpVertexAttributes(*mesh->getGeometry(),*mesh->getMaterial()->getProgram());

    glUseProgram(mesh->getMaterial()->getProgram()->getProgram());

    this->setUpGlobalUniforms(*mesh->getMaterial()->getProgram(),scene,matrices,*lightWorldMatrix);

    this->setUpDirectionalLightsUniforms(*mesh->getMaterial()->getProgram(),scene,dlightData);
    this->setUpPointLightsUniforms(*mesh->getMaterial()->getProgram(),scene,plightData);
    this->setUpAmbientLightUniforms(*mesh->getMaterial()->getProgram(),scene,*scene.getAmbientLight());

    this->setUpObjectUniforms(*mesh->getMaterial()->getProgram(),*mesh);
    this->setMaterialUniforms(*mesh->getMaterial()->getProgram(),*mesh->getMaterial());

    this->drawGeometry(*mesh->getGeometry());

    glDisableVertexAttribArray(mesh->getMaterial()->getProgram()->getAttrPosition());
  }

  return *this;
}

Renderer& Renderer::renderDeferred(Scene& scene){
  this->geometryPassRender(scene);
  this->lightPassRender(scene);
  return *this;
}

Renderer& Renderer::geometryPassRender(Scene& scene){
  if(!this->gBuffer){
    this->gBuffer = unique_ptr<GBuffer>(new GBuffer(1280,720));
  }

  this->activateGBuffer();
  auto matrices = this->calculateGlobalMatrices(scene);
  for(auto obj : scene.getObjects()){
    auto mesh = static_pointer_cast<Mesh>(obj);
    if(!mesh->getVisible()) continue;

    if(!mesh->getGeometry()->isInitialized())
      this->initializeGeometryBuffers(*mesh->getGeometry());

    if(!mesh->getMaterial()->getProgram())
      mesh->getMaterial()->makePrograms(scene,true);

    this->setUpVertexAttributes(*mesh->getGeometry(),*mesh->getMaterial()->getProgram());

    glUseProgram(mesh->getMaterial()->getProgram()->getProgram());

    this->setUpGlobalUniforms(*mesh->getMaterial()->getProgram(),scene,matrices,*shared_ptr<Mat4>(new Mat4()));

    this->setUpObjectUniforms(*mesh->getMaterial()->getProgram(),*mesh);

    this->setMaterialUniforms(*mesh->getMaterial()->getProgram(),*mesh->getMaterial());

    this->drawGeometry(*mesh->getGeometry());

    glDisableVertexAttribArray(mesh->getMaterial()->getProgram()->getAttrPosition());

  }
  return *this;
}

Renderer& Renderer::lightPassRender(Scene& scene){
  return *this;
}

Renderer& Renderer::render(Scene& scene){
  this->createVAO();

  if(this->renderShadows)
    scene.setUsesShadows(this->renderShadows);
    scene.setPCFShadows(this->softShadows);
    this->shadowPassRender(scene);

  if(this->deferred){
    this->renderDeferred(scene);
  }else{
    this->renderForward(scene);
  }

  return *this;
}

int makeBuffer(GLenum target, void* data, int size, GLenum usage){
  int buf;
  glGenBuffers(1,(GLuint*)&buf);
  glBindBuffer(target,buf);
  glBufferData(target,size,data,usage);
  return buf;
}

}