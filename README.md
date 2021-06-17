# Snake-Multithreaded

A little diffrent snake game. Goal is to get as many chest (Points) as you can. Keep an eye on the enemie snakes, they are unpredictable. 
Tipp: Don't get killed and stay inside the Walls.


## Execution
To compile use:

 `gcc -Wall -Werror -std=gnu11  -o snake snake.c helpers.c -pthread -D_GNU_SOURCE -D_DEFAULT_SOURCE -I/usr/include/ncursesw -lncursesw -ltinf`
 
You maybe need some libraries.
