#include <SDL.h>
#include <SDL_image.h>
#include "delta_r_engine.h"
#include <random>
#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>

const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 640;
const float PERSON_RADIUS = 10.f;
std::random_device gen;
std::uniform_int_distribution dis(static_cast<int>(PERSON_RADIUS), static_cast<int>(SCREEN_HEIGHT - PERSON_RADIUS));
void logSDLError(std::ostream& os, const std::string& msg) {
	os << msg << " error: " << SDL_GetError() << std::endl;
}
void set_pixel(SDL_Renderer* rend, int x, int y, Uint8 r, Uint8 g, Uint8 b, Uint8 a)
{
	SDL_Color prev_color;
	SDL_GetRenderDrawColor(rend, &prev_color.r, &prev_color.g, &prev_color.b, &prev_color.a);
	SDL_SetRenderDrawColor(rend, r, g, b, a);
	SDL_RenderDrawPoint(rend, x, y);
	SDL_SetRenderDrawColor(rend, prev_color.r, prev_color.g, prev_color.b, prev_color.a);
}
void draw_circle_border(SDL_Renderer* surface, int n_cx, int n_cy, int radius, Uint8 r, Uint8 g, Uint8 b, Uint8 a)
{
	// if the first pixel in the screen is represented by (0,0) (which is in sdl)
	// remember that the beginning of the circle is not in the middle of the pixel
	// but to the left-top from it:

	double error = (double)-radius;
	double x = (double)radius - 0.5;
	double y = (double)0.5;
	double cx = n_cx - 0.5;
	double cy = n_cy - 0.5;

	while (x >= y)
	{
		set_pixel(surface, (int)(cx + x), (int)(cy + y), r, g, b, a);
		set_pixel(surface, (int)(cx + y), (int)(cy + x), r, g, b, a);

		if (x != 0)
		{
			set_pixel(surface, (int)(cx - x), (int)(cy + y), r, g, b, a);
			set_pixel(surface, (int)(cx + y), (int)(cy - x), r, g, b, a);
		}

		if (y != 0)
		{
			set_pixel(surface, (int)(cx + x), (int)(cy - y), r, g, b, a);
			set_pixel(surface, (int)(cx - y), (int)(cy + x), r, g, b, a);
		}

		if (x != 0 && y != 0)
		{
			set_pixel(surface, (int)(cx - x), (int)(cy - y), r, g, b, a);
			set_pixel(surface, (int)(cx - y), (int)(cy - x), r, g, b, a);
		}

		error += y;
		++y;
		error += y;

		if (error >= 0)
		{
			--x;
			error -= x;
			error -= x;
		}

	}
}
void fill_circle(SDL_Renderer* surface, int cx, int cy, int radius, Uint8 r, Uint8 g, Uint8 b, Uint8 a)
{
	// Note that there is more to altering the bitrate of this 
	// method than just changing this value.  See how pixels are
	// altered at the following web page for tips:
	//   http://www.libsdl.org/intro.en/usingvideo.html
	static const int BPP = 4;

	for (double dy = 1; dy <= radius; dy += 1.0)
	{
		// This loop is unrolled a bit, only iterating through half of the
		// height of the circle.  The result is used to draw a scan line and
		// its mirror image below it.

		// The following formula has been simplified from our original.  We
		// are using half of the width of the circle because we are provided
		// with a center and we need left/right coordinates.

		double dx = floor(sqrt((2.0 * radius * dy) - (dy * dy)));
		int x = cx - dx;
		SDL_SetRenderDrawColor(surface, r, g, b, a);
		SDL_RenderDrawLine(surface, cx - dx, cy + dy - radius, cx + dx, cy + dy - radius);
		SDL_RenderDrawLine(surface, cx - dx, cy - dy + radius, cx + dx, cy - dy + radius);
	}
}

int main(int argc, char* argv[])
{
	SDL_Texture* a;
	
	if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
		logSDLError(std::cout, "SDL_Init");
		return 1;
	}
	SDL_Window* main_win = SDL_CreateWindow("Dot Test", 100, 100, SCREEN_WIDTH,SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
	if (main_win == nullptr) {
		logSDLError(std::cout, "CreateWindow");
		SDL_Quit();
		return 1;
	}
	SDL_Renderer* main_ren = SDL_CreateRenderer(main_win, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	if (main_ren == nullptr) {
		logSDLError(std::cout, "CreateRenderer");
		SDL_DestroyWindow(main_win);
		SDL_Quit();
		return 1;
	}
	IMG_Init(IMG_INIT_PNG);
	a = IMG_LoadTexture(main_ren,"res/apple.png");
	dr_object t{"obj", 64,64,a,200,200,{0,0,500,500} };
	if (t.get_img() == nullptr)
		return 1;
	for (bool quit = false; !quit;) {
		//e is an SDL_Event variable we've declared before entering the main loop
		for (SDL_Event e; SDL_PollEvent(&e);) {
			//If user closes the window
			if (e.type == SDL_QUIT) {
				quit = true;
			}

		}
		SDL_RenderClear(main_ren);
		t.SDL_RenderCopy_DEBUG(main_ren);
		SDL_SetRenderDrawColor(main_ren, 0, 255, 0, 255);
		SDL_RenderPresent(main_ren);
		t.set_x(t.get_x() + 1);
	}
	SDL_DestroyRenderer(main_ren);
	SDL_DestroyWindow(main_win);
	SDL_Quit();
	return 0;
}