#include "Game.h"
#include <chrono>
#include <thread>

Game::Game(int height, int width, int playerHeight) : winMutex(), exitMutex(), playerLeftMutex(), playerRightMutex(), gameMain(), input(), playerLeft(), playerRight()/*
			: 	win(height, width),
				playerRight((height / 2) - (playerHeight / 2), width - 1, height),
				playerLeft((height / 2) - (playerHeight / 2), 1, height)*/
{
	exitPressed = false;
	boardHeight = height;
	boardWidth = width;
	win = new MyWindow(height, width);
	int y = (height / 2) - (playerHeight / 2);
	playerRight = new Player(y, width - 2, playerHeight);
	playerLeft = new Player(y, 1, playerHeight);
}

Game::~Game()
{
	/*if(input.joinable())
		input.join();
	/*if(gameMain.joinable())
		gameMain.join();
	if(playerLeftThread.joinable())
		playerLeftThread.join();
	if(playerRightThread.joinable())
		playerRightThread.join();*/
	delete win;
	//delete playerRight;
	//delete playerLeft;
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
	//timeout(10);
	//char c = getch();
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
			//case 27:
			//	exitPressed = true;
			//break;
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
	//CheckForInput();
	//PlayerLeftMove();
	//PlayerRightMove();

	winMutex.lock();
		win->drawPlayer(playerLeft);// playerLeft->DrawPlayer(win);
		win->drawPlayer(playerRight);//playerRight->DrawPlayer(win);
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
		//playerLeftSpeed = 0;
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
		//playerRightSpeed = 0;
		playerRightMutex.unlock();
		inputMutex.unlock();
		std::this_thread::sleep_for (std::chrono::milliseconds(250));
	}
	inputMutex.unlock();
}

bool Game::ExitPressed()
{
	return exitPressed;
}