#pragma once
#include "gameObject.h"
#include <iostream>
#include "HUD.h"

class playerCharacter : public gameObject
{
public:
	playerCharacter(const Point2D &location, SDL_Texture *textureAssetP, bool isStaticP, SDL_Rect *clipP);
	~playerCharacter();

	void move(std::vector<gameObject*> gameObjectArray, HUD *myHUD, renderer myRenderer, bool *levelFinished);

	void takeDamage(HUD *myHUD);

	int playerHealth;
	int gameScore;

	void changegameScore(int x, HUD *myHUD, renderer myRenderer);
	
	bool isTakingDamage;
	float takingDamageStartTime;
	float takingDamageDuration;

	enum animState {idle, walk, jump, shoot};
	animState currentAnimState;
	void setCurrentAnimState();
	enum movementDirection{left, right};
	movementDirection movementDirection;

	void render(SDL_Renderer *myRenderer, SDL_Rect camera);
	void updateTextureClip();

	bool justTookDamage;
	int tookDamageCounter;

	SDL_Rect renderingRect;

	int frameCounter;
};

