#include "ParticleSystem.h"
#include <stdlib.h>  
#include <iostream>
#include "Projectile.h"


ParticleSystem::ParticleSystem(int particleAmountP, Point2D points)
{
	srand(SDL_GetTicks());
	pSystemPos = points;
	particleAmount = particleAmountP;
	//Particle *particleArray = new Particle[particleAmount];

	for (int i = 0; i < particleAmount; i++) {

		//particleArrayPointer[i] = new Particle(pSystemPos.getX(), pSystemPos.getY());
		particleVector.push_back(new Particle(pSystemPos.getX() + rand() % 4 - 2, pSystemPos.getY() + rand() % 4 - 2));
		//std::cout << "vectorzdsize: " << particleVector.size() << std::endl;
	}
	
}

ParticleSystem::ParticleSystem() {


}


ParticleSystem::~ParticleSystem()
{

	//std::cout << "calling BASEticledesturcotr " << std::endl;
	for (int i = 0; i < particleVector.size(); i++) {

		delete particleVector[i];

	}

	particleVector.clear();
}


void ParticleSystem::tick(SDL_Renderer *myRenderer, SDL_Rect camera) {

	

	for (int i = 0; i < particleVector.size(); i++) {

		if (particleVector[i]->isDead) {
		

			Particle *ppointer = particleVector[i];
			
			particleVector.erase(particleVector.begin() + i);

			delete ppointer;


		}
		else {
			
			particleVector[i]->tick();
			SDL_SetRenderDrawColor(myRenderer, 255, 255, 255, 255);
		
			SDL_RenderDrawPoint(myRenderer, particleVector[i]->particlePos.x - camera.x, particleVector[i]->particlePos.y - camera.y);

		}

		
	}
	
}




Particle::Particle() {


}



Particle::Particle(int xParam, int yParam) {

	isDead = false;
	particleLifeTime = 1500;
	particleKillTime = SDL_GetTicks() + particleLifeTime;
	particlePos = { xParam, yParam };

	
	xVel = rand() % 150 + (float)rand() / (float)(RAND_MAX) - 75;
	yVel = rand() % 150 + (float)rand() / (float)(RAND_MAX) - 50;

	xVel /= 3;
	yVel /= 3;
	
	

	//std::cout << "xvel: " << xVel << "yvel: " << yVel << std::endl;
	//xVel = 0.0f;
	//yVel = 0.0f;
}



void Particle::tick() {

	if (SDL_GetTicks() > particleKillTime) {
		isDead = true;
	}

	move();
	

}


void Particle::move() {

	
	//gravity
	if (yVel < 0) {
		yVel *= -1.03f;
	} else{
		yVel *= 1.03f;
	}
	

	//drag
	yVel *= 0.97f;
	xVel *= 0.999f;


	particlePos = { particlePos.x + (int)xVel, particlePos.y + (int)yVel };



}


void Particle::render() {



}


Particle::~Particle() {
	
}






//
//
//
//
//
ParticleSystem2::ParticleSystem2() {


}


ParticleSystem2::ParticleSystem2(int particleAmountP, Point2D points) {
	
	pSystemPos = points;
	particleAmount = particleAmountP;


	for (int i = 0; i < particleAmount; i++) {
		particleVector.push_back(new Particle2(pSystemPos.getX() + rand() % 50 - 25, pSystemPos.getY() + rand() % 50 - 25));
	}

}


ParticleSystem2::~ParticleSystem2() {

	//std::cout << "calling dparticledesturcotr " << std::endl;
	for (int i = 0; i < particleVector.size(); i++) {
	
		delete particleVector[i];
	
	}

	particleVector.clear();
}


void ParticleSystem2::tick(SDL_Renderer *myRenderer, SDL_Rect camera) {
	
	for (int i = 0; i < particleVector.size(); i++) {

		if (particleVector[i]->isDead) {

			Particle *ppointer = particleVector[i];
			particleVector.erase(particleVector.begin() + i);
			particleVector.push_back(new Particle2(pSystemPos.getX() + rand() % 50 - 25, pSystemPos.getY() + rand() % 50 - 25));
			delete ppointer;


		}
		else {
			srand(SDL_GetTicks());
			particleVector[i]->tick();
			SDL_SetRenderDrawColor(myRenderer, 255, 127, 80, 255);
			SDL_RenderDrawPoint(myRenderer, particleVector[i]->particlePos.x - camera.x, particleVector[i]->particlePos.y - camera.y);

		}


	}

}








Particle2::Particle2(int xParam, int yParam) {

	isDead = false;
	particleLifeTime =  0 + rand() % 150;
	particleKillTime = SDL_GetTicks() + particleLifeTime;
	particlePos = { xParam, yParam };


	//xVel = rand() % 10 + (float)rand() / (float)(RAND_MAX)-5;
	//yVel = rand() % 10 + (float)rand() / (float)(RAND_MAX)-5;
	xVel = 1;
	yVel = -5;

	xVel /= 3;
	yVel /= 3;



	//std::cout << "xvel: " << xVel << "yvel: " << yVel << std::endl;
	//xVel = 0.0f;
	//yVel = 0.0f;
}
