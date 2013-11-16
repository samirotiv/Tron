Tron
====

A C implementation of the infamous Tron game. 

We (Samir Otiv, Kaushik Garikipati & Ramya TR) created this to learn a few new things.

###Features
1. An implementation of the Minimax Algorithm - optimized with Alpha-Beta Pruning.
2. An intelligent heuristic - a few funtions applied onto the game's Voronoi Diagram.
3. An implementation of Dijkstra's Algorithm for square grids.
4. Highly responsive & accurate input - done by creating internal command buffers.
5. Pthreads implemented - bot created in a separate thread & given limited time, just like the human player.
6. A mechanism to dynamically adjust the Minimax depth to fit into the time window of the bot.
7. A two player game mode.
8. Terminal based colored UI using Curses.


###Installation Instructions
####To build from source
Either clone the repo, or just download the zip using the link on the right. 

If you're on a UNIX or Linux, ensure you have the developer's libraries for Ncurses installed (ncurses-dev). Then switch to the project folder and run the 'make' command.

If you're running Windows, you'd probably want to use MinGW. Ensure you install PDCurses and Pthreads. Then just run the 'make.bat' script in the project folder.



###Dependencies
1. Pthreads
2. Ncurses


