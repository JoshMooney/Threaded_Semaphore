#include "Game.h"
#include <iostream>
#include <thread>
#include <string>     // std::string, std::to_string

using namespace std;

std::mutex mtx;
static SDL_mutex* mutex = SDL_CreateMutex();
SDL_Rect buffer;

Game::Game() : m_running(false){
	spawn_y = 50;
	Message_rect = SDL_Rect();
	Message_rect.x = 0;  //controls the rect's x coordinate 
	Message_rect.y = 0; // controls the rect's y coordinte
	Message_rect.w = 200; // controls the width of the rect
	Message_rect.h = 50; // controls the height of the rect

	color = { 255, 255, 0 };

	initFont();
	font = TTF_OpenFont("computer.ttf", 36);




	//threadpool = vector<std::thread>();

	/*std::thread t = std::thread(&printout, "Sup");
	t.join();*/
}

bool Game::initFont() {
	//Initialize SDL_ttf
	if (TTF_Init() == -1)	{
		return false;
	}
	
	//Set the window caption
	//SDL_WM_SetCaption("TTF Test", NULL);

	//If everything initialized fine
	return true;
}

Game::~Game()
{
}

bool Game::Initialize(const char* title, int xpos, int ypos, int width, int height, int flags)
{
	FPS_Timer = 0;
	if(SDL_Init(SDL_INIT_EVERYTHING) == 0)
	{
		DEBUG_MSG("SDL Init success");
		m_p_Window = SDL_CreateWindow(title, xpos, ypos, width, height, flags);

		if(m_p_Window != 0)
		{
			DEBUG_MSG("Window creation success");
			m_p_Renderer = SDL_CreateRenderer(m_p_Window, -1, 0);
			if(m_p_Renderer != 0)
			{
				DEBUG_MSG("Renderer creation success");
				SDL_SetRenderDrawColor(m_p_Renderer, 255, 255, 255, 255);
			}
			else
			{
				DEBUG_MSG("Renderer init fail");
				return false;
			}
		}
		else
		{
			DEBUG_MSG("Window init fail");
			return false;
		}
	}
	else
	{
		DEBUG_MSG("SDL init fail");
		return false;
	}
	m_running = true;

	//test_player = new Player(0, spawn_y, m_p_Renderer);
	spawn_y += distance;
	
	return true;
}

void Game::LoadContent()
{
	DEBUG_MSG("Loading Content");
	m_p_Surface = SDL_LoadBMP("assets/sprite.bmp");
	m_p_Texture = SDL_CreateTextureFromSurface(m_p_Renderer, m_p_Surface);
	SDL_FreeSurface(m_p_Surface);

	if(SDL_QueryTexture(m_p_Texture, NULL, NULL, &m_Source.w, &m_Destination.h)==0)
	{
		m_Destination.x = m_Source.x = 0;
		m_Destination.y = m_Source.y = 0;
		m_Destination.w = m_Source.w;
		m_Destination.h = m_Source.h;

		//DEBUG_MSG("Destination X:" + m_Destination.x);
		/*DEBUG_MSG("Destination Y:" + m_Destination.y);
		DEBUG_MSG("Destination W:" + m_Destination.w);
		DEBUG_MSG("Destination H:" + m_Destination.h);*/
	}
	else
	{
		DEBUG_MSG("Texture Query Failed");
		m_running = false;
	}
}

void Game::Render(Player *p) {
	SDL_RenderClear(m_p_Renderer);
	
	SDL_RenderCopy(m_p_Renderer, Message, NULL, &Message_rect);
	
	p->render(m_p_Renderer);
	//test_player->render(m_p_Renderer);

	SDL_RenderPresent(m_p_Renderer);
}

class PlayerPB
{
public:
	bool direction = true;
	SDL_Texture* m_texture;		//The final texture

	SDL_Surface* loadedSurface;		//Load image at specified path
	SDL_Rect rec;

	PlayerPB(float x, float y, SDL_Renderer* gRenderer) {
		rec = SDL_Rect();
		rec.x = int(x);
		rec.y = int(y);
	}
};

/*int startRunning(void* data) {
	Player *p = static_cast<Player*>(data);
	SDL_Rect temp;

	//Mutex Lock Here
	mtx.lock();
	temp = p->getRect();

	//If true move right
	if (p->getDirection()) {
		temp.x += 1;
	}
	else {
		temp.x -= 1;
	}
	buffer = temp;

	mtx.unlock();
	return 42;
}*/

void Game::Update(float FPS) {
	////Outputs the FPS the Game is currently running at.
	//FPS_Timer++;
	//if (FPS_Timer >= FPS_Timer_MAX) {
		string s_FPS = "" + std::to_string(FPS);
		surfaceMessage = TTF_RenderText_Solid(font, s_FPS.c_str(), color);
		Message = SDL_CreateTextureFromSurface(m_p_Renderer, surfaceMessage);
		//FPS_Timer = 0;
	//}
	//Bare bones of threading using SDL_Threads
	//thread = SDL_CreateThread(startRunning, "Test_Thread", test_player);
	//SDL_WaitThread(thread, NULL);
	//test_player->consum(&buffer);
	buffer = SDL_Rect();

	//std::thread t1 = std::thread(&printout, 10);		//Produce
	//t1.join();							//Wait for Thread to finish

	//Test Functions
	//makeRun(&test_player);				//Produce
	//test_player.consum(&test_buffer);		//Consum
	//test_buffer = SDL_Rect();			//Clear Buffer
	
	//Phils Code
	//int b = 10;
	//Player* pb = new Player(0,0, m_p_Renderer);
	//int* a = &b;
	//std::thread t3 = std::thread(fun, pb);
	//std::thread t3 = std::thread(fun, test_player);
	//std::thread t3 = std::thread(&Game::funct, test_player);
	//t3.join();

	//Using Threads
	//std::thread t2 = std::thread(&Game::makeRun, &test_player);		//Produce
	//test_player.consum(&test_buffer);		//Consum
	//t2.join();					//Wait for Thread to finish
	//test_buffer = SDL_Rect();			//Clear Buffer

	//Using SDL Threads
	//thread = SDL_CreateThread(&Game::makeRun, "Test_Thread", &test_player);		//Produce
	//SDL_WaitThread(thread, &threadReturnValue);		//Wait for Thread to finish
	//test_player.consum(&test_buffer);	//Consum					
	//test_buffer = SDL_Rect();			//Clear Buffer

	//Player *p = malloc(sizeof(test_player));
	//thread = SDL_CreateThread(makeRun, "Thread_Test", test_player);

	//thread = SDL_CreateThread(makeRun, "Test_Thread", (void*)NULL);
	//thread = SDL_CreateThread(&Game::makeRun, &player_container[0], "Test_Thread");
	//std::thread t1 = std::thread(count);
	//std::thread t2 = std::thread(&Game::makeRun, &player_container[0]);
	
	//Populate Thread pool
	/*
	if (player_container.size() > 0) {
		for (int i = 0; i < player_container.size(); i++) {
			threadpool.push_back(std::thread(&Game::makeRun, &player_container[i]));
		}
	}
	
	//Do other work here

	//Merge Thread pool with master thread
	if (threadpool.size() > 0) {
		for (int i = 0; i < player_container.size(); i++) {
			threadpool[i].join();
		}
	}

	//Consum Buffer

	//Clear Buffer
	threadpool.clear();
	*/
	/*
	std::thread threads[1];
	if (player_container.size() > 0) {
		threads[0] = std::thread(&Game::makeRun, &test_player);
		threads[0].join();

		//std::thread t3 = std::thread(&Game::makeRun, &player_container[0]);
		//t3.join();
	}*/
}

void Game::HandleEvents(float FPS)
{
	SDL_Event event;

	while (SDL_PollEvent(&event))
	{
		switch (event.type) {
		case SDL_KEYDOWN:
			switch (event.key.keysym.sym)
			{
			case SDLK_ESCAPE:
				m_running = false;
				break;
			case SDLK_UP:
				DEBUG_MSG("Up Key Pressed");
				SDL_SetRenderDrawColor(m_p_Renderer, 255, 0, 0, 255);
				break;
			case SDLK_DOWN:
				DEBUG_MSG("Down Key Pressed");
				SDL_SetRenderDrawColor(m_p_Renderer, 0, 255, 0, 255);
				break;
			case SDLK_LEFT:
				DEBUG_MSG("Left Key Pressed");
				SDL_SetRenderDrawColor(m_p_Renderer, 0, 0, 255, 255);
				break;
			case SDLK_RIGHT:
				DEBUG_MSG("Right Key Pressed");
				SDL_SetRenderDrawColor(m_p_Renderer, 255, 255, 255, 255);
				break;
			default:
				SDL_SetRenderDrawColor(m_p_Renderer, 0, 0, 0, 255);
				break;
			}
		case SDL_KEYUP:
			switch (event.key.keysym.sym) {
			case SDLK_SPACE:
				/*Player player = Player(0, spawn_y, m_p_Renderer);

				player_container.push_back(player);
				spawn_y += distance;
				SDL_SetRenderDrawColor(m_p_Renderer, 255, 69, 0, 255);*/
				break;
			}
		}
	}
}

bool Game::IsRunning()
{
	return m_running;
}

void Game::UnloadContent()
{
	DEBUG_MSG("Unloading Content");
	//delete(m_p_Texture);
	//m_p_Texture = NULL;
}

void Game::CleanUp()
{
	DEBUG_MSG("Cleaning Up");
	SDL_DestroyWindow(m_p_Window);
	SDL_DestroyRenderer(m_p_Renderer);
	SDL_Quit();
}

/*void Game::makeRun(void *data) {
	Player *player = static_cast<Player*>(data);
	SDL_Rect temp;

	/*Mutex Lock Here ?
	mutex_lock.lock();
	temp = player->getRect();
	//If true move right
	if (player->getDirection()) {
		temp.x += 1;
	}
	else {
		temp.x -= 1;
	}
	/*SEM: Push to bounded buffer

	/*Mutex unlock Here
	mutex_lock.unlock();
}*/

void Game::writeFPS(float FPS) {
	/*SDL_Delay(500);

	string s_FPS = "" + std::to_string(FPS);
	surfaceMessage = TTF_RenderText_Solid(font, s_FPS.c_str(), color);
	Message = SDL_CreateTextureFromSurface(m_p_Renderer, surfaceMessage);*/
}
