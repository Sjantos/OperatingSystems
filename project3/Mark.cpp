#include "Mark.h"

Mark::Mark(char c, int yMov, int xMov, int yM)
{
	//posXY will be set in MyWindow
	posX = 0; posX = 0;
	mark = c;
	xMove = xMov;
	yMove = yMov;
	bounceCount = 0;
	yMax = yM;
	xMax = 2*yM;
}

bool Mark::move()
{
	bool returnValue = false;
	if(posX+xMove <= 0 || posX+xMove >= xMax-1)
	{
		xMove *= -1;
		returnValue = true;
	}
	if(posY+yMove <= 0 || posY+yMove >= yMax-1)
	{
		yMove *= -1;
		returnValue = true;
	}

	posX += xMove;
	posY += yMove;
	return returnValue;
}

Text::Text(int y, int x, std::string txt)
{
	this->posY = y;
	this->posX = x;
	this->txt = txt;
}