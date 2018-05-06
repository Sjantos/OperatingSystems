#include "Player.h"

Player::Player(int y, int x, int h)
{
	height = h;
	posY = y;
	posX = x;
}

/*void Player::DrawPlayer(MyWindow* win)
{
	for(int i=0; i<height; i++)
	{
		win->set(posY+i, posX, 'X');
	}
}*/