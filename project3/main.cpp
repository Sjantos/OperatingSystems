#include <thread>
#include <chrono>
#include <mutex>
#include <condition_variable>
#include <functional>
#include <time.h>
#include <cstdlib>
#include <ncurses.h>
#include <string>
#include <unistd.h>
#include <map>
#include "MyWindow.h"
#include "Mark.h"
#include "Game.h"

int main(int argc,  char** argv)
{
	//default values
	int height = 32, width = 64, playerHeight = 8, playersSleepTime = 250;
	switch(argc)
	{
		case 2:
		height = atoi(argv[1]);
		break;
		case 3:
		height = atoi(argv[1]);
		width = atoi(argv[2]);
		break;
		case 4:
		height = atoi(argv[1]);
		width = atoi(argv[2]);
		playerHeight = atoi(argv[3]);
		break;
		case 5:
		height = atoi(argv[1]);
		width = atoi(argv[2]);
		playerHeight = atoi(argv[3]);
		playersSleepTime = atoi(argv[4]);
		break;
	}
	
	Game* g = new Game(height, width, playerHeight, playersSleepTime);

	std::thread input = g->FireInputThread();
	std::thread playerLeft = g->FirePlayerLeftThread();
	std::thread playerRight = g->FirePlayerRightThread();
	
	timeout(10);
	bool whileCondition = true;
	while(whileCondition)
	{
		g->inputMutex.lock();
		g->inputValue = getch();
		whileCondition = g->inputValue == 27 ? false : true;
		g->inputMutex.unlock();
		std::this_thread::sleep_for (std::chrono::milliseconds(10));
		g->GameLoop();
	}
	
	playerLeft.join();
	playerRight.join();
	input.join();
	delete g;
}