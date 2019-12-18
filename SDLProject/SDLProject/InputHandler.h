#pragma once

#include "SDL.h"
#include <iostream>
#include "gameObject.h"
#include <vector>
#include "playerCharacter.h"


class InputHandler
{
public:
	SDL_Event event;

	int mouseX, mouseY;
	
	bool spawnProjectile;
	
	InputHandler();
	~InputHandler();
	void HandleInput(bool *programQuit, bool *quit, playerCharacter *myChar, bool *gamePaused, bool *levelFinished);
};

