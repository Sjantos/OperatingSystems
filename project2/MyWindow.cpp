#include "MyWindow.h"
#include <ncurses.h>
#include <unistd.h>
#include "Mark.h"

MyWindow::MyWindow(int h) : vector(), mtx(), c()
{
	//Init ncurses
    initscr();
    height = h;
    width = 2*h;
	startY = height - 2;
	startX = 1;
	//Create window
    win = newwin(height, width, 1, 1);
    refresh();
	//Create box border
    box(win, 0, 0);
}

MyWindow::~MyWindow()
{
	//Close ncurses
    endwin();
}

void MyWindow::addToBuffer(Mark* m)
{
	//Lock for one-thread-at-moment use
	std::lock_guard<std::mutex> lock(this->mtx);
	//Set starting position
	m->posY = startY;
	m->posX = startX;
	//Add mark to "register"
	vector.push_back(m);
	//Free lock
	c.notify_one();
}

void MyWindow::clearOne(int y, int x)
{
	//Take control
	mtx.lock();
		//Replace old position with empty space
		mvwaddch(win, y, x, ' ');
		wrefresh(win);
	mtx.unlock();
}

void MyWindow::update()
{
	//Take control
	mtx.lock();
	//Draw all marks from "register"
	for (auto &mark : vector)
		mvwaddch(win, mark->posY, mark->posX, mark->mark);
	wrefresh(win);
	//Draw description under box
	mvprintw(height+1, 3, "To end, press ESC and wait for all threads to bounce 3 times");
	refresh();	
	
	mtx.unlock();
}

int MyWindow::numberOfMarks()
{
	return vector.size();
}

int MyWindow::marksInHalf()
{
	//Check if all marks are in upper half of box
	int count = 0;
	for(auto &mark : vector)
		if(mark->posY < height/2)
			count++;
	return count;
}

void MyWindow::deleteMark(Mark* mark)
{
	mtx.lock();
	unsigned int i=0;
	//Find mark in "register""
	for(i=0; i<vector.size(); i++)
	{
		if(vector[i] == mark)
			break;
	}
	//Remove it
	vector.erase(vector.begin() + i);
	mtx.unlock();
}