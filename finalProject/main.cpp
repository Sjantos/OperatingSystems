#include <thread>
#include <ncurses.h>
#include <mutex>
#include <chrono>
#include <ctime>
#include <condition_variable>


#define height 20
#define width 40
#define mstime 100
#define ballSpawnTime 500
#define threads 10

std::mutex m;
std::mutex balls;
std::mutex ballsLoc;
std::mutex inputMutex;
std::mutex pauseMutex;
std::condition_variable cvar;

int ballsCount, ballsOver;
int ballsData[threads][4];
int ballsLocations[height][width];
char inputValue = 0;
bool escPressed = false;
int playerLeftSpeed = 0;
int playerRightSpeed = 0;
int leftPlayerScore = 0;
int rightPlayerScore = 0;
bool running = true;

void InputThread()
{
	std::unique_lock<std::mutex> locker(m, std::defer_lock);
	std::unique_lock<std::mutex> locker2(inputMutex, std::defer_lock);
	while(running)
	{
		locker.lock();
		inputValue = getch();
		locker.unlock();
		switch(inputValue)
		{
			case 'a':
				playerLeftSpeed = -1;
			break;
			case 'z':
				playerLeftSpeed = 1;
			break;
			case 'k':
				playerRightSpeed = -1;
			break;
			case 'm':
				playerRightSpeed = 1;
			break;
			case 27:
				if (escPressed)
				{
					cvar.notify_all();
					cvar.notify_all();
					escPressed = false;
				}
				else escPressed = true;
			break;
			case 'q':
				running = false;
			break;

		}
		//locker2.unlock();
		std::this_thread::sleep_for (std::chrono::milliseconds(10));
	}
	
}

int FindBall(int y, int x)
{
	for (int i = 0; i < threads; i++)
		if (ballsData[i][2] == y && ballsData[i][3] == x)
			return i;
	return -1;
}

void DetectCollision(int y, int x, int y2, int x2)
{
	int index = 0;
	int index2 = 0;
	int yDirectionBuf, xDirectionBuf;

	if(ballsLocations[y2][x2] == 0) return;
	else if (ballsLocations[y2][x2] == 2)
	{
		index = FindBall(y, x);
		ballsData[index][0] *= -1;
	}
	else if (ballsLocations[y2][x2] == 3) 
	{
		index = FindBall(y, x);
		ballsData[index][2] = -5;
		ballsData[index][3] = -5;
		rightPlayerScore++;
	}
	else if (ballsLocations[y2][x2] == 4)
	{
		index = FindBall(y, x);
		ballsData[index][2] = -5;
		ballsData[index][3] = -5;
		leftPlayerScore++;
	}
	else if (ballsLocations[y2][x2] == 5)
	{
		index = FindBall(y, x);
		ballsData[index][1] *= -1;
	}
	else
	{
		index = FindBall(y, x);
		index2 = FindBall(y2, x2);


		yDirectionBuf = ballsData[index][0];
		xDirectionBuf = ballsData[index][1];

		ballsData[index][0] = ballsData[index2][0];
		ballsData[index][1] = ballsData[index2][1];

		ballsData[index2][0] = yDirectionBuf;
		ballsData[index2][1] = xDirectionBuf;
		
	}
}

void barRight() {
	int highy = 10;
	int lowy = 4;

	std::unique_lock<std::mutex> pauseLocker(pauseMutex, std::defer_lock);
	std::unique_lock<std::mutex> locker(m, std::defer_lock);
	std::unique_lock<std::mutex> locker3(ballsLoc, std::defer_lock);
	locker.lock();
	locker3.lock();
	for (int i = lowy; i <= highy; i++) {
		mvprintw(i, width - 4, "#");
		ballsLocations[i][width - 4] = 5;

	}

	refresh();
	locker3.unlock();
	locker.unlock();

	while (running)
	{
		if (escPressed)
			cvar.wait(pauseLocker, []() { return !escPressed; });

		std::this_thread::sleep_for(std::chrono::milliseconds(mstime));
		if (highy == height - 2 && playerRightSpeed != -1) {
			playerRightSpeed = 0;
		}
		if (lowy == 1 && playerRightSpeed != 1) {
			playerRightSpeed = 0;
		}

		lowy += 1 * playerRightSpeed;
		highy += 1 * playerRightSpeed;

		locker.lock();
		locker3.lock();
		for (int i = lowy; i <= highy; i++) {
			mvprintw(i, width - 4, "#");
			ballsLocations[i][width - 4] = 5;
		}

		if (playerRightSpeed == 1) {
			mvprintw(lowy - 1, width - 4, " ");
			ballsLocations[lowy - 1][width - 4] = 0;
		}
		else if (playerRightSpeed == -1)
		{
			mvprintw(highy + 1, width - 4, " ");
			ballsLocations[highy + 1][width - 4] = 0;
		}
		refresh();
		locker3.unlock();
		locker.unlock();

	}
}

void barLeft() {
	int highy = 10;
	int lowy = 4;

	std::unique_lock<std::mutex> pauseLocker(pauseMutex, std::defer_lock);
	std::unique_lock<std::mutex> locker(m, std::defer_lock);
	std::unique_lock<std::mutex> locker3(ballsLoc, std::defer_lock);
	locker.lock();
	locker3.lock();
	for (int i = lowy; i <= highy; i++) {
		mvprintw(i, 3, "#");
		ballsLocations[i][3] = 5;
	}
	refresh();
	locker3.unlock();
	locker.unlock();


	while (running)
	{
		if (escPressed)
			cvar.wait(pauseLocker, []() { return !escPressed; });

		std::this_thread::sleep_for(std::chrono::milliseconds(mstime));
		if (highy == height - 2 && playerLeftSpeed != -1) {
			playerLeftSpeed = 0;
		}
		if (lowy == 1 && playerLeftSpeed != 1) {
			playerLeftSpeed = 0;
		}

		lowy += 1 * playerLeftSpeed;
		highy += 1 * playerLeftSpeed;

		locker.lock();
		locker3.lock();
		for (int i = lowy; i <= highy; i++) {
			mvprintw(i, 3, "#");
			ballsLocations[i][3] = 5;
		}
		if (playerLeftSpeed == 1) {
			mvprintw(lowy - 1, 3, " ");
			ballsLocations[lowy - 1][3] = 0;
		}
		else if (playerLeftSpeed == -1)
		{
			mvprintw(highy + 1, 3, " ");
			ballsLocations[highy + 1][3] = 0;
		}
		refresh();
		locker3.unlock();
		locker.unlock();

	}
}

void ball1(int threadID){
int prevy, prevx, i = 0, y, x, nextx, nexty;

std::unique_lock<std::mutex> pauseLocker(pauseMutex, std::defer_lock);
std::unique_lock<std::mutex> locker(m, std::defer_lock);
std::unique_lock<std::mutex> locker2(balls, std::defer_lock);
std::unique_lock<std::mutex> locker3(ballsLoc, std::defer_lock);


while(running){
	if (escPressed)
		cvar.wait(pauseLocker, []() { return !escPressed; });

	std::this_thread::sleep_for(std::chrono::milliseconds(mstime));
		locker.lock();
		locker2.lock();	
		locker3.lock();

		prevy = ballsData[threadID][2];
		prevx = ballsData[threadID][3];

		nexty = ballsData[threadID][2] + 1*ballsData[threadID][0];
		nextx = ballsData[threadID][3] + 1*ballsData[threadID][1];

		DetectCollision(prevy, prevx, nexty, nextx);
		ballsData[threadID][2] += 1*ballsData[threadID][0];
		ballsData[threadID][3] += 1*ballsData[threadID][1];
		y = ballsData[threadID][2];
		x = ballsData[threadID][3];
		ballsLocations[y][x] = 1;
		ballsLocations[prevy][prevx] = 0;

		if (ballsData[threadID][2] <= 0 || ballsData[threadID][2] >= height - 1
			|| ballsData[threadID][3] <= 0 || ballsData[threadID][3] >= width - 1) {
			mvprintw(prevy, prevx, " ");
			locker.unlock();
			locker2.unlock();
			locker3.unlock();
			return;
		}
		mvprintw(ballsData[threadID][2], ballsData[threadID][3], "1");

		mvprintw(prevy, prevx, " ");

		refresh();

		locker3.unlock();
		locker2.unlock();
		locker.unlock();


	}

}

void ball2(int threadID){
int y, x, ydir = -1, xdir = 1, prevy, prevx, i = 0, nextx, nexty;
std::unique_lock<std::mutex> pauseLocker(pauseMutex, std::defer_lock);
std::unique_lock<std::mutex> locker(m, std::defer_lock);
std::unique_lock<std::mutex> locker2(balls, std::defer_lock);
std::unique_lock<std::mutex> locker3(ballsLoc, std::defer_lock);

while(running){
	if (escPressed)
		cvar.wait(pauseLocker, []() { return !escPressed; });

	std::this_thread::sleep_for(std::chrono::milliseconds(mstime));
		locker.lock();
		locker2.lock();	
		locker3.lock();


		prevy = ballsData[threadID][2];
		prevx = ballsData[threadID][3];

		nexty = ballsData[threadID][2] + 1*ballsData[threadID][0];
		nextx = ballsData[threadID][3];

		DetectCollision(prevy, prevx, nexty, nextx);
		ballsData[threadID][2] += 1*ballsData[threadID][0];
		y = ballsData[threadID][2];
		ballsLocations[prevy][prevx] = 0;
		ballsLocations[y][ballsData[threadID][3]] = 1;

		if (ballsData[threadID][2] <= 0 || ballsData[threadID][2] >= height - 1
			|| ballsData[threadID][3] <= 0 || ballsData[threadID][3] >= width - 1) {
			mvprintw(prevy, prevx, " ");
			locker.unlock();
			locker2.unlock();
			locker3.unlock();
			return;
		}
		mvprintw(ballsData[threadID][2], ballsData[threadID][3], "2");

		mvprintw(prevy, prevx, " ");

		refresh();

		locker3.unlock();
		locker2.unlock();
		locker.unlock();

		std::this_thread::sleep_for(std::chrono::milliseconds(mstime));

		locker.lock();
		locker2.lock();
		locker3.lock();

		prevy = ballsData[threadID][2];
		prevx = ballsData[threadID][3];

		nexty = ballsData[threadID][2] + 1*ballsData[threadID][0];
		nextx = ballsData[threadID][3] + 1*ballsData[threadID][1];
	

		DetectCollision(prevy, prevx, nexty, nextx);
		ballsData[threadID][2] += 1*ballsData[threadID][0];
		ballsData[threadID][3] += 1*ballsData[threadID][1];
		y = ballsData[threadID][2];
		x = ballsData[threadID][3];
		ballsLocations[prevy][prevx] = 0;
		ballsLocations[y][x] = 1;

		if (ballsData[threadID][2] <= 0 || ballsData[threadID][2] >= height - 1
			|| ballsData[threadID][3] <= 0 || ballsData[threadID][3] >= width - 1) {
			mvprintw(prevy, prevx, " ");
			locker.unlock();
			locker2.unlock();
			locker3.unlock();
			return;
		}
		mvprintw(ballsData[threadID][2], ballsData[threadID][3], "2");

		mvprintw(prevy, prevx, " ");

		refresh();

		locker3.unlock();
		locker2.unlock();
		locker.unlock();




	}



}

void ball3(int threadID){
int y, x, ydir = -1, xdir = 1, prevy, prevx, i = 0, nexty, nextx;
std::unique_lock<std::mutex> pauseLocker(pauseMutex, std::defer_lock);
std::unique_lock<std::mutex> locker(m, std::defer_lock);
std::unique_lock<std::mutex> locker2(balls, std::defer_lock);
std::unique_lock<std::mutex> locker3(ballsLoc, std::defer_lock);

while(running){
	if (escPressed)
		cvar.wait(pauseLocker, []() { return !escPressed; });

	std::this_thread::sleep_for(std::chrono::milliseconds(mstime));
	locker.lock();
	locker2.lock();	
	locker3.lock();


	prevy = ballsData[threadID][2];
	prevx = ballsData[threadID][3];

	nexty = ballsData[threadID][2];
	nextx = ballsData[threadID][3] + 1*ballsData[threadID][1];


	DetectCollision(prevy, prevx, nexty, nextx);
	ballsData[threadID][3] += 1*ballsData[threadID][1];
	x = ballsData[threadID][3];
	ballsLocations[prevy][prevx] = 0;
	ballsLocations[ballsData[threadID][2]][x] = 1;

	if (ballsData[threadID][2] <= 0 || ballsData[threadID][2] >= height - 1
		|| ballsData[threadID][3] <= 0 || ballsData[threadID][3] >= width - 1) {
		mvprintw(prevy, prevx, " ");
		locker.unlock();
		locker2.unlock();
		locker3.unlock();
		return;
	}
	mvprintw(ballsData[threadID][2], ballsData[threadID][3], "3");

	mvprintw(prevy, prevx, " ");

	refresh();

	locker3.unlock();
	locker2.unlock();
	locker.unlock();

	std::this_thread::sleep_for(std::chrono::milliseconds(mstime));

	locker.lock();
	locker2.lock();	
	locker3.lock();


	prevy = ballsData[threadID][2];
	prevx = ballsData[threadID][3];

	nexty = ballsData[threadID][2] + 1*ballsData[threadID][0];
	nextx = ballsData[threadID][3] + 1*ballsData[threadID][1];


	DetectCollision(prevy, prevx, nexty, nextx);
	ballsData[threadID][2] += 1*ballsData[threadID][0];
	ballsData[threadID][3] += 1*ballsData[threadID][1];
	y = ballsData[threadID][2];
	x = ballsData[threadID][3];
	ballsLocations[prevy][prevx] = 0;
	ballsLocations[y][x] = 1;

	if (ballsData[threadID][2] <= 0 || ballsData[threadID][2] >= height - 1
		|| ballsData[threadID][3] <= 0 || ballsData[threadID][3] >= width - 1) {
		mvprintw(prevy, prevx, " ");
		locker.unlock();
		locker2.unlock();
		locker3.unlock();
		return;
	}
	mvprintw(ballsData[threadID][2], ballsData[threadID][3], "3");

	mvprintw(prevy, prevx, " ");

	refresh();

	locker3.unlock();
	locker2.unlock();	
	locker.unlock();




	}



}

int main(){

std::unique_lock<std::mutex> locker2(balls, std::defer_lock);
std::unique_lock<std::mutex> locker3(ballsLoc, std::defer_lock);

	for(int i = 0; i < 10; i++)
		for(int j = 0; j < 4; j++)
			ballsData[i][j] = 1;

	for (int i = 0; i < height; i++)
		for (int j = 0; j < width; j++)
		{
			if (i == 0)
				ballsLocations[i][j] = 2;
			else if (i == height - 1)
				ballsLocations[i][j] = 2;
			else if (j == 0) 
				ballsLocations[i][j] = 3;
			else if (j == width - 1)
				ballsLocations[i][j] = 4;
			else ballsLocations[i][j] = 0;
		}

	initscr();
	int i = 0, j = 0;
	std::unique_lock<std::mutex> locker(balls, std::defer_lock);
	std::unique_lock<std::mutex> pauseLocker(pauseMutex, std::defer_lock);
	curs_set(0);
	timeout(10);
	noecho();

	ballsCount = 0; ballsOver = 0;

	WINDOW * win = newwin(height, width, 0, 0);
	refresh();

	box(win, 0, 0);
	wrefresh(win);

	std::srand(std::time(NULL));
	int random;	
	int random2;

	std::thread threadArr[threads];
	std::thread barL = std::thread(barLeft);
	std::thread barR = std::thread(barRight);
	std::thread input = std::thread(InputThread);
	for(int i = 0; i < threads; i++){
	if (escPressed)
		cvar.wait(pauseLocker, []() { return !escPressed; });
	std::this_thread::sleep_for(std::chrono::milliseconds(mstime));
	random = std::rand();
	
	if(random <= (RAND_MAX/3))
	{
		random2 = std::rand();
		locker2.lock();
		if (random2 < RAND_MAX / 2)
		{
			ballsData[i][0] = -1;
			ballsData[i][1] = 1;
		}
		else 
		{
			ballsData[i][0] = -1;
			ballsData[i][1] = -1;
		}
		ballsData[i][2] = height - 3;
		ballsData[i][3] = int(width/2);
		locker3.lock();
		ballsLocations[ballsData[i][2]][ballsData[i][3]] = 1;
		locker3.unlock();
		locker2.unlock();
		threadArr[i] = std::thread(ball1,i);
	}
	else if(random > ((RAND_MAX/3)*2))
	{
		random2 = std::rand();
		locker2.lock();
		if (random2 < RAND_MAX / 2)
		{
			ballsData[i][0] = -1;
			ballsData[i][1] = 1;
		}
		else
		{
			ballsData[i][0] = -1;
			ballsData[i][1] = -1;
		}
		ballsData[i][2] = height - 3;
		ballsData[i][3] = int(width / 2) + 1;
		locker3.lock();
		ballsLocations[ballsData[i][2]][ballsData[i][3]] = 1;
		locker3.unlock();
		locker2.unlock();
		threadArr[i] = std::thread(ball2,i);
	}
	else 
	{
		random2 = std::rand();
		locker2.lock();
		if (random2 < RAND_MAX / 2)
		{
			ballsData[i][0] = -1;
			ballsData[i][1] = 1;
		}
		else
		{
			ballsData[i][0] = -1;
			ballsData[i][1] = -1;
		}
		ballsData[i][2] = height - 2;
		ballsData[i][3] = int(width / 2) - 1;
		locker3.lock();
		ballsLocations[ballsData[i][2]][ballsData[i][3]] = 1;
		locker3.unlock();
		locker2.unlock();
		threadArr[i] = std::thread(ball3,i);
	}

	if (!running) break;

	std::this_thread::sleep_for(std::chrono::milliseconds(ballSpawnTime));

	}
	
	/*//Wykorzystanie głównego wątku programu jako wątku inputu
	m.lock();
	timeout(10);
	m.unlock();
	inputMutex.lock();
	while(!escPressed)
	{
		m.lock();
		inputValue = getch();
		m.unlock();
		switch(inputValue)
		{
			/*case 'a':
				playerLeftSpeed = -1;
			break;
			case 'z':
				playerLeftSpeed = 1;
			break;
			case 'k':
				playerRightSpeed = -1;
			break;
			case 'm':
				playerRightSpeed = 1;
			break;
			case 27:
				escPressed = true;
			break;
		}
		inputMutex.unlock();
		std::this_thread::sleep_for (std::chrono::milliseconds(10));
	}
	
	inputMutex.unlock();*/

	for(int i = 0; i < threads; i++){
		if (threadArr[i].joinable())
			threadArr[i].join();
	}
	running = false;
	if (barL.joinable())
		barL.join();
	if (barL.joinable())
		barR.join();
	if (input.joinable())
		input.join();
	if (leftPlayerScore > rightPlayerScore)
		mvprintw(height + 3, 3, "Player left wins!");
	else if (rightPlayerScore > leftPlayerScore)
		mvprintw(height + 3, 3, "Player right wins!");
	else 
		mvprintw(height + 3, 3, "Draw");

	getch();
	endwin();


}
