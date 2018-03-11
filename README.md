# OperatingSystems
Repository for college course
Threads in Linux system written in C++ 11, pthreads and ncurses.

## Project 1
Shoot bullets from the bottom left-hand side of window in randomly direction. Bounce bullets of the walls 3 times.
After 3 times, bullet disappear. Then shoot new one. Every bullet is a thread. Threads must be created, safely deleted and managed.
Program shoot bullets all time, user should have and option to quit the program.

To run project, open terminal in project1 directory, type **make**, then **./project1 arg1 arg2** where arg1 is box (square) height (I suggest 32 for this value) and arg2 is number of bullets at the same time.

## Project 2
Modify Project 1 to stop all the bullets(threads), when they are in upper half of the box. Activate them, when a bullet show in bottom half (when new is created and shooted).
Additionally, projects needed rework, because bullets should be spawned by time interval, and in first project there was constant number of bullets, so all of them could get stuck in upper half.

To run project, open terminal in project2 directory, type **make**, then **./project2 arg1 arg2 arg3** where arg1 is box (square) height (I suggest 32 for this value), arg2 is interval between new bullets (in milliseconds) and arg3 is threads delay in moving bullets (in milliseconds).