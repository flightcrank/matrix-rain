
//main.c

//Using libs SDL, glibc 
#include <SDL.h>	//SDL version 2.0
#include <stdio.h>
#include <stdlib.h>
#include "font.h"

#define SCREEN_WIDTH 1290		//window height
#define SCREEN_HEIGHT 720		//window width
#define STRIPS 250				//the number of streams of text scrolling down the screen
#define SCALE 1					//the scale at which each char in the stream is rendered
#define V_GAP .8				//the vertical gap between chars in the stream

int init(int width, int height);
void vert_txt();
void check_bounds();

struct strip {

	int x;
	float y;
	float speed;
	int len;
	char str[32];
};

//function prototypes
SDL_Window* window = NULL;		//The window we'll be rendering to
SDL_Renderer *renderer;			//The renderer SDL will use to draw to the screen
SDL_Texture *font_t;			//The texture that holds the font
struct strip strips[STRIPS];		//the array that holds the strips of text and its properties

int main (int argc, char *argv[]) {
		
	//SDL Window setup
	if (init(SCREEN_WIDTH, SCREEN_HEIGHT) == 1) {
		
		return 0;
	}
	
	int sleep = 0;
	int quit = 0;
	Uint32 next_game_tick = SDL_GetTicks();
	
	//set the scale of the font
	font_set_scale(SCALE);
	font_set_v_gap(V_GAP);
	
	int i,j;
	
	//populate the strip array
	for (i = 0; i < STRIPS; i++) {
		
		int char_width = get_char_width();
		strips[i].x = (rand() % SCREEN_WIDTH / char_width) * char_width;
		strips[i].y = (strips[i].len * get_char_height()) * -1;
		strips[i].speed = (float) (rand() + RAND_MAX * .5) / RAND_MAX * 2.5;
		strips[i].len = rand() % 19 + 5;
		
		//populate str property
		for (j = 0; j < strips[i].len; j++) {
			
			int r = rand() % 1000;
			
			//random lower case
			if (r < 333) {
				
				strips[i].str[j] = rand() % 25 + 97;
			
			//random uppercase
			} else if (r > 333 && r < 666) {
				
				strips[i].str[j] = rand() % 25 + 65;
			
			} else {
			
				strips[i].str[j] = rand() % 9 + 48;
			}
		}
	}
	
	//render loop
	while(quit == 0) {
	
		//check for new events every frame
		SDL_PumpEvents();

		const Uint8 *state = SDL_GetKeyboardState(NULL);
		
		if (state[SDL_SCANCODE_ESCAPE]) {
		
			quit = 1;
		}
		
		check_bounds();

		SDL_RenderClear(renderer);
		
		int i;
		
		//draw the strips
		for (i = 0; i < STRIPS; i++) {

			//draw the strips
			vert_txt(strips[i].str, strips[i].x, (int)strips[i].y);
			strips[i].y += strips[i].speed;
		}
		
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

void vert_txt(char *str, int x, int y) {
	
	int i;
	int char_height = get_char_height();
	int length = strlen(str);
	
	SDL_SetTextureColorMod(font_t, 20, 160, 20);
	
	for (i = 0; i < length; i++) {
		
		float tail = (float) i / length;
		
		//draw the first few chars with a darker colour
		if (tail <= .4) {
			
			float grad = (float) i / (length * .4);
			
			if (i == 0 ) {
					
				grad = .15;
			}
			
			SDL_SetTextureAlphaMod(font_t, 255 * grad);
		}
		
		//draw the last element a different colour
		if (i == strlen(str) - 1) {

			SDL_SetTextureColorMod(font_t, 100, 255, 100);
		}

		print_char(str[i], x, y, renderer, font_t);
		y += char_height;
		
		
		if (rand() % 1000 < 10) {
			
			int r = rand() % 1000;
				
			//random lower case (ascii values)
			if (r < 333) {
				
				str[i] = rand() % 25 + 97;
			
			//random uppercase
			} else if (r > 333 && r < 666) {
				
				str[i] = rand() % 25 + 65;
			
			//random number
			} else {
			
				str[i] = rand() % 9 + 48;
			}
		}
	}
}

void check_bounds() {
	
	int i;
	
	for (i = 0; i < STRIPS; i++) {
		
		if (strips[i].y >= SCREEN_HEIGHT) {
			
			int char_width = get_char_width() * SCALE;
			strips[i].y = (strips[i].len * get_char_height()) * -1;
			strips[i].x = (rand() % SCREEN_WIDTH / char_width) * char_width;
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
