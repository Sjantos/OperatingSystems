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

MyWindow* win;
int squareSize;
std::mutex globalMutex;
std::condition_variable cv;
bool exitPressed;
char c = 0;


int main(int argc,  char** argv)
{
	//default values
	int height = 32, width = 64, playerHeight = 8;
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
	}
	
	Game* g = new Game(height, width, playerHeight);
	
	//g->FireThreads();
	//std::thread inputThread(&Game::CheckForInput, g);
	//g->GameLoop();
	std::thread input = g->FireInputThread();
	std::thread playerLeft = g->FirePlayerLeftThread();
	std::thread playerRight = g->FirePlayerRightThread();
	//std::thread gameLoop = g-> FireGameLoopThread();
	
	//char c = 0;
	timeout(10);
	bool whileCondition = true;
	while(whileCondition)//g->ExitPressed())
	{
		g->inputMutex.lock();
		g->inputValue = getch();
		whileCondition = g->inputValue == 27 ? false : true;
		g->inputMutex.unlock();
		std::this_thread::sleep_for (std::chrono::milliseconds(10));
		//g->CheckForInput();
		g->GameLoop();
		//win->update();
	}
	
	playerLeft.join();
	playerRight.join();
	input.join();
	//gameLoop.join();
	delete g;
}

/*
int main(int argc,  char** argv)
{
	//INITIALIZE
		//Input arguments
		squareSize = atoi(argv[1]);
		int width = atoi(argv[2]);
		//int threadDelay = atoi(argv[3]);
		
		srand(time(NULL));
		win = new MyWindow(squareSize, width);
		//MyThread* mt = new MyThread();
		//std::map<int, std::thread> threadMap;
		exitPressed = false;
		win->writeText("O Y-1 na 1", squareSize-1, 1);
		win->writeText("Y-1 na X-1 O", squareSize-1, width-12);
		win->writeText("O 1 na 1", 1, 1);
		win->writeText("1 na X-1 O", 1, width-10);
	
	//MAIN PROGRAM
		//ncurses don't block at getch, check getch every 100ms
		timeout(100);
		int c, counter=0;
		//While ESC not pressed, shoot
		while(!exitPressed)
		{
			if((c=getch()) == 27)
				exitPressed = true;
			win->set(counter % squareSize, counter % squareSize, 'O');
			win->update();
			counter++;
			//threadMap[counter]=std::thread(&MyThread::ThreadFunction, mt, threadDelay);
			////New mark in bottom half, notify other threads
			//cv.notify_all();
			//std::this_thread::sleep_for (std::chrono::milliseconds(shootDelay));
			//counter++;
		}
	
	//CLEANING
		//Join all threads at the end of program
		//for(unsigned int i=0; i<threadMap.size(); i++)
		//	threadMap[i].join();
		
		//Delete allocated memory
		delete win;

    return 0;
}*/