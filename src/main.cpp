#include "appinfo.h"
#include <cstdlib>
#include <iostream>
#include <sstream>
#define NO_SDL_GLEXT
#include <SDL2/SDL.h>
#include <GL/glew.h>
#include <SDL2/SDL_opengl.h>
#include "Vec3.h"
#include "SphericalCoord.h"
#include "Scene.h"
#include "Renderer.h"
#include "Geometry.h"
#include "Mesh.h"
#include "PhongMaterial.h"
#include "DirectionalLight.h"
#include "TextureMaterial.h"
#include "Texture.h"

using namespace math3D;
using namespace render;
using namespace scene;
using namespace object3D;
using namespace material;
using namespace light;
using namespace texture;
using namespace std;

const int SCREEN_WIDTH = 1280;
const int SCREEN_HEIGHT = 720;

SDL_Window* window = NULL;
SDL_GLContext context = NULL;

int oldTime = 0;
int nFrames = 0;

Renderer renderer;
Scene scene1;
shared_ptr<Mesh> robot;
shared_ptr<Light> light1;

void initializeContext(){
	if(SDL_Init(SDL_INIT_VIDEO) < 0){
		cerr << "SDL could not initialize! SDL_Error: " << SDL_GetError() << endl;
	}
	else{
		SDL_GL_SetAttribute( SDL_GL_CONTEXT_MAJOR_VERSION, 3 );
		SDL_GL_SetAttribute( SDL_GL_CONTEXT_MINOR_VERSION, 3 );
		SDL_GL_SetAttribute( SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE );
		SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, 1);
		SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, 4);
		SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
		window = SDL_CreateWindow(
			"Engine",
			SDL_WINDOWPOS_UNDEFINED,
			SDL_WINDOWPOS_UNDEFINED,
			SCREEN_WIDTH,
			SCREEN_HEIGHT,
			SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL 
		);
		if(window == NULL){
			cerr << "Window could not be created! SDL_Error: " << SDL_GetError() << endl;
		}else{
            context = SDL_GL_CreateContext( window );
			if( context == NULL ){
				printf( "OpenGL context could not be created! SDL Error: %s\n", SDL_GetError() );
			}
			else{
				glewExperimental = GL_TRUE;
				GLenum err = glewInit();
				if (GLEW_OK != err)
				{
					fprintf(stderr, "Error: %s\n", glewGetErrorString(err));
				}

				if(!GLEW_VERSION_3_3){
					fprintf(stderr,"OpenGL 4.5 not available\n");
					exit(-1);
				}
				const unsigned char* version = glGetString(GL_VERSION);
				if (version == NULL) {
					exit(-1);
				}
				SDL_GL_SetSwapInterval(0);
				printf("%s\n", version);
				glEnable(GL_DEPTH_TEST);
				glDepthFunc(GL_LEQUAL);
				glEnable(GL_CULL_FACE);
				glCullFace(GL_BACK);
				glFrontFace(GL_CCW);
				glEnable(GL_MULTISAMPLE);
				glHint(GL_LINE_SMOOTH_HINT, GL_NICEST );
				glHint(GL_POLYGON_SMOOTH_HINT, GL_NICEST );
				glEnable(GL_LINE_SMOOTH);
				glEnable(GL_POLYGON_SMOOTH);
				glEnable( GL_PROGRAM_POINT_SIZE);
			}
        }
	}
}

bool handleEvents(){
	SDL_Event event;
	while(SDL_PollEvent(&event)){
		Vec3 offset = Vec3();
		Vec3 offset2 = Vec3();
		switch(event.type){
			case SDL_QUIT:
				return true;
			case SDL_KEYDOWN:
				switch(event.key.keysym.sym){
					case SDLK_w:
						offset.setY(0.2f);
						break;
					case SDLK_a:
						offset.setX(-0.2f);
						break;
					case SDLK_s:
						offset.setY(-0.2f);
						break;
					case SDLK_d:
						offset.setX(0.2);
						break;
					case SDLK_q:
						offset.setZ(0.2f);
						break;
					case SDLK_e:
						offset.setZ(-0.2f);
						break;
					case SDLK_i:
						offset2.setX(-0.2f);
						break;
					case SDLK_k:
						offset2.setX(0.2f);
						break;
					case SDLK_j:
						offset2.setY(-0.2f);
						break;
					case SDLK_l:
						offset2.setY(0.2f);
						break;
				}
				{
				auto light1 = scene1.getDirectionalLights()[0];
				Vec3 newPos = orbit(*(light1->getTarget()->getPosition()),*(light1->getPosition()), offset2.getX(),offset2.getY(),1.0);
				light1->setPosition(shared_ptr<Vec3>(new Vec3(newPos)));
				*(robot->getPosition()) += offset;
				}
				break;
			case SDL_MOUSEMOTION:
				if(event.motion.state && SDL_BUTTON(1)){
					auto dTheta = -2.0f * 3.1415f * event.motion.xrel / SCREEN_WIDTH;
					auto dPhi = -2.0f * 3.1415f * event.motion.yrel / SCREEN_HEIGHT;
					Vec3 newPos = orbit(*(scene1.getCamera()->getTarget()),*(scene1.getCamera()->getPosition()),dPhi,dTheta,1.0);					
					scene1.getCamera()->setPosition(shared_ptr<Vec3>(new Vec3(newPos)));
				}
				break;
			case SDL_MOUSEWHEEL:
				auto factor = event.wheel.y * 0.05f;
				auto newPos = orbit(*(scene1.getCamera()->getTarget()),*(scene1.getCamera()->getPosition()),0,0,1-factor);
				scene1.getCamera()->setPosition(shared_ptr<Vec3>(new Vec3(newPos)));
				break;
		}
	}
	return false;
}

void renderLoop(){
	int currentTime = SDL_GetTicks();
	nFrames++;
	if(currentTime - oldTime >= 1000){
		ostringstream title;
		title << "FPS: " << nFrames << " | ms/frame: " << 1000/nFrames;
		SDL_SetWindowTitle(window,title.str().c_str());
		nFrames= 0;
		oldTime = currentTime;
	}
	glClearColor(1.0f,1.0f,1.0f,1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	renderer.render(scene1);
	SDL_GL_SwapWindow(window);
}

void cleanUp(){
	SDL_GL_DeleteContext(context);
	SDL_DestroyWindow(window);
	SDL_Quit();
}

void mainLoop(){
	bool quit = false;
	while(!quit){
		quit = handleEvents();
		renderLoop();
	}
}

int main(int argc, char** argv){
	initializeContext();
	scene1 = Scene();
	renderer = Renderer();
	//renderer.setRenderShadows(false);
	//renderer.setSoftShadows(false);

	auto geometry = shared_ptr<Geometry>(new Geometry(loadDataFromFile("share/post3D/robot-low-normal.dae")));
	auto phongMat = shared_ptr<PhongMaterial>(new PhongMaterial());
	phongMat->setShininess(100);
	phongMat->getDiffuseColor()->setRGBA(0.0f,0.0f,1.0f,0.0f);
	phongMat->getSpecularColor()->setRGBA(1.0f,1.0f,1.0f,1.0f);

	auto tex = shared_ptr<Texture>(new Texture());
	tex->setSourceFile("share/post3D/normalMap.jpg");
	phongMat->setNormalMap(tex);
	
	robot = shared_ptr<Mesh>(new Mesh(geometry,phongMat));
	robot->getRotation()->setX(3.141592f * -90 /180.0f);
	scene1.addObject(robot);

	auto squareGeom = shared_ptr<Geometry>(new Geometry(generateQuadGeometry()));
	auto squareMaterial = shared_ptr<PhongMaterial>(new PhongMaterial());
	squareMaterial->getDiffuseColor()->setRGBA(0.8f,0.8f,0.8f,0.0f);
	auto square = shared_ptr<Mesh>(new Mesh(squareGeom,squareMaterial));
	square->getRotation()->setX(3.141592f * -90 /180.0f);
	square->getPosition()->setY(-2.3f);
	square->setScale(shared_ptr<Vec3>(new Vec3(100.0f,100.0f,100.0f)));
	square->setCastShadows(false);
	scene1.addObject(square);

	scene1.getCamera()->setTarget(robot->getPosition());
	scene1.getCamera()->getPosition()->setX(2.0f);
	auto light1 = shared_ptr<DirectionalLight>(new DirectionalLight());
	light1->setPosition(shared_ptr<Vec3>(new Vec3(12.0f,14.0f,15.0f)));
	light1->getColor()->setRGBA(1.0f,1.0f,1.0f,1.0f);
	scene1.addDirectionalLight(light1);
	mainLoop();
	cleanUp();
	return EXIT_SUCCESS;
}
