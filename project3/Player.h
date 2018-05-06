#include "MyWindow.h"

#ifndef Player_h
#define Player_h

class Player
{
public:
	int height;
    int posX, posY;
	const char block = (char)0x2588;
	
	Player(){};
    Player(int y, int x, int h);
	//void DrawPlayer(MyWindow* win);
};
#endif