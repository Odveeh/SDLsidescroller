#include "InputHandler.h"


InputHandler::InputHandler()
{
	spawnProjectile = false;
	SDL_ShowCursor(SDL_DISABLE);
}


InputHandler::~InputHandler()
{
}


void InputHandler::HandleInput(bool *programQuit, bool *quit, playerCharacter *myChar, bool *gamePaused, bool *levelFinished) {


	while(SDL_PollEvent(&event)){
		
		if (event.type == SDL_QUIT) {
			//std::cout << "QUIT PRESSED!" << std::endl;
			*quit = true;
			*programQuit = true;
			*levelFinished = false;
		
		}
		else {

			switch (event.type) {

				case SDL_KEYDOWN:
					//std::cout << "Key press detected!" << std::endl;
					
					switch (event.key.keysym.sym) {
						case SDLK_a:
							//std::cout << "a pressed!!" << std::endl;
					
							myChar->velocityX = -5;
							break;
						case SDLK_w:
							//std::cout << "w pressed!!" << std::endl;
				
							myChar->velocityY = -5;
							break;
						case SDLK_s:
							//std::cout << "s pressed!!" << std::endl;
					
							myChar->velocityY = 5;
							break;
						case SDLK_d:
							//std::cout << "d pressed!!" << std::endl;
					
							myChar->velocityX = 5;
							break;

						case SDLK_r:
							myChar->playerHealth = 3;
							*gamePaused = false;
							*quit = true;
							*levelFinished = false;
							break;

						case SDLK_SPACE:
							//std::cout << "JUMP BUTTON" << std::endl;
							if (myChar->isInAir == false) {
								myChar->velocityY -= 10;
							}
							break;
						case SDLK_ESCAPE:
							if (*gamePaused == false) {
								*gamePaused = true;
							}
							else {
								*gamePaused = false;
						
							}
						
							break;
					}

					break;

				case SDL_KEYUP:

					switch (event.key.keysym.sym) {
						case SDLK_a:
							//std::cout << "a pressed!!" << std::endl;

							myChar->velocityX = 0;
							break;

						case SDLK_d:
							//std::cout << "a pressed!!" << std::endl;

							myChar->velocityX = 0;
							break;
				
					}

					//std::cout << "Key release detected!" << std::endl;
					break;

				case SDL_MOUSEMOTION:

					//mouseY = event.motion.y;
					//mouseX = event.motion.x;

					SDL_GetMouseState(&mouseX, &mouseY);

					//std::cout << "mouse position: " << mouseX << ", " << mouseY << std::endl;
					break;
				case SDL_MOUSEBUTTONDOWN:
					//std::cout << "mosuebuttondown!" << std::endl;
					spawnProjectile = true;
			
				default:
					break;
				}


		}

	
	}


}
