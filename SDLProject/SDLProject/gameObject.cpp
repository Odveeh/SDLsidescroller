#include "gameObject.h"
#include <iostream>
#include <vector>

gameObject::gameObject() {


}


gameObject::gameObject(const Point2D &location, SDL_Texture *textureAssetP, bool isStaticP, SDL_Rect *clipP)
{

	status = 1;
	objectType = 0;

	clip = clipP;
	position = location;
	textureAsset = textureAssetP;

	SDL_QueryTexture(textureAsset, NULL, NULL, &textureWidth, &textureHeight);
	rect.w = textureWidth;
	rect.h = textureHeight;
	rect.x = (int)position.getX();
	rect.y = (int)position.getY();

	
	

	if (clip != NULL) {

		if (clip->w != 0 && clip->h != 0) {
			textureColumnCount = rect.w / clip->w;
			textureRowCount = rect.h / clip->h;
		}


		rect.w = clip->w;
		rect.h = clip->h;
	}

	//renderöidään defaulttina eli visibility = true
	visibility = true;
	isStatic = isStaticP;

	velocityX = 0.0f;
	velocityY = 0.0f;

}


gameObject::~gameObject()
{
	//std::cout << "gameobject basedestructor" << std::endl;
	destroy();
}




void gameObject::render(SDL_Renderer *myRenderer, SDL_Rect camera) {


	//kaikki renderöidään suhteessa kameraan
	SDL_Rect renderRect = {rect.x - camera.x, rect.y - camera.y, rect.w, rect.h };
	
	/*
	SDL_Rect *clip2 = NULL;

	if (clip != NULL) {
		clip2 = clip;
		clip2->w -= 20;
		clip2->h -= 20;
		clip2->x += 10;
		clip2->y += 10;
	}
	*/
	
	SDL_RenderCopy(myRenderer, textureAsset, clip, &renderRect);

	//spritesheet animation
	if (clip != NULL) {
		updateTextureClip();
	}
	

}


void gameObject::destroy() {

	//std::cout << "basedestroy!" << std::endl;
	
}

void gameObject::move(std::vector<gameObject*> gameObjectArray) {

	float newX = position.getX() + velocityX;
	float newY = position.getY() + velocityY;

	position.setPoint(newX, newY);

	rect.x = position.getX();
	rect.y = position.getY();

	//muuttuja forlooppia varten jos isInAir toteutuu yhdelläkään objektilla niin ei tarvitse sen jälkeen testailla
	bool isInAirHelperBool = false;

	//hardcoded array length :S
	for (int i = 0; i < gameObjectArray.size(); i++) {

		SDL_Rect *otherRect = &gameObjectArray[i]->rect;

		SDL_Rect resultRect;
		SDL_bool collisionBool = SDL_IntersectRect(&rect, otherRect, &resultRect);



		//hardcoded dimensions atm, need to fix
// || position.getX() < -2000 || position.getY() < 0 || position.getX() + rect.w > 2000 || position.getY() + rect.h > 600
		if (collisionBool == SDL_TRUE && this != gameObjectArray[i]) {

			if(gameObjectArray[i]->isStatic){
				//std::cout << "static collision!" << std::endl;

				int newX = position.getX() - velocityX;
				int newY = position.getY() - velocityY;

				position.setPoint(newX, newY);

				rect.x = position.getX();
				rect.y = position.getY();

				velocityX = 0;
				velocityY = 0;

			} else {
				if (gameObjectArray[i]->objectType == 1) {
					if (this->objectType == 2) {
						//std::cout << "enemy collision" << std::endl;
					}
					
				}
				
			}

		}

		//ground collision check -> change isInAir
		float groundX = position.getX();
		float groundY = position.getY() + 5;

		SDL_Rect groundRect = { groundX, groundY, rect.w, rect.h };
		SDL_Rect resultRect2;
		SDL_bool groundCollisionBool = SDL_IntersectRect(&groundRect, otherRect, &resultRect2);

		//hardcoded 600, need to change later
		if (groundCollisionBool == SDL_TRUE || groundY + rect.h > 600) {
			//std::cout << "ground collision!!" << std::endl;
			isInAir = false;
	
			isInAirHelperBool = true;
		}
		else {

			if (!isInAirHelperBool) {
				isInAir = true;
			}
			
		}

	}

	


	

}


void gameObject::updateTextureClip() {
	
	//vaihtaa spritesheetin aktiivista osaa (Clip) olettaen että koko tekstuuri on täytetty samankokoisilla osilla

	if (clip->y < clip->h * (textureColumnCount - 1)) {
			
		if (clip->x < clip->w * (textureRowCount - 1)) {
			clip->x += clip->w;
			
		
		}
		else {
			clip->x = 0;
			clip->y += clip->h;

		
		}

	}
	else {
		clip->y = 0;
		
		
	}
		
		
		
	
	

}
