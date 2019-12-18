#pragma once
#include "gameObject.h"
#include <vector>
#include "ParticleSystem.h"


class Projectile : public gameObject
{
public:
	Projectile(const Point2D &location, SDL_Texture *textureAssetP, bool isStaticP, SDL_Rect *clipP, ParticleSystem2 *particleSystemPointerP);
	~Projectile();

	ParticleSystem2 *particleSystemPointer;

	virtual void move(std::vector<gameObject*> gameObjectArray);
	virtual void destroy();

	virtual void render(SDL_Renderer *myRenderer, SDL_Rect camera);
	void updateTextureClip();
	int frameCounter;

	int killTime;
	int maxDuration;

	

};

