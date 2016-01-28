#ifndef PLAYER_HPP
#define PLAYER_HPP

#include <SDL.h>
#include "SDL_image.h"

class Player {
private:
	bool direction = true;
	SDL_Texture* m_texture;		//The final texture
	
	SDL_Surface* loadedSurface;		//Load image at specified path
	SDL_Rect rec;
	SDL_Rect prev_rec;
public:
	Player()	{
		rec = SDL_Rect();
		rec.x = 0;
		rec.y = 100;

	}
	Player(float x, float y, SDL_Renderer* gRenderer) {
		rec = SDL_Rect();
		rec.x = int(x);
		rec.y = int(y);
		loadTexture("Player.png", gRenderer);
	}
	~Player()	{}
	SDL_Texture* loadTexture(std::string path, SDL_Renderer* gRenderer) {
		loadedSurface = IMG_Load(path.c_str());

		if (loadedSurface == NULL) {
			printf("Unable to load image %s! SDL_image Error: %s\n", path.c_str(), IMG_GetError());
		}
		else {
			//Create texture from surface pixels
			m_texture = SDL_CreateTextureFromSurface(gRenderer, loadedSurface);

			//rec = SDL_Rect{ rec.x, rec.y, loadedSurface->w, loadedSurface->h };
			rec.w = int(loadedSurface->w);
			rec.h = int(loadedSurface->h);

			if (m_texture == NULL) {
				printf("Unable to create texture from %s! SDL Error: %s\n", path.c_str(), SDL_GetError());
			}

			//Get rid of old loaded surface
			SDL_FreeSurface(loadedSurface);

		}//must return something
		return m_texture;
	}

	void consum(SDL_Rect *r) {
		prev_rec = *r;
		if (r->x + r->w < 800 && r->x > 0)	//If your within the screen bounds 
			rec = *r;		
		else
			direction = !direction;
	}

	bool getDirection() { return direction; }
	SDL_Rect& getRect() { return rec; }
	void setRect(SDL_Rect &r)	{
		rec = r;
	}
	void render(SDL_Renderer* r) {
		SDL_RenderCopy(r, m_texture, NULL, &rec);
	}
};

#endif
