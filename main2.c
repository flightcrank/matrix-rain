//main.c c99 standard

//Using libs SDL, glibc
#include <SDL.h>	//SDL version 2.0
#include <stdio.h>
#include <stdlib.h>
#include "font.h"

#define SCREEN_WIDTH 1290		//window height
#define SCREEN_HEIGHT 720		//window width
#define NUM_STREAM 150			//number of chars streaming down
#define SCALE .9					//scale at which the chars are drawn
#define H_GAP .9				//gap between the chars horizontally
#define V_GAP .7				//gap between the chars vertically

//function prototypes
int init(int w, int h);
void fade(int col, int row, unsigned char a[col][row]);

//structure to define the stream of chars cascading down the window
struct stream {
	
	float x;
	float y;
	float speed;
};

//globals
SDL_Window* window = NULL;		//The window we'll be rendering to
SDL_Renderer *renderer;			//The renderer SDL will use to draw to the screen
SDL_Texture *font_t;			//The texture that holds the font
struct stream streams[NUM_STREAM];

int main (int argc, char *argv[]) {
		
	//SDL Window setup
	if (init(SCREEN_WIDTH, SCREEN_HEIGHT) == 1) {
		
		return 0;
	}
	
	//font settings
	font_set_scale(SCALE);
	font_set_v_gap(V_GAP);
	font_set_h_gap(H_GAP);
	
	//calculate the dimensions of the  two arrays that will hold the chars and their alpha values respectively 
	int cols = SCREEN_WIDTH / get_char_width();
	int rows = SCREEN_HEIGHT / get_char_height();		
	
	char char_grid[cols][rows];
	unsigned char alpha_grid[cols][rows];
	
	int x,y;
	
	//populate grid of random chars and set all alpha values to 0
	for (x = 0; x < cols; x++) {

		for (y = 0; y < rows; y++) {
		 
			int r = rand() % 1000;
			
			//char_grid values random lower case
			if (r < 333) {
				
				char_grid[x][y] = rand() % 25 + 97;
			
			//random uppercase
			} else if (r > 333 && r < 666) {
				
				char_grid[x][y] = rand() % 25 + 65;
			
			} else {
			
				char_grid[x][y] = rand() % 9 + 48;
			}
			
			//alpha values
			alpha_grid[x][y] = 0;
		}
	}
	
	//populate streams
	for (x = 0; x < NUM_STREAM; x++) {
	
		streams[x].x = rand() % cols;
		streams[x].y = 0;
		streams[x].speed = (float) (rand() % 150) / 1000 + .05;
	}
	
	int sleep = 0;
	int quit = 0;
	Uint32 next_game_tick = SDL_GetTicks();
	
	//render loop
	while(quit == 0) {
	
		//check for new events every frame
		SDL_PumpEvents();

		const Uint8 *state = SDL_GetKeyboardState(NULL);
		
		if (state[SDL_SCANCODE_ESCAPE]) {
		
			quit = 1;
		}
		
		//draw background
		SDL_RenderClear(renderer);
		
		int x,y;
		int px = 0;
		int py = 0;
		
		//print all chars to  the screen
		for (x = 0; x < cols; x++) {
			
			for (y = 0; y < rows; y++) {
				
				unsigned char val = alpha_grid[x][y];
				
				if (val > 250) {
					
					SDL_SetTextureColorMod(font_t, 100, 255, 100);
					
				} else {
					
					SDL_SetTextureColorMod(font_t, 20, 160, 20);
				}
				
				SDL_SetTextureAlphaMod(font_t, val);
				print_char(char_grid[x][y], px, py, renderer, font_t);
				py += get_char_height();
			}
			
			py = 0;
			px += get_char_width();
		}
		
		int i;
		
		//render the streams
		for (i = 0; i < NUM_STREAM; i++) {
			
			if (streams[i].y > rows) {
				
				streams[i].y = 0;
				streams[i].x = rand() % cols;
			}
			
			alpha_grid[(int)streams[i].x][(int)streams[i].y] = 255;
			streams[i].y += streams[i].speed;
		}
		
		fade(cols, rows, alpha_grid);
		
		//draw to the screen
		SDL_RenderPresent(renderer);
				
		//time it takes to render 1 frame in milliseconds
		next_game_tick += 1000 / 60;
		sleep = next_game_tick - SDL_GetTicks();
	
		if( sleep >= 0 ) {
            				
			SDL_Delay(sleep);
		}
	}

	//free renderer and all textures used with it
	SDL_DestroyRenderer(renderer);
	
	//Destroy window 
	SDL_DestroyWindow(window);

	//Quit SDL subsystems 
	SDL_Quit(); 
	 
	return 0;
}


//fade out the the alpha_grid values by 1;
void fade(int col, int row, unsigned char a[col][row]) {
	
	int x,y;
	
	for (x =0; x < col; x++) {
		
		for (y =0; y < row; y++) {
				
			if (a[x][y] > 0) {
				
				a[x][y] -= 1;
			}
		}
	}
}

int init(int width, int height) {

	//Initialize SDL
	if (SDL_Init(SDL_INIT_VIDEO) < 0) {

		printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
		
		return 1;
	} 
	
	//Create window	
	SDL_CreateWindowAndRenderer(SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN, &window, &renderer);
	
	//set up font texture
	font_t = load_font("mtx.bmp",renderer);
	
	if (window == NULL) { 
		
		printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
		
		return 1;
	}

	if (font_t == NULL) { 
		
		printf("Font texture could not be created!\n");
		
		return 1;
	}
	
	return 0;
}
