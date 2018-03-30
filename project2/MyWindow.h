#ifndef MyWindow_h
#define MyWindow_h
#include <ncurses.h>
#include <vector>
#include <mutex>
#include <condition_variable>
#include "Mark.h"

class MyWindow
{
public:
    MyWindow(int h);
    ~MyWindow();
    void addToBuffer(Mark* m);
    void update();
	void clearOne(int y, int x);
	void deleteMark(Mark* mark);
	int marksInHalf();
	int numberOfMarks();

private:
	int startX, startY;
    int height, width;
    WINDOW * win;
	std::vector<Mark*> vector;
	mutable std::mutex mtx;
	std::condition_variable c;
};
#endif
