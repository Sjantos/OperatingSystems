#include "Game.h"
#include <chrono>
#include <thread>

Game::Game(int height, int width, int playerHeight) : exitMutex(), playerLeftMutex(), playerRightMutex(), gameMain(), input(), playerLeft(), playerRight()/*
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
	//gameMain = std::thread(&Game::GameLoop, this);
	//input = std::thread(&Game::CheckForInput, this);
	//playerLeftThread = std::thread(&Game::PlayerLeftMove, this);
	//playerRightThread = std::thread(&Game::PlayerRightMove, this);
}

std::thread Game::FireGameLoopThread()
{
	return std::thread([=] { GameLoop(); });
}

void Game::CheckForInput()
{
		timeout(10);
		char c = getch();
		playerLeftMutex.lock();
		playerRightMutex.lock();
		exitMutex.lock();
		switch(c)
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
			case 27:
				exitPressed = true;
			break;
		}
		playerLeftMutex.unlock();
		playerRightMutex.unlock();
		exitMutex.unlock();
}

void Game::GameLoop()
{
	while(!exitPressed)
	{
		playerLeftMutex.lock();
		playerRightMutex.lock();
	
		CheckForInput();
		PlayerLeftMove();
		PlayerRightMove();

		playerLeftMutex.lock();
		win->drawPlayer(playerLeft);// playerLeft->DrawPlayer(win);
		playerLeftMutex.unlock();
		playerRightMutex.lock();
		win->drawPlayer(playerRight);//playerRight->DrawPlayer(win);
		playerRightMutex.unlock();
		win->set(counter, counter, 'O');
		win->update();
		counter++;
	
		playerLeftMutex.unlock();
		playerRightMutex.unlock();
	}
	
}

void Game::PlayerLeftMove()
{
		playerLeftMutex.lock();
		if(playerLeftSpeed < 0 && playerLeft->posY > 0)
		{
			win->set(playerLeft->posY+playerLeft->height - 1, playerLeft->posX, ' ');
			playerLeft->posY--;
		} else if(playerLeftSpeed > 0 && (playerLeft->posY + playerLeft->height) < boardHeight)
		{
			win->set(playerLeft->posY, playerLeft->posX, ' ');
			playerLeft->posY++;
		}
		playerLeftSpeed = 0;
		playerLeftMutex.unlock();
}

void Game::PlayerRightMove()
{
		playerRightMutex.lock();
		if(playerRightSpeed < 0 && playerRight->posY > 0)
		{
			win->set(playerRight->posY+playerRight->height - 1, playerRight->posX, ' ');
			playerRight->posY--;
		} else if(playerRightSpeed > 0 && (playerRight->posY + playerRight->height) < boardHeight)
		{
			win->set(playerRight->posY, playerRight->posX, ' ');
			playerRight->posY++;
		}
		playerRightSpeed = 0;
		playerRightMutex.unlock();
}

bool Game::ExitPressed()
{
	return exitPressed;
}