#ifndef MyWindow_h
#define MyWindow_h
#include <ncurses.h>
#include <vector>
#include <mutex>
#include <condition_variable>
#include "Mark.h"
#include <string>
#include "Player.h"

class MyWindow
{
public:
	MyWindow(){};
    MyWindow(int h, int w);
    ~MyWindow();
    void addText(Text* m);
    void update(bool menu);
	char get(int y, int x);
	void set(int y, int x, char c);
	void deleteMark(Mark* mark);
	int marksInHalf();
	int numberOfMarks();
	void writeText(std::string text, int posY, int posX);
	void drawPlayer(Player* player);

private:
	int startX, startY;
    int height, width;
	char** board;
    WINDOW * win;
	std::vector<Text*> vector;
	mutable std::mutex mtx;
};
#endif
