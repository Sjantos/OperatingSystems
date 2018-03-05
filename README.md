# OperatingSystems
Repository for college course
Threads in Linux system written in C++ 11, pthreads and ncurses.

## Project 1
Shoot bullets from the bottom left-hand side of window in randomly direction. Bounce bullets of the walls 3 times.
After 3 times, bullet disappear. Then shoot new one. Every bullet is a thread. Threads must be created, safely deleted and managed.
Program shoot bullets all time, user should have and option to quit the program.

To run project, open terminal in project1 directory, type **make**, then **./project1 arg1 arg2** where arg1 is box (square) height (I suggest 32 for this value) and arg2 is number of bullets at the same time.
