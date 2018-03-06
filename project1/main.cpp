#include <thread>
#include <chrono>
#include <time.h>
#include <cstdlib>
#include <ncurses.h>
#include <string>
#include <unistd.h>
#include <vector>
#include "MyWindow.h"
#include "Mark.h"

MyWindow* win;
int squareSize;
int tableSize;

class MyThread
{
public:
	MyThread() {};
	//Function that threads will be executing
	void ThreadFunction(bool& b)
	{
		//Create new mark (bullet) from letter O with random speed
		Mark* m = new Mark('O', (-1)*(rand()%3+1), rand()%3+1, squareSize);
		//Add mark to "register"
		win->addToBuffer(m);
		int bounce = 0;
		//While bounce counter less than 3, move and draw bullet
		while(bounce < 3)
		{
			//Clear old position
			win->clearOne(m->posY, m->posX);
			//Move, if true, then increase bounce
			if(m->move())
				bounce++;
			//Update view
			win->update();
			//Slow down
			std::this_thread::sleep_for (std::chrono::milliseconds(100));
		}
		//Bounced 3 times, delete mark
		win->deleteMark(m);
		win->clearOne(m->posY, m->posX);
		//Set bool in table to true - means that new thread can replace it
		b = true;
                delete[] m;
	}
};

int main(int argc,  char** argv)
{
	squareSize = atoi(argv[1]);
	tableSize = atoi(argv[2]);
	srand(time(NULL));
    win = new MyWindow(squareSize);
	std::thread* ThreadTable = new std::thread[tableSize];
	
	//If cell contain false, thread is working, if true, thread is ready to join
	bool* statTable = new bool[tableSize];
	MyThread* mt = new MyThread();
	
	//Create starting threads
	for(int i=0; i<tableSize; i++)
	{
		statTable[i] = false;
		//ThreadTable[i] = std::thread((Fctor()), std::ref(statTable[i]), tableSize-1);
		ThreadTable[i] = std::thread(&MyThread::ThreadFunction, mt, std::ref(statTable[i]));
	}
	
	//ncurses don't block at getch, check getch every 100ms
	timeout(100);
	int c;
	//While ESC not pressed, shoot
	while((c=getch()) != 27)
	{
		//Look for a thread to replace
		for(int i=0; i<tableSize; i++)
		{
			if(statTable[i])
			{
				//Join old thread and start new one
				ThreadTable[i].join();
				statTable[i] = false;
				//ThreadTable[i] = std::thread((Fctor()), std::ref(statTable[i]), tableSize-1);
				ThreadTable[i] = std::thread(&MyThread::ThreadFunction, mt, std::ref(statTable[i]));
			}
		}
	}

	//Join all threads at the end of program
	for(int i=0; i<tableSize; i++)
	{
		ThreadTable[i].join();
	}
	//Delete allocated memory
	delete[] ThreadTable;
        delete[] statTable;
        delete mt;
    delete win;

    return 0;
}


//Inne podejście
// class Fctor
// {
// public:
	// void operator()(bool& b, int startY)
	// {
		// Mark* m = new Mark('O', (-1)*(rand()%3+1), rand()%3+1, squareSize);//(tableSize-1, 1, 'O', (-1)*(rand()%3+1), rand()%3+1, squareSize);
		// win->addToBuffer(m);
		// int bounce = 0;
		// while(bounce <= 3)
		// {
			// win->clearOne(m->posY, m->posX);
			// if(m->move())
				// bounce++;
			// win->update();
			// std::this_thread::sleep_for (std::chrono::milliseconds(100));
		// }
		// win->deleteMark(m);
		// win->clearOne(m->posY, m->posX);
		// b = true;
	// }
// };
