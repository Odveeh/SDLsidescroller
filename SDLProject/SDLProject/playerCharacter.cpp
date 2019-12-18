#include "playerCharacter.h"



playerCharacter::playerCharacter(const Point2D &location, SDL_Texture *textureAssetP, bool isStaticP, SDL_Rect *clipP) : gameObject(location, textureAssetP, isStaticP, clipP)
{
	objectType = 2;
	playerHealth = 3;
	isTakingDamage = false;
	gameScore = 0;
	currentAnimState = idle;
	frameCounter = 0;
	movementDirection = right;

	justTookDamage = false;
	tookDamageCounter = 0;

	rect.w *= 3;
	rect.h *= 3;

}


playerCharacter::~playerCharacter()
{
}


void playerCharacter::render(SDL_Renderer *myRenderer, SDL_Rect camera) {

	if (justTookDamage && tookDamageCounter < 30) {
		tookDamageCounter++;
		SDL_SetTextureColorMod(textureAsset, 255, 0, 0);

	}
	else {
		SDL_SetTextureColorMod(textureAsset, 255, 255, 255);
		justTookDamage = false;
		tookDamageCounter = 0;

	}

	//kaikki renderˆid‰‰n suhteessa kameraan
	SDL_Rect renderRect = {rect.x - camera.x, rect.y - camera.y, rect.w, rect.h};

	//renderˆid‰‰n tekstuuri flipattuna riippuen siit‰ liikkuuko hahmo vasemmalle vai oikealle
	if (movementDirection == right) {
		SDL_RenderCopy(myRenderer, textureAsset, clip, &renderRect);
	}
	else {
		SDL_RenderCopyEx(myRenderer, textureAsset, clip, &renderRect, 0, NULL, SDL_FLIP_HORIZONTAL);
	}
	

	//spritesheet animation
	if (clip != NULL) {
		updateTextureClip();
	}
}


void playerCharacter::move(std::vector<gameObject*> gameObjectArray, HUD *myHUD, renderer myRenderer, bool *levelFinished) {

	setCurrentAnimState();

	float newX = position.getX() + velocityX;
	float newY = position.getY() + velocityY;

	position.setPoint(newX, newY);

	rect.x = position.getX();
	rect.y = position.getY();

	//muuttuja forlooppia varten jos isInAir toteutuu yhdell‰k‰‰n objektilla niin ei tarvitse sen j‰lkeen testailla
	bool isInAirHelperBool = false;
	bool collidedWithEnemy = false;

	//hardcoded array length :S
	for (int i = 0; i < gameObjectArray.size(); i++) {

		SDL_Rect *otherRect = &gameObjectArray[i]->rect;

		SDL_Rect resultRect;
		SDL_bool collisionBool = SDL_IntersectRect(&rect, otherRect, &resultRect);



		//hardcoded dimensions atm, need to fix
// || position.getX() < -2000 || position.getY() < 0 || position.getX() + rect.w > 2000 || position.getY() + rect.h > 600
		if (collisionBool == SDL_TRUE && this != gameObjectArray[i]) {

			if (gameObjectArray[i]->isStatic) {
				//std::cout << "static collision!" << std::endl;

				int newX = position.getX() - velocityX;
				int newY = position.getY() - velocityY;

				position.setPoint(newX, newY);

				rect.x = position.getX();
				rect.y = position.getY();

				

				velocityX = 0;
				velocityY = 0;

			}
			else {
				if (gameObjectArray[i]->objectType == 1) {

					if (this->objectType == 2) {
			
						collidedWithEnemy = true;
						//std::cout << "enemy collision" << std::endl;
					
						if (isTakingDamage == false) {
							
							takingDamageStartTime = SDL_GetTicks();
							takeDamage(myHUD);
						
							isTakingDamage = true;

							justTookDamage = true;
							
						}
						else {

							justTookDamage = true;
							
							takingDamageDuration = SDL_GetTicks() - takingDamageStartTime;

							if (takingDamageDuration > 500) {
					
								takeDamage(myHUD);
								takingDamageStartTime = SDL_GetTicks();
							}

						}
					
			
						
					}

				}
				//3 == pickupitem
				else if (gameObjectArray[i]->objectType == 3) {
		
					changegameScore(100, myHUD, myRenderer);
					gameObjectArray[i]->status = 0;
					
				}

				else if (gameObjectArray[i]->objectType == 7) {

					std::cout << "level finished!" << std::endl;
					*levelFinished = true;
				}
				else if (gameObjectArray[i]->objectType == 10) {
					//MASSIVE PICKUP ITEM
					changegameScore(500, myHUD, myRenderer);
					gameObjectArray[i]->status = 0;

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
		if (groundCollisionBool == SDL_TRUE && gameObjectArray[i]->objectType != 4 && gameObjectArray[i]->objectType != 3 && gameObjectArray[i]->objectType != 2) {
			//std::cout << "ground collision!!" << std::endl;
			isInAir = false;

			isInAirHelperBool = true;
		}
		else {

			if (!isInAirHelperBool) {
				isInAir = true;
			}

		}
		

		if (i == gameObjectArray.size()-1 && collidedWithEnemy == false && isTakingDamage) {
			isTakingDamage = false;
		}

	}



}


void playerCharacter::takeDamage(HUD *myHUD) {

	playerHealth -= 1;
	myHUD->deleteAHeart();

}



void playerCharacter::changegameScore(int x, HUD *myHUD, renderer myRenderer) {

	gameScore += x; 

	myHUD->updateScoreText(gameScore, myRenderer);

}


void playerCharacter::setCurrentAnimState() {

	//Sets movement direction
	if (velocityX > 0.1f) {
		movementDirection = right;
	}
	else if(velocityX < -0.1f) {
		movementDirection = left;
	}


	//sets animstate
	if (isInAir) {

		currentAnimState = jump;
	}
	else {

		if ((velocityX < 0.3f && velocityY < 0.1f) && (velocityX > -0.3f && velocityY > -0.1f)) {

			currentAnimState = idle;
		}

		else {

			currentAnimState = walk;

		}
		

	}


}


void playerCharacter::updateTextureClip() {

	frameCounter++;
	
	//vaihtaa spritesheetin aktiivista osaa (Clip) olettaen ett‰ koko tekstuuri on t‰ytetty samankokoisilla osilla

	//kuva vaihtuu viiden framen v‰lein
	if (frameCounter == 15) {
		frameCounter = 0;

		switch (currentAnimState) {

		case idle:
			clip->y = 0;

			if (clip->x < clip->w * 12) {

				if (clip->x < clip->w * 9) {
					clip->x = clip->w * 9;
				}

				clip->x += clip->w;
			}
			else {
				clip->x = clip->w * 9;
			}
			break;

		case walk:
			clip->y = 0;

			if (clip->x < clip->w * 22) {
				if (clip->x < clip->w * 17) {
					clip->x = clip->w * 17;
				}
				clip->x += clip->w;
			}
			else {
				clip->x = clip->w * 17;
			}
			break;

		case jump:
			clip->x = clip->w * 18;
			clip->y = 0;

		}

	}

	
}
