#pragma once

#include <SDL.h>
#include "Point2D.h"
#include <vector>


class gameObject
{
public:

	gameObject();
	gameObject(const Point2D &location, SDL_Texture *textureAssetP, bool isStaticP, SDL_Rect *clipP);
	~gameObject();

	virtual void destroy();

	virtual void move(std::vector<gameObject*> gameObjectArray);
	virtual void render(SDL_Renderer *myRenderer, SDL_Rect camera);

	bool visibility;
	bool isStatic;

	SDL_Texture *textureAsset;
	int textureWidth, textureHeight;

	//same size as textureAsset, used for collision and rendering
	SDL_Rect rect;
	int textureColumnCount;
	int textureRowCount;

	//used for spritesheets
	SDL_Rect *clip;
	virtual void updateTextureClip();

	Point2D position;

	//character's velocity, affects movement (duh)
	float velocityX;
	float velocityY;

	float maxVelocityX;
	float maxVelocityY;

	bool isInAir;
	float gravityY = 0.2f;

	//status = 0 = toBeDestoyed, 1 = activeProjectile, ...
	int status;
	
	int objectType;


};

