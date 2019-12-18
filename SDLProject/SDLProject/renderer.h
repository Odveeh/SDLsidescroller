#pragma once
#include "SDL.h"
#include <string>
#include "SDL_ttf.h"

class renderer
{
public:

	int WIDTH = 1000;
	int HEIGHT = 800;

	int mouseX = 0;
	int mouseY = 0;

	SDL_Window *window = NULL;
	SDL_Renderer *SDLrenderer = NULL;

	

	SDL_Surface *windowSurface = NULL;

	renderer();
	~renderer();

	SDL_Texture *loadTexture(std::string texturepath);

	void setMousePosition(int x, int y);
	void renderGame();

	TTF_Font *myFont = NULL;
	SDL_Texture *textureFromText(std::string textureText, SDL_Color textColor);
	
};

