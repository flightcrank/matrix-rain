
//font.h

#include <SDL.h>

SDL_Texture * load_font(char *file, SDL_Renderer *rend);

int get_char_width();

int get_char_height();

void font_set_scale(float s);

void print_str(char *str, int x, int y, SDL_Renderer *rend, SDL_Texture *tex);

void print_char(char c, int x, int y, SDL_Renderer *rend, SDL_Texture *tex);
