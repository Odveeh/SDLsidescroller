#pragma once
#include "gameObject.h"


class floatingText : public gameObject
{
public:
	floatingText(const Point2D &location, SDL_Texture *textureAssetP, bool isStaticP, SDL_Rect *clipP);
	~floatingText();

	int initTime;
	int lifeTime;
	int timeDifference;

	virtual void move(std::vector<gameObject*> gameObjectArray);
	

};

