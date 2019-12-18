#include "Enemy.h"
#include <iostream>


Enemy::Enemy(const Point2D &location, SDL_Texture *textureAssetP, bool isStaticP, SDL_Rect *clipP) : gameObject(location, textureAssetP, isStaticP, clipP)
{
	objectType = 1;
	textureRowCount = 7;
	textureColumnCount = 1;

	rect.w *= 3;
	rect.h *= 3;
}

Enemy::~Enemy()
{
}

void Enemy::render(SDL_Renderer *myRenderer, SDL_Rect camera) {

	//kaikki renderöidään suhteessa kameraan
	SDL_Rect renderRect = { rect.x - camera.x, rect.y - camera.y, rect.w, rect.h };

	SDL_RenderCopy(myRenderer, textureAsset, clip, &renderRect);

	//spritesheet animation
	if (clip != NULL) {
		updateTextureClip();
	}

};

void Enemy::updateTextureClip()
{
	frameCounter++;

	//change textureclip every n:th frame
	if (frameCounter == 20) {
		frameCounter = 0;

			if (clip->x < clip->w * (textureRowCount -1)) {
				clip->x += clip->w;

			}
			else {
				clip->x = 0;
				//clip->y += clip->h;

			}

		}



	



	




};