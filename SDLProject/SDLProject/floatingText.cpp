#include "floatingText.h"
#include <iostream>


floatingText::floatingText(const Point2D &location, SDL_Texture *textureAssetP, bool isStaticP, SDL_Rect *clipP) : gameObject(location, textureAssetP, isStaticP, clipP)
{

	initTime = SDL_GetTicks();
	velocityY = -1;
	SDL_SetTextureAlphaMod(textureAsset, 255);
	lifeTime = 500;
	objectType = 5;
}


floatingText::~floatingText()
{
	
}



void floatingText::move(std::vector<gameObject*> gameObjectArray){

	//seuraaava koodinpätkä tekee "hienon" fade-efektin eli lisää teksturiin alphaa lifetimen kuluessa
	//olen aika ylpeä miten sekavannäköisen koodinpätkän sain aikaan
	float suhdeLuku = 0;
	timeDifference = SDL_GetTicks() - initTime;
	if ((float)timeDifference / lifeTime < 1) {
		suhdeLuku = (float)timeDifference / lifeTime;
		SDL_SetTextureAlphaMod(textureAsset, 255 - (255 * suhdeLuku));
	}
	
	//lisätään tekstiobjekti poistettavaksi lifetimen kuluttua
	if ((initTime + lifeTime) < SDL_GetTicks()) {
		
		status = -1;
	}

	float newX = position.getX() + velocityX;
	float newY = position.getY() + velocityY;

	position.setPoint(newX, newY);

	rect.x = position.getX();
	rect.y = position.getY();

}