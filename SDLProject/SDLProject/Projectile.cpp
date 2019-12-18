#include "Projectile.h"
#include <iostream>


Projectile::Projectile(const Point2D &location, SDL_Texture *textureAssetP, bool isStaticP, SDL_Rect *clipP, ParticleSystem2 *particleSystemPointerP) : gameObject(location, textureAssetP, isStaticP, clipP)
{

	objectType = 4;
	particleSystemPointer = particleSystemPointerP;
	maxDuration = 1000;
	killTime = SDL_GetTicks() + maxDuration;
	frameCounter = 0;
}


Projectile::~Projectile()
{
	destroy();
}


void Projectile::destroy() {

	delete particleSystemPointer;
}

void Projectile::move(std::vector<gameObject*> gameObjectArray) {

	if (killTime < SDL_GetTicks()) {

		status = -1;
	}

	particleSystemPointer->pSystemPos.setPoint(position.getX() + rect.w / 2, position.getY() + rect.h / 2);

	//std::cout << "projeectile shjieeet" << std::endl;

	float newX = position.getX() + velocityX;
	float newY = position.getY() + velocityY;

	position.setPoint(newX, newY);

	rect.x = position.getX();
	rect.y = position.getY();
	


	//collisionstuff
	for (int i = 0; i < gameObjectArray.size(); i++) {
		SDL_Rect *otherRect = &gameObjectArray[i]->rect;

		SDL_Rect resultRect;
		SDL_bool collisionBool = SDL_IntersectRect(&rect, otherRect, &resultRect);


		//this == gameObjectArray[i] on selfcollision mikä olisi aina totta eikä tätä tietenkään haluta tarkastaa
		if (collisionBool == SDL_TRUE && &gameObjectArray[i]->isStatic && this != gameObjectArray[i]) {
			if (gameObjectArray[i]->objectType != 4) {
				//std::cout << "projecitle ocllision" << std::endl;
				if (gameObjectArray[i]->objectType == 1) {
					//std::cout << "eneemy ocollision " << std::endl;
					gameObjectArray[i]->status = -1;
				}
				status = -1;
			}
		

		}

	}

}

void Projectile::render(SDL_Renderer *myRenderer, SDL_Rect camera) {

	SDL_Rect renderRect = { rect.x - camera.x, rect.y - camera.y, rect.w, rect.h };

	

	SDL_RenderCopy(myRenderer, textureAsset, clip, &renderRect);

	//spritesheet animation
	if (clip != NULL) {
		updateTextureClip();
	}

	particleSystemPointer->tick(myRenderer, camera);


}


void Projectile::updateTextureClip() {

	//vaihtaa spritesheetin aktiivista osaa (Clip) olettaen että koko tekstuuri on täytetty samankokoisilla osilla

	frameCounter++;

	if (frameCounter > 1) {
		frameCounter = 0;
		if (clip->y < clip->h * (textureColumnCount - 1)) {

			if (clip->x < clip->w * (textureRowCount - 2)) {
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
	






}