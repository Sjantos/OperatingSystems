#include "MyWindow.h"
#include <ncurses.h>
#include <unistd.h>
#include "Mark.h"

MyWindow::MyWindow(int h, int w) : vector(), mtx()
{
	board = new char*[h];
	for(int i=0; i<h; i++)
	{
		board[i] = new char[w];
	}
	//Init ncurses
    initscr();
    height = h;
    width = w;
	startY = height - 2;
	startX = 1;
	//Create window
    win = newwin(height, width, 1, 1);
    refresh();
	//Create box border
    box(win, 0, 0);
	refresh();
}

MyWindow::~MyWindow()
{
	for(int i=0; i<height; i++)
	{
		delete board[i];
	}
	delete[] board;
	//Close ncurses
    endwin();
}

void MyWindow::addText(Text* t)
{
	//Lock for one-thread-at-moment use
	mtx.lock();
	vector.push_back(t);
	mtx.unlock();
}

char MyWindow::get(int y, int x)
{
	return board[y][x];
}

void MyWindow::set(int y, int x, char c)
{
	//Take control
	mtx.lock();
		//Replace old position with empty space
		if(c == 'X')
			mvwaddch(win, y, x, ACS_CKBOARD);
		mvwaddch(win, y, x, c);
		wrefresh(win);
	mtx.unlock();
}

void MyWindow::update(bool menu)
{
	//Take control
	mtx.lock();
	if(menu)
	{
		//This can print some text on screen is there are some Text objects in vector
		for (auto &text : vector)
			mvprintw(text->posY, text->posX, text->txt.c_str());
	}
	wrefresh(win);
	//Draw description under box
	mvprintw(height+1, 3, "To end, press ESC and wait for all threads to join");
	box(win, 0, 0);
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

void MyWindow::writeText(std::string text, int posY, int posX)
{
	const char* c = text.c_str();
	mtx.lock();
	mvprintw(posY, posX, c);
	refresh();
	mtx.unlock();
}

void MyWindow::drawPlayer(Player* player)
{
	for(int i=0; i<player->height; i++)
	{
		set(player->posY+i, player->posX, 'X');
	}
}