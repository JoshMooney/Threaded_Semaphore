#ifndef GAME_H
#define GAME_H
#include <SDL.h>
#include "Debug.h"
#include "SDL_ttf.h"
using namespace std;
#include <vector>
//#include "Player.hpp"
#include "SDL_thread.h"
#include <thread>
#include <mutex>
#include "SDL_image.h"
#include "Player.hpp"

class Game
{
public:
	Game();
	~Game();
	bool Initialize(const char*, int, int, int, int, int);
	void LoadContent();
	void UnloadContent();
	void Render(Player *p);
	void Update(float FPS);
	void HandleEvents(float FPS);
	bool IsRunning();
	void CleanUp();
	bool initFont();

	//void makeRun(Player *player);
	//void makeRun(void *data);
	void writeFPS(float FPS);

	//Buffers
	//static SDL_Rect test_buffer;

	SDL_Thread *thread;
	int    threadReturnValue;

	vector<std::thread> threadpool;
	std::mutex mutex_lock;
	//SDL_mutex mutex_sdl;

	//void funct(Player* pb);
	int FPS_Timer;
	const int FPS_Timer_MAX = 500;
	SDL_Renderer* m_p_Renderer;
private:
	//vector<Player> player_container;
	float spawn_y;
	float distance = 75;
	bool m_running;
	SDL_Window* m_p_Window;

	SDL_Texture* m_p_Texture;
	SDL_Rect m_Source;
	SDL_Rect m_Destination;
	SDL_Surface* m_p_Surface;

	SDL_Rect Message_rect; //create a rect
	TTF_Font* font;
	SDL_Color color;  // this is the color in rgb format, maxing out all would give you the color white, and it will be your text's color
	SDL_Surface* surfaceMessage; // as TTF_RenderText_Solid could only be used on SDL_Surface then you have to create the surface first
	SDL_Texture* Message;
};
#endif

