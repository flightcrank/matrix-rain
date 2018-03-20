
//font.c

#include <SDL.h>
#include "font.h"

//The width and height of each char in the font image
#define CHAR_WIDTH 16
#define CHAR_HEIGHT 25

float scale = 1;

SDL_Texture * load_font(char *file, SDL_Renderer *rend) {

	SDL_Surface *temp = SDL_LoadBMP(file);
	SDL_Texture *tex = SDL_CreateTextureFromSurface(rend, temp);
	SDL_FreeSurface(temp);
	
	if (temp == NULL) {
	
		printf("Could not load BMP file %s:%s", file, SDL_GetError());
		return NULL;
	}
	
	if (tex == NULL) {
	
		printf("Could not create font texture :%s", SDL_GetError());
		return NULL;
	}
	
	return tex;
}

int get_char_width() {
	
	return CHAR_WIDTH;	
}

int get_char_height() {

	return CHAR_HEIGHT;
}

void font_set_scale(float s){
	
	scale = s;	
}

void print_str(char *str, int x, int y, SDL_Renderer *rend, SDL_Texture *tex) {
	
	int i;
	
	for(i = 0; i < strlen(str); i++) {
		
		print_char(str[i], x, y, rend, tex);
		x += CHAR_WIDTH * scale;
	}
}

//assumes the bitmap'd font is all on the same y axis (all on the same line)
void print_char(char c, int x, int y, SDL_Renderer *rend, SDL_Texture *tex) {
	
	//string representation of the layout of the bitmap font
	char *cmap = " abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789!@#$%^&*()[];':\",./\\?";
	
	int i;
	
	SDL_Rect src;
	src.w = CHAR_WIDTH; //each char is 17px wide
	src.h = CHAR_HEIGHT; //each char is 24 pix tall
	src.x = 0;
	src.y = 0;
	
	for (i = 0; i < strlen(cmap); i++) {
	
		if (c == cmap[i]) {
		
			break;
		}
		
		src.x += src.w;
	}

	SDL_Rect dest;
	dest.w = src.w * scale;
	dest.h = src.h * scale;
	dest.x = x;
	dest.y = y;

	SDL_RenderCopy(rend, tex, &src, &dest);
}
