#pragma once
#include "Point2D.h"
#include "SDL.h"
#include <vector>

class Particle {

public:
	Particle();
	Particle(int xParam, int yParam);
	~Particle();

	SDL_Point particlePos;
	float xVel;
	float yVel;

	int particleKillTime;
	int particleLifeTime;

	bool isDead;

	SDL_Color particleColor;

	void tick();
	void move();
	void render();

};



class ParticleSystem
{
public:
	ParticleSystem();
	ParticleSystem(int particleAmountP, Point2D points);
	~ParticleSystem();

	std::vector <Particle*> particleVector;

	Point2D pSystemPos;
	int particleAmount;
	int particleSystemLifetime;

	virtual void tick(SDL_Renderer *myRenderer, SDL_Rect camera);

};



class Particle2 : public Particle {

public:

	Particle2(int xParam, int yParam);


};



class ParticleSystem2 : public ParticleSystem {

public:
	ParticleSystem2();
	ParticleSystem2(int particleAmountP, Point2D points);
	~ParticleSystem2();
	void tick(SDL_Renderer *myRenderer, SDL_Rect camera);






};
