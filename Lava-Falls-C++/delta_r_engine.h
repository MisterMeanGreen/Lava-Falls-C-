#ifndef DELTA_R_ENGINE_H
#define DELTA_R_ENGINE_H
#include <SDL.h>
#include <SDL_ttf.h>
#include <string>
class dr_object { //basic object, hold position, rect, and image
		//Naming Behavior
		//curr <-- Stands for current
		//Use for all variables defined within the class
		//Use simplest name for all parameters
		float curr_vel; //Current Velocity
		float curr_dir; //Current Direction
		float curr_dir_vel; //Current Direction Velocity (For smooth direction turning)

		SDL_Texture* curr_img; //Stores Image Picture
		SDL_Rect curr_render_img_rect; //Stores info about where to render curr_img and the resulting size (X pos, Y pos, X Scale, Y Scale)
		SDL_Rect curr_src_img_rect; //Stores what part of curr_img to render

		std::string curr_name; //Object name to be referenced if needed
public:
	//Constructors
		dr_object(std::string name,int x, int y, SDL_Texture* img, int size_x, int size_y, float vel, float dir, float dir_vel,SDL_Rect src);
		dr_object(std::string name, int x, int y, SDL_Texture* img, int size_x, int size_y, SDL_Rect src);
		dr_object(std::string name, int x, int y, SDL_Texture* img, int size_x, int size_y);
		dr_object(std::string name, int x, int y, SDL_Texture* img, SDL_Rect src);
		dr_object(std::string name, int x, int y, SDL_Texture* img);
		dr_object(std::string name, int x, int y);
		dr_object(std::string name);

	//Deconstructor
		~dr_object();

	//Get
		float get_y_vel(); //gets y velocity
		float get_x_vel(); //gets x velocity
		float get_vel(); //gets velocity
		float get_dir(); //gets direction
		float get_dir_vel(); //gets directional velocity (aka turning speed)
		SDL_Texture* get_img(); //gets the image on the object
		SDL_Rect get_render_rect(); //gets the render rectangle
		int get_x(); //gets x-position of the object (Top Left)
		int get_y(); //gets y-position of the object (Top Left)
		int get_size_x(); //gets the rendered width of the object
		int get_size_y(); //gets the rendered height of the object
		SDL_Rect get_src_rect();  //gets the source rectangle from which curr_img is rendering from
		std::string get_name(); //gets the name

	//Set
		float set_y_vel(float y_vel); //sets y velocity
		float set_x_vel(float x_vel); //sets x velocity
		float set_vel(float vel); //sets velocity
		float set_dir(float dir); //sets direction
		float set_dir_vel(float dir_vel);
		SDL_Texture* set_img(SDL_Texture* img);
		int set_x(int x);
		int set_y(int y);
		SDL_Rect set_src_rect(SDL_Rect src);
		std::string set_name(std::string name);

	//Rendering
		void SDL_RenderCopy(SDL_Renderer* renderer, SDL_Rect* src); //Render the curr_render_img to the screen with optional src parameters
		void SDL_RenderCopy(SDL_Renderer* renderer); //Render the curr_render_img to the screen with curr_src_img as the parameter
		void SDL_RenderCopy_DEBUG(SDL_Renderer* renderer);
		void SDL_RenderCopy_DEBUG(SDL_Renderer* renderer, SDL_Rect* src);

	};
class dr_sdl_text {
	SDL_Texture* tex;
	SDL_Texture* Text_To_Texture(SDL_Renderer* renderer, std::string text, TTF_Font* font, SDL_Color color) {
		SDL_Surface* surface = TTF_RenderText_Solid(font, text.c_str(), color);
		SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
		SDL_FreeSurface(surface);
		return texture;
	}
	
public:
	dr_sdl_text(SDL_Renderer* renderer, std::string text, TTF_Font* font, SDL_Color color);
	~dr_sdl_text();
	SDL_Texture* operator()();
	SDL_Texture* operator()(SDL_Renderer* renderer, std::string text, TTF_Font* font, SDL_Color color);
};
#endif