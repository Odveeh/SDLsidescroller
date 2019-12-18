#pragma once
#include "SDL.h"
#include <iostream>
#include "renderer.h"
#include <string>

class HUD
{
public:
	HUD(renderer *myRenderer);
	~HUD();


	void render(SDL_Renderer *myRenderer);

	int textureWidth, textureHeight;

	SDL_Rect renderRect;
	SDL_Rect renderRect2;
	SDL_Rect renderRect3;
	SDL_Texture *heartTexture;
	SDL_Texture *emptyHeartTexture;

	SDL_Texture *firstHeartTexture;
	SDL_Texture *secondHeartTexture;
	SDL_Texture *thirdHeartTexture;

	int fullHearts;

	void deleteAHeart();
	void setAllHearts();

	SDL_Rect scoreRect;
	int score = 0;
	std::string scoreText = "Score: " + std::to_string(score);
	SDL_Texture *scoreTexture;
	SDL_Color scoreTextColor = { 255, 255, 255 };
	void updateScoreText(int x, renderer myRenderer);

};

