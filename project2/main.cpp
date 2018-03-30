#include <thread>
#include <chrono>
#include <mutex>
#include <condition_variable>
#include <functional>
#include <time.h>
#include <cstdlib>
#include <ncurses.h>
#include <string>
#include <unistd.h>
#include <map>
#include "MyWindow.h"
#include "Mark.h"

MyWindow* win;
int squareSize;
std::mutex globalMutex;
std::condition_variable cv;
bool exitPressed;

bool markInBottomHalf()
{
	return win->numberOfMarks() != win->marksInHalf();
}

class MyThread
{
public:
	MyThread() {};
	//Function that threads will be executing
	void ThreadFunction(int delay)
	{
		//Create new mark (bullet) from letter O with random speed
		Mark* m = new Mark('O', (-1)*(rand()%3+1), rand()%3+1, squareSize);
		//Add mark to "register"
		win->addToBuffer(m);
		int bounce = 0;
		//While bounce counter less than 3, move and draw bullet
		while(bounce < 3)
		{
			win->update();
			//If ESC was pressed, do not wait
			if(!exitPressed)
			{
				std::unique_lock<std::mutex> lock(globalMutex);
				//wait if marksInHalf return true (all marks in upper half)
				//cv.wait(lock, std::bind(&MyWindow::marksInHalf, win));
				cv.wait(lock, markInBottomHalf);
				cv.notify_all();
			}
			//Clear old position
			win->clearOne(m->posY, m->posX);
			//Move, if true, then increase bounce
			if(m->move())
				bounce++;
			//Update view
			win->update();
			//Slow down
			std::this_thread::sleep_for (std::chrono::milliseconds(delay));
		}
		//Bounced 3 times, delete mark
		win->deleteMark(m);
		win->clearOne(m->posY, m->posX);
        delete[] m;
	}
};

int main(int argc,  char** argv)
{
	//INITIALIZE
		//Input arguments
		squareSize = atoi(argv[1]);
		int shootDelay = atoi(argv[2]);
		int threadDelay = atoi(argv[3]);
		
		srand(time(NULL));
		win = new MyWindow(squareSize);
		MyThread* mt = new MyThread();
		std::map<int, std::thread> threadMap;
		exitPressed = false;
	
	//MAIN PROGRAM
		//ncurses don't block at getch, check getch every 100ms
		timeout(100);
		int c, counter=0;
		//While ESC not pressed, shoot
		while(!exitPressed)
		{
			if((c=getch()) == 27)
				exitPressed = true;
			threadMap[counter]=std::thread(&MyThread::ThreadFunction, mt, threadDelay);
			//New mark in bottom half, notify other threads
			cv.notify_all();
			std::this_thread::sleep_for (std::chrono::milliseconds(shootDelay));
			counter++;
		}
	
	//CLEANING
		//Join all threads at the end of program
		for(unsigned int i=0; i<threadMap.size(); i++)
			threadMap[i].join();
		
		//Delete allocated memory
		delete win;

    return 0;
}