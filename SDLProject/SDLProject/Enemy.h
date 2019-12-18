#pragma once

#include "gameObject.h"

class Enemy : public gameObject
{
public:
	Enemy(const Point2D &location, SDL_Texture *textureAssetP, bool isStaticP, SDL_Rect *clipP);
	~Enemy();

	void render(SDL_Renderer *myRenderer, SDL_Rect camera);

	void updateTextureClip();

	int frameCounter;

};

