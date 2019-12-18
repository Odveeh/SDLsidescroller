#include "HUD.h"


HUD::HUD(renderer *myRenderer)
{
	heartTexture = myRenderer->loadTexture("./resources/heart4.bmp");
	emptyHeartTexture = myRenderer->loadTexture("./resources/emptyheart.bmp");

	
	scoreRect.x = 800;
	scoreRect.y = 10;

	scoreTexture = myRenderer->textureFromText(scoreText, scoreTextColor);
	SDL_QueryTexture(scoreTexture, NULL, NULL, &scoreRect.w, &scoreRect.h);
	scoreRect.w /= 2;
	scoreRect.h /= 2;

	
	
	
	renderRect.x = 10;
	renderRect.y = 10;

	renderRect2.x = 72;
	renderRect2.y = 10;

	renderRect3.x = 132;
	renderRect3.y = 10;



	renderRect.w = 52;
	renderRect.h = 52;

	renderRect2.w = 52;
	renderRect2.h = 52;

	renderRect3.w = 52;
	renderRect3.h = 52;

	firstHeartTexture = heartTexture;
	secondHeartTexture = heartTexture;
	thirdHeartTexture = heartTexture;

	fullHearts = 3;


}


HUD::~HUD()
{
}


void HUD::render(SDL_Renderer *myRenderer) {
	

	SDL_RenderCopy(myRenderer, firstHeartTexture, NULL, &renderRect);
	SDL_RenderCopy(myRenderer, secondHeartTexture, NULL, &renderRect2);
	SDL_RenderCopy(myRenderer, thirdHeartTexture, NULL, &renderRect3);

	SDL_RenderCopy(myRenderer, scoreTexture, NULL, &scoreRect);

}


void HUD::deleteAHeart() {

	fullHearts--;

	if (fullHearts == 2) {
		thirdHeartTexture = emptyHeartTexture;
	}
	else if (fullHearts == 1) {
		secondHeartTexture = emptyHeartTexture;
	}
	else if (fullHearts == 0) {
		firstHeartTexture = emptyHeartTexture;
	}



}


void HUD::setAllHearts() {
	thirdHeartTexture = heartTexture;
	secondHeartTexture = heartTexture;
	firstHeartTexture = heartTexture;
}



void HUD::updateScoreText(int x, renderer myRenderer) {

	scoreText = "Score: " + std::to_string(x);

	//tuhotaan vanha tekstuuri kun sit‰ ei en‰‰ tarvita
	SDL_DestroyTexture(scoreTexture);

	//uusi tekstuuri
	scoreTexture = myRenderer.textureFromText(scoreText, scoreTextColor);
}