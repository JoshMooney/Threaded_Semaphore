#include <iostream>
#include <thread>
#include <string>

#include "TinyXML2.h"

#include <SDL.h>
#include <SDL_image.h>

#include "Game.h"
#include "Player.hpp"
#include <SDL_thread.h>

using namespace std;
using namespace tinyxml2;

#include "SDL.h"

static SDL_mutex* player_mtx = SDL_CreateMutex();
SDL_mutex* buffer_mtx = SDL_CreateMutex();

SDL_semaphore* buffer_sem = SDL_CreateSemaphore(1);

// How many frames time values to keep
// The higher the value the smoother the result is...
// Don't make it 0 or less :)
#define FRAME_VALUES 10


Uint32 frametimes[FRAME_VALUES];	// An array to store frame times:
Uint32 frametimelast;		// Last calculated SDL_GetTicks
Uint32 framecount;			// total frames rendered
float framespersecond;		// the value you want

// This function gets called once on startup.
void fpsinit() {

	// Set all frame times to 0ms.
	memset(frametimes, 0, sizeof(frametimes));
	framecount = 0;
	framespersecond = 0;
	frametimelast = SDL_GetTicks();

}
void fpsthink() {

	Uint32 frametimesindex;
	Uint32 getticks;
	Uint32 count;
	Uint32 i;

	// frametimesindex is the position in the array. It ranges from 0 to FRAME_VALUES.
	// This value rotates back to 0 after it hits FRAME_VALUES.
	frametimesindex = framecount % FRAME_VALUES;

	// store the current time
	getticks = SDL_GetTicks();

	// save the frame time value
	frametimes[frametimesindex] = getticks - frametimelast;

	// save the last frame time for the next fpsthink
	frametimelast = getticks;

	// increment the frame count
	framecount++;

	// Work out the current framerate

	// The code below could be moved into another function if you don't need the value every frame.

	// I've included a test to see if the whole array has been written to or not. This will stop
	// strange values on the first few (FRAME_VALUES) frames.
	if (framecount < FRAME_VALUES) {

		count = framecount;

	}
	else {

		count = FRAME_VALUES;

	}

	// add up all the values and divide to get the average frame time.
	framespersecond = 0;
	for (i = 0; i < count; i++) {

		framespersecond += frametimes[i];

	}

	framespersecond /= count;

	// now to make it an actual frames per second value...
	framespersecond = 1000.f / framespersecond;
}

//Simple Bounded buffer implementation
class BoundedBuffer {
public:
	int lenght = 10;			//Lenght of the buffer
	SDL_Rect buffer[10];		//The actual array to store everything put into the buffer

	int front = 0;
	int rear = 0;
	int count = 0;

	SDL_semaphore* not_full = SDL_CreateSemaphore(0);
	SDL_semaphore* not_empty = SDL_CreateSemaphore(0);

	void deposit(SDL_Rect &rec) {
		while (count == lenght - 1) {
			SDL_SemWait(not_full);
		}
		buffer[rear] = rec;
		if(rear < lenght)
			rear++;
		if (rear == lenght)
			rear = 0;
		count++;
		SDL_SemPost(not_empty);
	}
	SDL_Rect* fetch() {
		while (count == 0) {
			SDL_SemWait(not_empty);
		}
		SDL_Rect* result = &buffer[front];
		
		if (front < lenght)
			front = (front + 1) % lenght;
		if (front == lenght)
			front = 0;
		count--;
		SDL_SemPost(not_full);

		return result;
	}
};

static BoundedBuffer buffer = BoundedBuffer();

class AI {
public:	
	static int makeRun(void *data) {
		SDL_Rect temp;
		Player* player = static_cast<Player*>(data);
		int result = 0;
		while (true) {
			//Mutex Lock Here
			SDL_mutexP(player_mtx);	
			
			//Critical Section
			temp = player->getRect();
			//Do Calculations
			if (player->getDirection())
				temp.x += 1;
			else 
				temp.x -= 1;


			//Release Player Mutex here
			SDL_mutexV(player_mtx);

			//Deposit the generated position into the buffer
			buffer.deposit(temp);
			cout << "Deposit to buffer" << endl;		
		}
		return result;
	}
	static int updatePlayer(void *data) {
		int result = 0;
		Player* player = static_cast<Player*>(data);
		while (true) {
			//Mutex Lock Here
			SDL_Rect* nextPosition = buffer.fetch();
			SDL_mutexP(player_mtx);

			//Critical Section
			cout << "Consum Buffer (" << nextPosition->x << ", " << nextPosition->y << ")"<< endl;
			player->consum(nextPosition);

			//Release Players Mutex here
			SDL_mutexV(player_mtx);
		}
		return result;
	}
};

int main(int argc, char** argv){
	DEBUG_MSG("Game Object Created");
	Player *test_player;			//Create a player	

	Game* game = new Game();		//Create a game

	XMLDocument doc;				//XML for reasons
	doc.LoadFile("Levels.xml");

	const char* temp = doc.FirstChildElement("root")->FirstChildElement("level1")->FirstChildElement("player")->FirstChildElement("health")->GetText();

	DEBUG_MSG(temp);

	//Adjust screen positions as needed
	DEBUG_MSG("Game Initialising");
	game->Initialize("GE: Project 1",300,100,800,600, SDL_WINDOW_INPUT_FOCUS);
	
	DEBUG_MSG("Loading Content");
	game->LoadContent();

	test_player = new Player(0, 100, game->m_p_Renderer);
	AI ai = AI();
	SDL_Thread* sdl_ai_thread;
	sdl_ai_thread = SDL_CreateThread(&AI::makeRun, "AI Thread", test_player);
	/*
	std::vector<SDL_Thread*> sdl_ai_threads;
	for (int i = 0; i < 3; i++) {
		sdl_ai_threads.push_back(SDL_CreateThread(&AI::makeRun, "AI Thread", test_player));
	}*/

	SDL_Thread* sdl_update_thread;
	sdl_update_thread = SDL_CreateThread(&AI::updatePlayer, "Update Thread", test_player);
	/*
	BoundedBuffer bb;
	SDL_Rect* rect = bb.fetch();
	bb.deposit(SDL_Rect{ 0,1 });
	rect = bb.fetch();
	*/
	DEBUG_MSG("Game Loop Starting......");
	fpsinit();
	while(game->IsRunning())
	{
		fpsthink();
		//makeRun(test_player);

		game->HandleEvents(framespersecond);
		game->Update(framespersecond);
		game->Render(test_player);
	}

	DEBUG_MSG("Calling Cleanup");
	game->CleanUp();
	game->UnloadContent();
	
	SDL_DestroyMutex(player_mtx);
	SDL_DestroyMutex(buffer_mtx);

	return 0;
}


