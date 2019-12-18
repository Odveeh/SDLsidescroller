#pragma once
#include "gameObject.h"

class pickUpItem : public gameObject
{
public:
	pickUpItem(const Point2D &location, SDL_Texture *textureAssetP, bool isStaticP, SDL_Rect *clipP);
	~pickUpItem();




};

