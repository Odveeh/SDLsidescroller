#include "renderer.h"
#include <iostream>

renderer::renderer()
{
	window = SDL_CreateWindow("Sidescroller demo", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WIDTH, HEIGHT, SDL_WINDOW_SHOWN);
	if (window == NULL) {
		std::cout << "sdl window could not be created" << SDL_GetError() << std::endl;
	}

	SDLrenderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
	windowSurface = SDL_GetWindowSurface(window);

	myFont = TTF_OpenFont("./resources/PixelFJVerdana12pt.ttf", 40);
	if (myFont == NULL) {
		std::cout << "couldnt open font!" << TTF_GetError() << std::endl;
	}

}

renderer::~renderer()
{
}

void renderer::renderGame() {

	SDL_Rect rect;
	rect.x = 50;
	rect.y = 100;
	rect.w = 200;
	rect.h = 100;
	
	windowSurface = NULL;

}
void renderer::setMousePosition(int x, int y) {

	mouseX = x;
	mouseY = y;
}

SDL_Texture *renderer::loadTexture(std::string texturepath) {

	SDL_Texture *texture = NULL;

	//load image at path
	SDL_Surface* loadedSurface = SDL_LoadBMP(texturepath.c_str());
	if (loadedSurface == NULL) {
		std::cout << "Could not load BMP" << texturepath.c_str() << SDL_GetError() << std::endl;

	}
	else {
		//create texture from surface pixels
		texture = SDL_CreateTextureFromSurface(SDLrenderer, loadedSurface);
		if (texture == NULL) {
			std::cout << "Unable to create texture from surface" << SDL_GetError() << std::endl;
		}

		//free loadedsurface
		SDL_FreeSurface(loadedSurface);

	}

	return texture;

}

SDL_Texture *renderer::textureFromText(std::string textureText, SDL_Color textColor) {

	SDL_Texture *texture = NULL;

	SDL_Surface *textSurface = TTF_RenderText_Solid(myFont, textureText.c_str(), textColor);
	if (textSurface == NULL) {
		std::cout << "unable to render textr surface" << TTF_GetError() << std::endl;
	}
	else {
		texture = SDL_CreateTextureFromSurface(SDLrenderer, textSurface);
		if (texture == NULL) {
			std::cout << "unabvle to create texture ffrom reenderer text" << SDL_GetError() << std::endl;

		}
	
		SDL_FreeSurface(textSurface);
	}

	return texture;
}

