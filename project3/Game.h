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
	int counter;
	int boardHeight, boardWidth;
	
	bool exitPressed = false;
	int playerLeftSpeed = 0;
	int playerRightSpeed = 0;
	
	std::thread gameMain;
	std::thread input;
	std::thread playerLeftThread;
	std::thread playerRightThread;
	
	std::mutex playerLeftMutex;
	std::mutex playerRightMutex;
	std::mutex exitMutex;
	std::mutex winMutex;
	
public:
	Game(int height, int width, int playerHeight);
	~Game();
	std::thread FireInputThread();
	std::thread FireGameLoopThread();
	std::thread FirePlayerLeftThread();
	std::thread FirePlayerRightThread();
	void CheckForInput();
	void GameLoop();
	void PlayerLeftMove();
	void PlayerRightMove();
	bool ExitPressed();
	
	std::mutex inputMutex;
	char inputValue = 0;
};

#endif