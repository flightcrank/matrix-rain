# matrix-rain
This is a simple attempt at creating a matrix rain effect seen in the movie the matrix

It can scale the text to any size by adjusting the SCALE constant, in addition to rendering in any resolution.
This program is dependent on the SDL 2 library installed on your system and can run on any operating system
with little to no modification. 

Compilation instructions are as follows

    gcc main.c font.c `sdl2-config --cflags --libs` -o matrix-rain
	
## Images 
![animation](https://i.imgur.com/lgszAqv.gif)
