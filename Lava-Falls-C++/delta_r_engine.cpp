#include "delta_r_engine.h"
#include <SDL.h>
#include <SDL_image.h>
#include <cmath>
#include <exception>
dr_object::dr_object(std::string name, int x, int y, SDL_Texture* img, int size_x, int size_y, float vel, float dir, float dir_vel,SDL_Rect src) {
	curr_name = name;
	curr_src_img_rect = src;
	curr_vel = vel;
	curr_dir = dir;
	curr_dir_vel = dir_vel;
	curr_img = img;
	curr_render_img_rect = { x,y,size_x,size_y };
}

dr_object::dr_object(std::string name, int x, int y, SDL_Texture* img, int size_x, int size_y, SDL_Rect src) : dr_object(name,x, y, img, size_x, size_y, 0.f, 0.f, 0.f, src)
{
}

dr_object::dr_object(std::string name, int x, int y, SDL_Texture* img, int size_x, int size_y) : dr_object(name, x, y, img, size_x, size_y, 0.f, 0.f, 0.f, { 0,0,0,0 }) {
	Uint32 throwaway_format;
	int throw_away_access;
	if (SDL_QueryTexture(img, &throwaway_format, &throw_away_access, &curr_src_img_rect.w, &curr_src_img_rect.h) ==  -1)
		throw std::exception(std::string("Object src_img_rect SDL_QueryTexture Error on object of name : " + name).c_str());
}

dr_object::dr_object(std::string name, int x, int y, SDL_Texture* img, SDL_Rect src) : dr_object(name,x, y, img, 0, 0, 0.f, 0.f, 0.f, src) {
	Uint32 throwaway_format;
	int throw_away_access;
	if (SDL_QueryTexture(img, &throwaway_format, &throw_away_access, &curr_render_img_rect.w, &curr_render_img_rect.h) == -1)
		throw std::exception(std::string("Object render_img_rect SDL_QueryTexture Error on object of name : " + name).c_str());
	if (SDL_QueryTexture(img, &throwaway_format, &throw_away_access, &curr_src_img_rect.w, &curr_src_img_rect.h) == -1)
		throw std::exception(std::string("Object src_img_rect SDL_QueryTexture Error on object of name : " + name).c_str());

}

dr_object::dr_object(std::string name, int x, int y, SDL_Texture* img) : dr_object(name,x, y, img, 0, 0, 0.f, 0.f, 0.f, {0,0,0,0})
{
	Uint32 throwaway_format;
	int throw_away_access;
	if (SDL_QueryTexture(img, &throwaway_format, &throw_away_access, &curr_render_img_rect.w, &curr_render_img_rect.h) == -1)
		throw std::exception(std::string("Object render_img_rect SDL_QueryTexture Error on object of name : " + name).c_str());
	if (SDL_QueryTexture(img, &throwaway_format, &throw_away_access, &curr_src_img_rect.w, &curr_src_img_rect.h) == -1)
		throw std::exception(std::string("Object src_img_rect SDL_QueryTexture Error on object of name : " + name).c_str());
}

dr_object::dr_object(std::string name, int x, int y) : dr_object(name,x, y, nullptr, 0, 0, 0.f, 0.f, 0.f, { 0,0,0,0 }) {}

dr_object::dr_object(std::string name) : dr_object(name,0, 0, nullptr, 0, 0.f, 0.f, 0.f, 0.f, { 0,0,0,0 }) {}

dr_object::~dr_object() {}

float dr_object::get_y_vel()
{
	return sin(curr_dir) * curr_vel;
}
float dr_object::get_x_vel()
{
	return cos(curr_dir) * curr_vel;
}
float dr_object::get_vel()
{
	return curr_vel;
}
float dr_object::get_dir()
{
	return curr_dir;
}
float dr_object::get_dir_vel()
{
	return curr_dir_vel;
}
SDL_Texture* dr_object::get_img()
{
	return curr_img;
}
SDL_Rect dr_object::get_render_rect()
{
	return curr_render_img_rect;
}
int dr_object::get_x()
{
	return curr_render_img_rect.x;
}
int dr_object::get_y()
{
	return curr_render_img_rect.y;
}
int dr_object::get_size_x()
{
	return curr_render_img_rect.w;
}
int dr_object::get_size_y()
{
	return curr_render_img_rect.h;
}
SDL_Rect dr_object::get_src_rect()
{
	return curr_src_img_rect;
}
std::string dr_object::get_name()
{
	return curr_name;
}
float dr_object::set_y_vel(float y_vel)
{
	float old_x_vel = get_x_vel();
	curr_vel = sqrt(pow(old_x_vel, 2) + pow(y_vel, 2));
	curr_dir = atan2(y_vel, old_x_vel);
	return y_vel;
}

float dr_object::set_x_vel(float x_vel)
{
	float old_y_vel = get_y_vel();
	curr_vel = sqrt(pow(old_y_vel, 2) + pow(x_vel, 2));
	curr_dir = atan2(old_y_vel, x_vel);
	return x_vel;
}

float dr_object::set_vel(float vel)
{
	return curr_vel = vel;
}

float dr_object::set_dir(float dir)
{
	return curr_dir = dir;
}

float dr_object::set_dir_vel(float dir_vel)
{
	return curr_dir_vel = dir_vel;
}

SDL_Texture* dr_object::set_img(SDL_Texture* img)
{
	return curr_img = img;
}

int dr_object::set_x(int x)
{
	return curr_render_img_rect.x = x;
}

int dr_object::set_y(int y)
{
	return curr_render_img_rect.y = y;
}

SDL_Rect dr_object::set_src_rect(SDL_Rect src)
{
	return curr_src_img_rect = src;
}

std::string dr_object::set_name(std::string name)
{
	return curr_name = name;
}


void dr_object::SDL_RenderCopy(SDL_Renderer* renderer, SDL_Rect* src)
{
	if(::SDL_RenderCopy(renderer, curr_img, src, &curr_render_img_rect) == -1)
		throw std::exception(std::string("Object SDL_RenderCopy Error on object of name : " + curr_name).c_str());
}

void dr_object::SDL_RenderCopy(SDL_Renderer* renderer)
{
	SDL_RenderCopy(renderer, &curr_src_img_rect);
}

void dr_object::SDL_RenderCopy_DEBUG(SDL_Renderer* renderer)
{
	if (SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255) == -1)
		throw std::exception(std::string("Object SDL_SetRenderDrawColor Error on object of name : " + curr_name).c_str());
	SDL_RenderCopy(renderer, &curr_src_img_rect);
	if (SDL_RenderDrawRect(renderer, &curr_render_img_rect) == -1)
		throw std::exception(std::string("Object SDL_SetRenderDrawColor Error on object of name : " + curr_name).c_str());
}

void dr_object::SDL_RenderCopy_DEBUG(SDL_Renderer* renderer, SDL_Rect* src)
{
	if (SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255) == -1)
		throw std::exception(std::string("Object SDL_SetRenderDrawColor Error on object of name : " + curr_name).c_str());
	SDL_RenderCopy(renderer, src);
	if (SDL_RenderDrawRect(renderer, &curr_render_img_rect) == -1)
		throw std::exception(std::string("Object SDL_SetRenderDrawColor Error on object of name : " + curr_name).c_str());
}


dr_sdl_text::dr_sdl_text(SDL_Renderer* renderer, std::string text, TTF_Font* font, SDL_Color color) {
	tex = Text_To_Texture(renderer, text, font, color);
}

dr_sdl_text::~dr_sdl_text()
{
	SDL_DestroyTexture(tex);
}

SDL_Texture* dr_sdl_text::operator()()
{
	return tex;
}

SDL_Texture* dr_sdl_text::operator()(SDL_Renderer* renderer,std::string text,TTF_Font* font, SDL_Color color)
{
	SDL_DestroyTexture(tex);
	return 	tex = Text_To_Texture(renderer, text, font, color);
}
