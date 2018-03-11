#ifndef Mark_h
#define Mark_h

class Mark
{
	int xMax, yMax;
	int xMove, yMove;
    int bounceCount;
public:
	
    Mark(char c, int yMov, int xMov, int yM);

	//posXY will be set in MyWindow
    int posX, posY;
    char mark;
    bool move();
};
#endif
