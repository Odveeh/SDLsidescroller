#include "pickUpItem.h"
#include <iostream>


pickUpItem::pickUpItem(const Point2D &location, SDL_Texture *textureAssetP, bool isStaticP, SDL_Rect *clipP) : gameObject(location, textureAssetP, isStaticP, clipP)
{
	objectType = 3;
}


pickUpItem::~pickUpItem()
{ 
	
}
