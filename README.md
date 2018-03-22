# matrix-rain
This is a simple attempt at creating a matrix rain effect seen in the movie the matrix

There are 2 versions of the effect

Both can scale the text to any size by adjusting the SCALE constant, in addition to rendering in any resolution.
This program is dependent on the SDL 2 library installed on your system and can run on any operating system
with little to no modification. 

Compilation instructions on a Linux system are as follows..

main.c version

    gcc main.c font.c `sdl2-config --cflags --libs` -o matrix-rain-1
    
main2.c version

    gcc main2.c font.c `sdl2-config --cflags --libs` -o matrix-rain-2

## Images 
main.c
![animation1](https://i.imgur.com/eymQH3q.gif)

main2.c
![animation2](https://i.imgur.com/t5GJReh.gif)
