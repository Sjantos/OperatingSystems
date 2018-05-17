#include "Game.h"
#include <chrono>
#include <thread>

Game::Game(int height, int width, int playerHeight, int playersRefreshInterval) : playerLeft(), playerRight()
{
	boardHeight = height;
	boardWidth = width;
	win = new MyWindow(height, width);
	int y = (height / 2) - (playerHeight / 2);
	playerRight = new Player(y, width - 2, playerHeight);
	playerLeft = new Player(y, 1, playerHeight);
	playersThreadSleepTime = playersRefreshInterval;
}

Game::~Game()
{
	delete win;
	delete playerLeft;
	delete playerRight;
}

std::thread Game::FireInputThread()
{
	return std::thread([=] { CheckForInput(); });
}
std::thread Game::FireGameLoopThread()
{
	return std::thread([=] { GameLoop(); });
}
std::thread Game::FirePlayerLeftThread()
{
	return std::thread([=] { PlayerLeftMove(); });
}
std::thread Game::FirePlayerRightThread()
{
	return std::thread([=] { PlayerRightMove(); });
}

void Game::CheckForInput()
{
	inputMutex.lock();
	while(inputValue != 27)
	{
		playerLeftMutex.lock();
		playerRightMutex.lock();
		switch(inputValue)
		{
			case 'a':
				playerLeftSpeed = -1;
			break;
			case 'z':
				playerLeftSpeed = 1;
			break;
			case 'k':
				playerRightSpeed = -1;
			break;
			case 'm':
				playerRightSpeed = 1;
			break;
		}
		playerLeftMutex.unlock();
		playerRightMutex.unlock();
		inputMutex.unlock();
		std::this_thread::sleep_for (std::chrono::milliseconds(10));
	}
	
	inputMutex.unlock();
}

void Game::GameLoop()
{
	winMutex.lock();
		win->drawPlayer(playerLeft);
		win->drawPlayer(playerRight);
		win->set(counter, counter, 'O');
		win->update();
	winMutex.unlock();
	counter++;
}

void Game::PlayerLeftMove()
{
	inputMutex.lock();
	while(inputValue != 27)
	{
		playerLeftMutex.lock();
		if(playerLeftSpeed < 0 && playerLeft->posY > 0)
		{
			winMutex.lock();
			win->set(playerLeft->posY+playerLeft->height - 1, playerLeft->posX, ' ');
			winMutex.unlock();
			playerLeft->posY--;
		} else if(playerLeftSpeed > 0 && (playerLeft->posY + playerLeft->height) < boardHeight)
		{
			winMutex.lock();
			win->set(playerLeft->posY, playerLeft->posX, ' ');
			winMutex.unlock();
			playerLeft->posY++;
		}
		playerLeftMutex.unlock();
		inputMutex.unlock();
		std::this_thread::sleep_for (std::chrono::milliseconds(250));
	}
	inputMutex.unlock();
}

void Game::PlayerRightMove()
{
	inputMutex.lock();
	while(inputValue != 27)
	{
		playerRightMutex.lock();
		if(playerRightSpeed < 0 && playerRight->posY > 0)
		{
			winMutex.lock();
			win->set(playerRight->posY+playerRight->height - 1, playerRight->posX, ' ');
			winMutex.unlock();
			playerRight->posY--;
		} else if(playerRightSpeed > 0 && (playerRight->posY + playerRight->height) < boardHeight)
		{
			winMutex.lock();
			win->set(playerRight->posY, playerRight->posX, ' ');
			winMutex.unlock();
			playerRight->posY++;
		}
		playerRightMutex.unlock();
		inputMutex.unlock();
		std::this_thread::sleep_for (std::chrono::milliseconds(250));
	}
	inputMutex.unlock();
}