#include "MyWindow.h"
#include "Player.h"
#include <ncurses.h>
#include <thread>
#include <mutex>

#ifndef Game_h
#define Game_h

class Game
{
	MyWindow* win;
	Player* playerLeft;
	Player* playerRight;
	int boardHeight, boardWidth;
	
	int playerLeftSpeed = 0;
	int playerRightSpeed = 0;
	int playersThreadSleepTime=250;
	
	std::mutex playerLeftMutex;
	std::mutex playerRightMutex;
	std::mutex winMutex;
	
	void CheckForInput();
	void PlayerLeftMove();
	void PlayerRightMove();
public:
	Game(int height, int width, int playerHeight, int playersRefreshInterval);
	~Game();
	std::thread FireInputThread();
	std::thread FireGameLoopThread();
	std::thread FirePlayerLeftThread();
	std::thread FirePlayerRightThread();
	//GameLoop is used in "main" thread
	void GameLoop();
	
	std::mutex inputMutex;
	char inputValue = 0;
};

#endif