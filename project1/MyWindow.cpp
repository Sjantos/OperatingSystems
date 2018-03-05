#include "MyWindow.h"
#include <ncurses.h>
#include <unistd.h>
#include "Mark.h"

MyWindow::MyWindow(int h) : vector(), m(), c()
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
	std::lock_guard<std::mutex> lock(this->m);
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
	m.lock();
		//Replace old position with empty space
		mvwaddch(win, y, x, ' ');
		wrefresh(win);
	m.unlock();
}

void MyWindow::update()
{
	//Take control
	m.lock();
	//Draw all marks from "register"
	for (auto &mark : vector)
	{
		mvwaddch(win, mark->posY, mark->posX, mark->mark);
		wrefresh(win);
	}
	//Draw description under box
	mvprintw(height+1, 3, "To end, press ESC and wait for all threads to bounce 3 times");
	refresh();
	m.unlock();
}

void MyWindow::deleteMark(Mark* mark)
{
	m.lock();
	unsigned int i=0;
	//Find mark in "register""
	for(i=0; i<vector.size(); i++)
	{
		if(vector[i] == mark)
			break;
	}
	//Remove it
	vector.erase(vector.begin() + i);
	m.unlock();
}