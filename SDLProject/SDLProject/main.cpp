#include "SDL.h"
#include "InputHandler.h"
#include "renderer.h"
#include <iostream>
#include "gameObject.h"
#include "Point2D.h"
#include "Projectile.h"
#include <vector>
#include "HUD.h"
#include "Enemy.h"
#include "playerCharacter.h"
#include "SDL_ttf.h"
#include "pickUpItem.h"
#include "floatingText.h"
#include <cmath>
#include "ParticleSystem.h"

bool quit = false;
bool gamePaused = false;
bool programQuit = false;
bool levelFinished = false;

const int FPS_CAP = 120;
const float millisPerFrame = 1000 / FPS_CAP;
float timeInMillis;
float timeAtFrameStart;
float timeAtFrameEnd;
float timeDifference;
float frames = 0;
float averageFPS;

std::vector<gameObject*> AllGameObjects;

int main(int argc, char *argv[]) {

	if (SDL_Init(SDL_INIT_EVERYTHING) == -1) {
		std::cout << "sdl could not be initalized" << SDL_GetError() << std::endl;

	};

	if (TTF_Init() == -1) {
		std::cout << "could not initialize TTF" << TTF_GetError() << std::endl;
	}


	Projectile *spawnProjectile(float xPos, float yPos, float xVel, float yVel, SDL_Texture *texture, SDL_Rect *clip, ParticleSystem2 *particleSystemP);
	void destroyAllAllocatedObjects(std::vector <gameObject*>);

	InputHandler inputHandler;
	renderer myRenderer;

	const int CAMERA_WIDTH = myRenderer.WIDTH;
	const int CAMERA_HEIGHT = myRenderer.HEIGHT;

	//YOU DIED TEXT RESOURCES
	SDL_Rect textRect2 = { 0, 0, 500, 300 };
	SDL_Color color2 = { 50, 0, 0 };
	SDL_Texture *textTexture2 = myRenderer.textureFromText("YOU DIED! press R to restart.", color2);

	//YOU FINISHED LEVEL TEXT RESOURCES
	SDL_Rect textRect3 = { 0, 0, 500, 300 };
	SDL_Texture *textTexture3 = myRenderer.textureFromText("Congrats! You beat the game!", color2);

	SDL_Rect textRect4 = { 0, 0, 500, 300 };

	SDL_Texture *textTexture5 = myRenderer.textureFromText("Press R to replay.", color2);
	SDL_Rect textRect5 = { 0, 0, 500, 300 };

	//SCALING TEXT TEXTURES AND SETTING POSITIONS
	SDL_QueryTexture(textTexture2, NULL, NULL, &textRect2.w, &textRect2.h);
	textRect2.w /= 2;
	textRect2.h /= 2;
	textRect2.x = CAMERA_WIDTH / 2 - textRect2.w / 2;
	textRect2.y = CAMERA_HEIGHT / 2 - textRect2.h / 2;

	SDL_QueryTexture(textTexture3, NULL, NULL, &textRect3.w, &textRect3.h);
	textRect3.w /= 2;
	textRect3.h /= 2;
	textRect3.x = CAMERA_WIDTH / 2 - textRect3.w / 2;
	textRect3.y = (CAMERA_HEIGHT / 2 - textRect3.h / 2) - textRect3.h;

	SDL_QueryTexture(textTexture5, NULL, NULL, &textRect5.w, &textRect5.h);
	textRect5.w /= 2;
	textRect5.h /= 2;
	textRect5.x = CAMERA_WIDTH / 2 - textRect3.w / 2;
	textRect5.y = (CAMERA_HEIGHT / 2 - textRect3.h / 2) + textRect3.h * 2;

	//CHARACTER RESOURCES
	Point2D startPos(-700.0f, 300.0f);
	SDL_Texture *myTexture = NULL;
	SDL_Rect clipC;
	clipC.x = 0;
	clipC.y = 0;
	clipC.w = 30;
	clipC.h = 37;
	myTexture = myRenderer.loadTexture("./resources/adventurer.bmp");

	//PROJECTILE RESOURCES
	Point2D startPos4(400.0f, 400.0f);
	SDL_Texture *myTexture4 = NULL;
	SDL_Rect clip1;
	clip1.x = 0;
	clip1.y = 0;
	clip1.w = 60;
	clip1.h = 50;
	myTexture4 = myRenderer.loadTexture("./resources/sunburnsheet.bmp");

	//ENEMY RESOURCES
	Point2D enemyPos(-300.0f, 400.0f);
	SDL_Texture *enemyTexture = NULL;
	enemyTexture = myRenderer.loadTexture("./resources/Ghost-Files/ghost-idle3.bmp");
	SDL_Rect enemyClip;
	enemyClip.x = 0;
	enemyClip.y = 0;
	enemyClip.w = 30;
	enemyClip.h = 46;

	//SECONDARY ENEMY CLIP
	SDL_Rect enemyClip2;
	enemyClip2.x = 64;
	enemyClip2.y = 0;
	enemyClip2.w = 30;
	enemyClip2.h = 46;

	//ENEMY POSITIONS
	Point2D enemyPos2(650.0f, 20.0f);
	Point2D enemyPos3(2200.0f, 300.0f);
	Point2D enemyPos4(2400.0f, 300.0f);
	Point2D enemyPos5(2600.0f, 300.0f);
	Point2D enemyPos6(2800.0f, 300.0f);
	Point2D enemyPos7(3000.0f, 300.0f);
	Point2D enemyPos8(-1600.0f, 400.0f);

	//CROSSHAIR RESOURCES
	Point2D crosshairStartPos(0.0f, 0.0f);
	SDL_Texture *myTextureCrossHair = NULL;
	myTextureCrossHair = myRenderer.loadTexture("./resources/crosshair2.bmp");
	gameObject crossHair(crosshairStartPos, myTextureCrossHair, false, NULL);

	//BGTEXTURE1
	SDL_Texture *BGTexture1 = NULL;
	SDL_Rect BGRect1;
	BGRect1.x = -1000;
	BGRect1.y = -500;
	BGRect1.w = 592 * 5;
	BGRect1.h = 272 * 5;
	BGTexture1 = myRenderer.loadTexture("./resources/BG/parallax-demon-woods-close-trees.bmp");

	//BGTEXTURE2
	SDL_Texture *BGTexture2 = NULL;
	SDL_Rect BGRect2;
	BGRect2.x = -1000;
	BGRect2.y = -500;
	BGRect2.w = 592 * 5;
	BGRect2.h = 272 * 5;
	BGTexture2 = myRenderer.loadTexture("./resources/BG/parallax-demon-woods-mid-trees.bmp");
	//SDL_SetTextureAlphaMod(BGTexture2, 100);
	
	//BGTEXTURE3
	SDL_Texture *BGTexture3 = NULL;
	SDL_Rect BGRect3;
	BGRect3.x = -1000;
	BGRect3.y = -500;
	BGRect3.w = 592 * 5;
	BGRect3.h = 272 * 5;
	BGTexture3 = myRenderer.loadTexture("./resources/BG/parallax-demon-woods-far-trees.bmp");
	//SDL_SetTextureAlphaMod(BGTexture3, 50);

	//BGTEXTURE4
	SDL_Texture *BGTexture4 = NULL;
	SDL_Rect BGRect4;
	BGRect4.x = -1000;
	BGRect4.y = -500;
	BGRect4.w = 592 * 5;
	BGRect4.h = 272 * 5;
	BGTexture4 = myRenderer.loadTexture("./resources/BG/parallax-demon-woods-bg.bmp");
	SDL_SetTextureAlphaMod(BGTexture4, 80);

	//PARTICLESYSTEMTESST
	Point2D point(300.0f, 300.0f);
	ParticleSystem myParticleSystem(1000, point);

	//PARTICLESYSTEM2TEST
	Point2D point2(300.0f, 300.0f);
	ParticleSystem2 myParticleSystem2(1000, point2);

	//BGRECT1 COPY
	SDL_Rect BGRect1C;

	//BGRECT2 COPY
	SDL_Rect BGRect2C;

	//BGRECT3 COPY
	SDL_Rect BGRect3C;

	//Active BGrect1
	SDL_Rect* activeRect;
	activeRect = &BGRect1;

	//ACCESSORY BGRECT1
	SDL_Rect* helperRect;
	helperRect = &BGRect1C;

	//Active BGrect2
	SDL_Rect* activeRect2;
	activeRect2 = &BGRect2;

	//ACCESSORY BGRECT2
	SDL_Rect* helperRect2;
	helperRect2 = &BGRect2C;

	//Active BGrect3
	SDL_Rect* activeRect3;
	activeRect3 = &BGRect3;

	//ACCESSORY BGRECT3
	SDL_Rect* helperRect3;
	helperRect3 = &BGRect3C;


	while(!programQuit){

		HUD myHUD(&myRenderer);

		//CLEAR GAME OBJECT VECTOR FOR GAME RESTART
		AllGameObjects.clear();

		//MAIN CAMERA
		SDL_Rect camera = { 0, 0, CAMERA_WIDTH, CAMERA_HEIGHT };

		std::vector<gameObject> generateLevelGeometry(renderer Myrenderer);
		std::vector<gameObject> levelGeometria = generateLevelGeometry(myRenderer);

		//creating character object
		playerCharacter myCharacter(startPos, myTexture, false, &clipC);


		for (int i = 0; i < levelGeometria.size(); i++) {
			AllGameObjects.push_back(&levelGeometria[i]);
		}

		//ADDING ENEMY OBJECTS TO GAME OBJECT VECTOR
		Enemy enemy1(enemyPos, enemyTexture, false, &enemyClip);
		AllGameObjects.push_back(&enemy1);

		Enemy enemy2(enemyPos2, enemyTexture, false, &enemyClip);
		AllGameObjects.push_back(&enemy2);

		Enemy enemy3(enemyPos3, enemyTexture, false, &enemyClip);
		AllGameObjects.push_back(&enemy3);

		Enemy enemy4(enemyPos4, enemyTexture, false, &enemyClip);
		AllGameObjects.push_back(&enemy4);

		Enemy enemy5(enemyPos5, enemyTexture, false, &enemyClip);
		AllGameObjects.push_back(&enemy5);

		Enemy enemy6(enemyPos6, enemyTexture, false, &enemyClip);
		AllGameObjects.push_back(&enemy6);

		Enemy enemy8(enemyPos8, enemyTexture, false, &enemyClip2);
		AllGameObjects.push_back(&enemy8);

	
		//main game loop
		while (!quit) {
		
			frames++;
			timeInMillis = SDL_GetTicks();
			timeAtFrameStart = timeInMillis;

			inputHandler.HandleInput(&programQuit, &quit, &myCharacter, &gamePaused, &levelFinished);

			//CLEAR RENDERER
			SDL_SetRenderDrawColor(myRenderer.SDLrenderer, 0, 0, 0, 0);
			SDL_RenderClear(myRenderer.SDLrenderer);

			float BGLayer2Suhde = 0.85f;
			float BGLayer3Suhde = 0.70f;

			//rendering background textures with parallax effect
			SDL_Rect BGRenderRect = { activeRect->x - camera.x, activeRect->y - camera.y, activeRect->w, activeRect->h };
			SDL_Rect BGRenderRect2 = { activeRect2->x - (camera.x * BGLayer2Suhde), activeRect2->y - camera.y, activeRect2->w, activeRect2->h };
			SDL_Rect BGRenderRect3 = { activeRect3->x - (camera.x * BGLayer3Suhde), activeRect3->y - camera.y, activeRect3->w, activeRect3->h };

			SDL_Rect windowRenderRect = { 0, 0, myRenderer.WIDTH, myRenderer.HEIGHT };

			SDL_RenderCopy(myRenderer.SDLrenderer, BGTexture4, NULL, &windowRenderRect);
			SDL_RenderCopy(myRenderer.SDLrenderer, BGTexture4, NULL, &windowRenderRect);

			SDL_RenderCopy(myRenderer.SDLrenderer, BGTexture3, NULL, &BGRenderRect3);

			//THIRD BG LAYER SWITCHING
			if (activeRect3->x + activeRect3->w < camera.x + camera.w) {
				if (camera.x > activeRect3->x + activeRect3->w * BGLayer3Suhde) {

					SDL_Rect* tempRect = activeRect3;
					activeRect3 = helperRect3;
					helperRect3 = tempRect;

				}

				helperRect3->x = activeRect3->x + activeRect3->w;
				helperRect3->y = activeRect3->y;
				helperRect3->w = activeRect3->w;
				helperRect3->h = activeRect3->h;

				SDL_Rect BGRenderRectC3 = { helperRect3->x - (camera.x * BGLayer3Suhde), helperRect3->y - camera.y, helperRect3->w, helperRect3->h };
				SDL_RenderCopy(myRenderer.SDLrenderer, BGTexture3, NULL, &BGRenderRectC3);

			}
			else if (camera.x * BGLayer3Suhde < activeRect3->x) {
				
				helperRect3->x = activeRect3->x - activeRect3->w;
				helperRect3->y = activeRect3->y;
				helperRect3->w = activeRect3->w;
				helperRect3->h = activeRect3->h;

				SDL_Rect BGRenderRectC3 = { helperRect3->x - (camera.x * BGLayer3Suhde), helperRect3->y - camera.y, helperRect3->w, helperRect3->h };
				SDL_RenderCopy(myRenderer.SDLrenderer, BGTexture3, NULL, &BGRenderRectC3);

				if (camera.x * BGLayer3Suhde + camera.w < activeRect3->x) {

					SDL_Rect* tempRect = activeRect3;
					activeRect3 = helperRect3;
					helperRect3 = tempRect;

				}
			}

			//SECOND ACTIVE BG LAYER REND
			SDL_RenderCopy(myRenderer.SDLrenderer, BGTexture2, NULL, &BGRenderRect2);
			SDL_RenderCopy(myRenderer.SDLrenderer, BGTexture4, NULL, &windowRenderRect);

			//SECOND BG LAYER SWITCHING
			if (activeRect2->x + activeRect2->w < camera.x + camera.w) {
				if (camera.x > activeRect2->x + activeRect2->w * BGLayer2Suhde) {

					SDL_Rect* tempRect = activeRect2;
					activeRect2 = helperRect2;
					helperRect2 = tempRect;

				}
			
				helperRect2->x = activeRect2->x + activeRect2->w;
				helperRect2->y = activeRect2->y;
				helperRect2->w = activeRect2->w;
				helperRect2->h = activeRect2->h;

				SDL_Rect BGRenderRectC2 = { helperRect2->x - (camera.x * BGLayer2Suhde), helperRect2->y - camera.y, helperRect2->w, helperRect2->h };
				SDL_RenderCopy(myRenderer.SDLrenderer, BGTexture2, NULL, &BGRenderRectC2);

			}
			else if (camera.x * BGLayer2Suhde < activeRect2->x ) {
				

			
				helperRect2->x = activeRect2->x - activeRect2->w ;
				helperRect2->y = activeRect2->y;
				helperRect2->w = activeRect2->w;
				helperRect2->h = activeRect2->h;

				SDL_Rect BGRenderRectC2 = { helperRect2->x - (camera.x * BGLayer2Suhde), helperRect2->y - camera.y, helperRect2->w, helperRect2->h };
				SDL_RenderCopy(myRenderer.SDLrenderer, BGTexture2, NULL, &BGRenderRectC2);


				if (camera.x * BGLayer2Suhde + camera.w < activeRect2->x) {
					SDL_Rect* tempRect = activeRect2;
					activeRect2 = helperRect2;
					helperRect2 = tempRect;

				}
			}

			//RENDERING FIRST ACTIVE BGLAYER
			SDL_RenderCopy(myRenderer.SDLrenderer, BGTexture1, NULL, &BGRenderRect);

			//FIRST BG LAYER SWITCHING
			if (activeRect->x + activeRect->w < camera.x + camera.w) {
				if (camera.x > activeRect->x + activeRect->w) {
					//std::cout << "active bg poissa n‰kyvist‰ kokonaan oikealla, vaihdetaan active ja helper" << std::endl;

					SDL_Rect* tempRect = activeRect;
					activeRect = helperRect;
					helperRect = tempRect;

				}

				//BG1
				helperRect->x = activeRect->x + activeRect->w;
				helperRect->y = activeRect->y;
				helperRect->w = activeRect->w;
				helperRect->h = activeRect->h;

				SDL_Rect BGRenderRectC = { helperRect->x - camera.x, helperRect->y - camera.y, helperRect->w, helperRect->h };
				SDL_RenderCopy(myRenderer.SDLrenderer, BGTexture1, NULL, &BGRenderRectC);
				
			}

			else if (camera.x < activeRect->x) {
				//std::cout << "vasen bg loppui" << std::endl;

				//BG1
				helperRect->x = activeRect->x - activeRect->w;
				helperRect->y = activeRect->y;
				helperRect->w = activeRect->w;
				helperRect->h = activeRect->h;

				SDL_Rect BGRenderRectC = { helperRect->x - camera.x, helperRect->y - camera.y, helperRect->w, helperRect->h };
				SDL_RenderCopy(myRenderer.SDLrenderer, BGTexture1, NULL, &BGRenderRectC);
	

				if (camera.x + camera.w < activeRect->x) {
					//std::cout << "active bg poissa n‰kyvist‰ oikealla, vaihdetan active ja helper" << std::endl;
					SDL_Rect* tempRect = activeRect;
					activeRect = helperRect;
					helperRect = tempRect;

				}
			}
			
			SDL_SetRenderDrawBlendMode(myRenderer.SDLrenderer, SDL_BLENDMODE_BLEND);
			SDL_SetRenderDrawColor(myRenderer.SDLrenderer, 0, 0, 0, 150);
			SDL_Rect fillRect4;
			fillRect4.x = 0;
			fillRect4.y = 0;
			fillRect4.w = CAMERA_WIDTH;
			fillRect4.h = CAMERA_HEIGHT;
			SDL_RenderFillRect(myRenderer.SDLrenderer, &fillRect4);

				
			//render all objects in the game object vector
			for (int i = 0; i < AllGameObjects.size(); i++) {
		
				AllGameObjects[i]->render(myRenderer.SDLrenderer, camera);

				//move objects that aren't static
				if (!AllGameObjects[i]->isStatic) {
					AllGameObjects[i]->move(AllGameObjects);
				
				}
				
			}

			//IF PLAYER CLICKED MOUSEBUTTON -> SPAWN PROJECTILE
			if (inputHandler.spawnProjectile == true) {

				float xVel = (inputHandler.mouseX + camera.x) - myCharacter.position.getX();
				float yVel = (inputHandler.mouseY + camera.y) - myCharacter.position.getY();

				//ei oikea tapa laskea, mutta toimii sinnep‰in:
				float suhdeLuku = xVel / yVel;
				//std::cout << "suhdeluku: " << suhdeLuku << std::endl;

				float actualVel = sqrt(xVel*xVel + yVel * yVel);
				//std::cout << "actual vel: " << actualVel << std::endl;

				const float targetVel = 600.0f;
				float velSuhde = targetVel / actualVel;

				float newXVel = xVel * velSuhde;
				float newYVel = yVel * velSuhde;

				Point2D particleStartPos{ myCharacter.position.getX() + myCharacter.rect.w / 2, myCharacter.position.getY() + myCharacter.rect.h / 2 };
				ParticleSystem2 *myParticleSystem3 = new ParticleSystem2(2000, particleStartPos);

				AllGameObjects.push_back(spawnProjectile(myCharacter.position.getX(), myCharacter.position.getY(), newXVel / 100, newYVel / 100, myTexture4, &clip1, myParticleSystem3));
				//std::cout << "array size: " << AllGameObjects.size() << std::endl;
				inputHandler.spawnProjectile = false;
			}
		
			//moving and rendering crosshair
			crossHair.position.setPoint(inputHandler.mouseX, inputHandler.mouseY);
			crossHair.rect.x = crossHair.position.getX() + camera.x - (crossHair.rect.w / 2);
			crossHair.rect.y = crossHair.position.getY() + camera.y - (crossHair.rect.h / 2);
			crossHair.render(myRenderer.SDLrenderer, camera);

			//rendering main character
			myCharacter.render(myRenderer.SDLrenderer, camera);

			//rendering HUD
			myHUD.render(myRenderer.SDLrenderer);

			//SHOWING RENDER RESULT
			SDL_RenderPresent(myRenderer.SDLrenderer);

			//MOVING MAIN CHARACTER
			myCharacter.move(AllGameObjects, &myHUD, myRenderer, &levelFinished);

			//IF CHARACTER IS IN AIR, ADD GRAVITY
			if (myCharacter.isInAir == true) {
				myCharacter.velocityY = myCharacter.velocityY + myCharacter.gravityY;
			}
		
			//center camera to character:
			camera.x = (myCharacter.position.getX() + myCharacter.rect.w / 2) - myRenderer.WIDTH / 2;
			
			//camera.y:n rajoittaminen ettei menee backgroundin yli
			if (camera.y > BGRect1.y) {
		
				camera.y = (myCharacter.position.getY() + myCharacter.rect.h / 2) - myRenderer.HEIGHT / 1.5;
			}
			else {
				if (myCharacter.position.getY() - camera.y > myRenderer.HEIGHT / 1.5) {
					camera.y = (myCharacter.position.getY() + myCharacter.rect.h / 2) - myRenderer.HEIGHT / 1.5;
					}
					
				}
			
		
			//delete objects 
			for (std::vector <gameObject*>::iterator i = AllGameObjects.begin(); i != AllGameObjects.end(); ) {
			
				if ((*i)->status == 0) {
				
					//std::cout << "deleted with status 0" << std::endl;

					if ((*i)->objectType == 3) {

						//std::cout << "spawned a text" << std::endl;
						Point2D floatPos1((*i)->position.getX(), (*i)->position.getY());
						SDL_Color floatColor = { 255, 255, 255 };
						SDL_Texture *floatTexture = myRenderer.textureFromText("+100", floatColor);
						floatingText *floatingText2 = new floatingText(floatPos1, floatTexture, false, NULL);
						floatingText2->rect.w /= 3;
						floatingText2->rect.h /= 3;
						AllGameObjects.push_back(floatingText2);

					}
					else if ((*i)->objectType == 10) {
						//std::cout << "spawned a text" << std::endl;
						Point2D floatPos1((*i)->position.getX(), (*i)->position.getY());
						SDL_Color floatColor = { 255, 255, 255 };
						SDL_Texture *floatTexture = myRenderer.textureFromText("+500", floatColor);
						floatingText *floatingText2 = new floatingText(floatPos1, floatTexture, false, NULL);
						floatingText2->rect.w /= 2;
						floatingText2->rect.h /= 2;
						AllGameObjects.push_back(floatingText2);
					}
			
			
					gameObject *p = *i;

					i = AllGameObjects.erase(i);
					//++i;
					delete p;

				}

				else if ((*i)->status == -1) {
				
					//std::cout << "deleted with status -1" << std::endl;

					if ((*i)->objectType == 3) {

						//std::cout << "spawned a text" << std::endl;
						Point2D floatPos1((*i)->position.getX(), (*i)->position.getY());
						SDL_Color floatColor = { 255, 255, 255 };
						SDL_Texture *floatTexture = myRenderer.textureFromText("+100", floatColor);
						floatingText *floatingText2 = new floatingText(floatPos1, floatTexture, false, NULL);
						floatingText2->rect.w /= 3;
						floatingText2->rect.h /= 3;
						AllGameObjects.push_back(floatingText2);

					}

					if ((*i)->objectType == 4) {
						(*i)->destroy();
					}

					//kutsutaan stack objektin destructoria ja poistetaan vektorista, emt oks t‰‰ iha legit mutta toimii ainakin peliss‰
					//std::cout << "array size before: " << AllGameObjects.size() << std::endl;
					(*i)->~gameObject();

					//std::cout << "array size after: " << AllGameObjects.size() << std::endl;
					i = AllGameObjects.erase(i);
				
				}
				else {
					++i;
				}
			
			}


			SDL_Texture *textTexture4 = myRenderer.textureFromText("Your score: " + std::to_string(myCharacter.gameScore), color2);

			SDL_QueryTexture(textTexture4, NULL, NULL, &textRect4.w, &textRect4.h);
			textRect4.w /= 2;
			textRect4.h /= 2;
			textRect4.x = CAMERA_WIDTH / 2 - textRect3.w / 2;
			textRect4.y = (CAMERA_HEIGHT / 2 - textRect3.h / 2);

			while (levelFinished) {

				inputHandler.HandleInput(&programQuit, &quit, &myCharacter, &gamePaused, &levelFinished);

				SDL_SetRenderDrawBlendMode(myRenderer.SDLrenderer, SDL_BLENDMODE_BLEND);
				SDL_SetRenderDrawColor(myRenderer.SDLrenderer, 15, 15, 15, 50);
				SDL_Rect fillRect;
				fillRect.x = 0;
				fillRect.y = 0;
				fillRect.w = CAMERA_WIDTH;
				fillRect.h = CAMERA_HEIGHT;

				SDL_RenderFillRect(myRenderer.SDLrenderer, &fillRect);

				SDL_RenderCopy(myRenderer.SDLrenderer, textTexture3, NULL, &textRect3);
				SDL_RenderCopy(myRenderer.SDLrenderer, textTexture4, NULL, &textRect4);
				SDL_RenderCopy(myRenderer.SDLrenderer, textTexture5, NULL, &textRect5);

				SDL_RenderPresent(myRenderer.SDLrenderer);

			
			}
			SDL_DestroyTexture(textTexture4);


			//player death loop
			while (myCharacter.playerHealth <= 0) {

				//allows input handling inside the loop
				inputHandler.HandleInput(&programQuit, &quit, &myCharacter, &gamePaused, &levelFinished);

				SDL_SetRenderDrawBlendMode(myRenderer.SDLrenderer, SDL_BLENDMODE_BLEND);
				SDL_SetRenderDrawColor(myRenderer.SDLrenderer, 128, 0, 0, 5);
				SDL_Rect fillRect;
				fillRect.x = 0;
				fillRect.y = 0;
				fillRect.w = CAMERA_WIDTH;
				fillRect.h = CAMERA_HEIGHT;
			
				//death screen red overlay
				SDL_RenderFillRect(myRenderer.SDLrenderer, &fillRect);
				//render "death texture"
				SDL_RenderCopy(myRenderer.SDLrenderer, textTexture2, NULL, &textRect2);
				//show rendering result
				SDL_RenderPresent(myRenderer.SDLrenderer);

			}

			//pause game loop
			while (gamePaused) {

				//allow input handling inside pause loop
				inputHandler.HandleInput(&programQuit, &quit, &myCharacter, &gamePaused, &levelFinished);
			}

			//calculate how long it took from frame start to this point, and get the remainder time that we need to delay for FPS CAP
			timeAtFrameEnd = SDL_GetTicks();
			timeDifference = timeAtFrameEnd - timeAtFrameStart;

			//time delay to cap framerate
			if (timeDifference < millisPerFrame) {
				SDL_Delay(millisPerFrame - timeDifference);

			}

		}
		
	quit = false;

	}

	SDL_DestroyWindow(myRenderer.window);
	SDL_Quit();

	return 0;
}


std::vector<gameObject> generateLevelGeometry(renderer renderer1) {

	std::vector<gameObject> geometriaVektori;

	SDL_Texture *churchTexture = NULL;
	churchTexture = renderer1.loadTexture("./resources/Church.bmp");
	SDL_SetTextureColorMod(churchTexture, 80, 80, 80);
	Point2D startPosChurch(-1100.0f, 100.0f);
	gameObject churchObj(startPosChurch, churchTexture, true, NULL);
	churchObj.rect.w *= 4;
	churchObj.rect.h *= 4;
	geometriaVektori.push_back(churchObj);

	Point2D startPosChurch2(6100.0f, 100.0f);
	gameObject churchObj2(startPosChurch2, churchTexture, true, NULL);
	churchObj2.rect.w *= 4;
	churchObj2.rect.h *= 4;
	geometriaVektori.push_back(churchObj2);

	SDL_Texture *pickupTexture2 = NULL;
	pickupTexture2 = renderer1.loadTexture("./resources/gem2.bmp");

	SDL_Texture *pickupTexture3 = NULL;
	pickupTexture3 = renderer1.loadTexture("./resources/gem3.bmp");

	SDL_Texture *pickupTexture4 = NULL;
	pickupTexture4 = renderer1.loadTexture("./resources/gem4.bmp");

	Point2D startPos1(650.0f, 200.0f);
	SDL_Texture *myTexture1 = NULL;
	myTexture1 = renderer1.loadTexture("./resources/Platform_Center.bmp");
	SDL_SetTextureColorMod(myTexture1, 120, 120, 120);
	gameObject geometryObj1(startPos1, myTexture1, true, NULL);
	geometryObj1.rect.w *= 4;
	geometryObj1.rect.h *= 4;
	geometriaVektori.push_back(geometryObj1);

	Point2D pickupPos3(700.0f, 100.0f);
	pickUpItem *pickup3 = new pickUpItem(pickupPos3, pickupTexture2, false, NULL);
	pickup3->rect.w *= 2;
	pickup3->rect.h *= 2;
	AllGameObjects.push_back(pickup3);

	Point2D pickupPos4(50.0f, -20.0f);
	pickUpItem *pickup4 = new pickUpItem(pickupPos4, pickupTexture4, false, NULL);
	pickup4->rect.w *= 2;
	pickup4->rect.h *= 2;
	AllGameObjects.push_back(pickup4);

	//MASSIVE PICKUP
	Point2D pickupPos15(-1250.0f, 400.0f);
	pickUpItem *pickup15 = new pickUpItem(pickupPos15, pickupTexture2, false, NULL);
	pickup15->rect.w *= 5;
	pickup15->rect.h *= 5;
	pickup15->objectType = 10;
	AllGameObjects.push_back(pickup15);

	Point2D startPos2(0.0f, 50.0f);
	gameObject geometryObj2(startPos2, myTexture1, true, NULL);
	geometryObj2.rect.w *= 4;
	geometryObj2.rect.h *= 4;
	geometriaVektori.push_back(geometryObj2);
	
	Point2D startPos3(200.0f, 400.0f);
	gameObject geometryObj3(startPos3, myTexture1, true, NULL);
	geometryObj3.rect.w *= 4;
	geometryObj3.rect.h *= 4;

	geometriaVektori.push_back(geometryObj3);

	Point2D levelFinishPos(5000.0f, 300.0f);
	SDL_Texture *levelExitTexture = NULL;
	levelExitTexture = renderer1.loadTexture("./resources/exitdoor.bmp");
	gameObject levelExitObj(levelFinishPos, levelExitTexture, false, NULL);
	levelExitObj.objectType = 7;
	geometriaVektori.push_back(levelExitObj);
	
	SDL_Texture *pickupTexture = NULL;
	pickupTexture = renderer1.loadTexture("./resources/gem1.bmp");


	for (int i = 0; i < 10; i++) {
		Point2D pickupPos2(700.0f + i * 150, 500.0f);
		pickUpItem *pickup2 = new pickUpItem(pickupPos2, pickupTexture, false, NULL);
		pickup2->rect.w *= 2;
		pickup2->rect.h *= 2;
		AllGameObjects.push_back(pickup2);

	}

	for (int i = 0; i < 10; i++) {
		Point2D platformPos2(2200.0f + i * 150, 500.0f);
		gameObject geometryObj2(platformPos2, myTexture1, true, NULL);
		geometryObj2.rect.w *= 4;
		geometryObj2.rect.h *= 4;
		geometriaVektori.push_back(geometryObj2);

	}

	Point2D platformPos9(3000.0f, 200.0f);
	gameObject geometryObj9(platformPos9, myTexture1, true, NULL);
	geometryObj9.rect.w *= 4;
	geometryObj9.rect.h *= 4;
	geometriaVektori.push_back(geometryObj9);

	Point2D platformPos11(3500.0f, 400.0f);
	gameObject geometryObj11(platformPos11, myTexture1, true, NULL);
	geometryObj11.rect.w *= 4;
	geometryObj11.rect.h *= 4;
	geometriaVektori.push_back(geometryObj11);

	Point2D platformPos12(2600.0f, 0.0f);
	gameObject geometryObj12(platformPos12, myTexture1, true, NULL);
	geometryObj12.rect.w *= 4;
	geometryObj12.rect.h *= 4;
	geometriaVektori.push_back(geometryObj12);

	Point2D platformPos13(2300.0f, -200.0f);
	gameObject geometryObj13(platformPos13, myTexture1, true, NULL);
	geometryObj13.rect.w *= 4;
	geometryObj13.rect.h *= 4;
	geometriaVektori.push_back(geometryObj13);

	for (int i = 0; i < 10; i++) {
		Point2D platformPos2(2700.0f + i * 150, -400.0f);
		gameObject geometryObj2(platformPos2, myTexture1, true, NULL);
		geometryObj2.rect.w *= 4;
		geometryObj2.rect.h *= 2;
		geometriaVektori.push_back(geometryObj2);

		Point2D pickupPos2(2900.0f + i * 100, -500.0f);
		pickUpItem *pickup2 = new pickUpItem(pickupPos2, pickupTexture3, false, NULL);
		pickup2->rect.w *= 2;
		pickup2->rect.h *= 2;
		AllGameObjects.push_back(pickup2);

	}

	
	SDL_Texture *myTexture5 = NULL;
	myTexture5 = renderer1.loadTexture("./resources/Platform_Center.bmp");

	SDL_Texture *myTexture6 = NULL;
	myTexture6 = renderer1.loadTexture("./resources/Under_Platform_Center.bmp");

	SDL_SetTextureColorMod(myTexture5, 50, 59, 50);
	SDL_SetTextureColorMod(myTexture6, 50, 59, 50);
	

	for (int i = 0; i < 200; i++) {
		Point2D startPosX(-2000.0f + i*51, 600.0f);
		gameObject geometryObjX(startPosX, myTexture5, true, NULL);
		geometryObjX.rect.w *= 3;
		geometryObjX.rect.h *= 3;
		geometriaVektori.push_back(geometryObjX);

		Point2D startPosX2(-2000.0f + i * 51, 600.0f + geometryObjX.rect.h);
		gameObject geometryObjX2(startPosX2, myTexture6, true, NULL);
		geometryObjX2.rect.w *= 3;
		geometryObjX2.rect.h *= 3;
		geometriaVektori.push_back(geometryObjX2);

	}

	//PROGRAMMER SIGNATURE TEXTURE
	SDL_Texture *signatureTexture = renderer1.loadTexture("./resources/programmedbyheader.bmp");
	Point2D sigPos(-800.0f, 700.0f);
	gameObject sigObj(sigPos, signatureTexture, true, NULL);
	sigObj.rect.w *= 1.5f;
	sigObj.rect.h *= 1.5f;
	geometriaVektori.push_back(sigObj);

	return geometriaVektori;
}

Projectile *spawnProjectile(float xPos, float yPos, float xVel, float yVel, SDL_Texture *texture, SDL_Rect *clip, ParticleSystem2 *particleSystemP) {

	Point2D projectilePos(xPos, yPos);
	SDL_Texture *projectileTexture = NULL;
	projectileTexture = texture;

	Projectile *projetilePointer = new Projectile(projectilePos, projectileTexture, false, clip, particleSystemP);
	projetilePointer->velocityX = xVel;
	projetilePointer->velocityY = yVel;

	return projetilePointer;
}

void destroyAllAllocatedObjects(std::vector <gameObject*> AllGameObjects) {

	std::cout << "isvectorempoty: " << AllGameObjects.empty() << std::endl;
	std::cout << "start destrouying stuff" << std::endl;
	for (std::vector <gameObject*>::iterator i = AllGameObjects.begin(); i != AllGameObjects.end(); ) {
	
		std::cout << *i << std::endl;

		//3 == pickupitem, 4 == projectile, 5 == floating text;
		if ((*i)->objectType == 3 || (*i)->objectType == 4 || (*i)->objectType == 5) {
			std::cout << "destryoing stuf" << "obecjt type: " << (*i)->objectType << "i adress: " << (*i) << std::endl;
			gameObject *p = *i;
			i = AllGameObjects.erase(i);
			//delete p;

		}
		else {
			++i;
		}
		
	
	}


}