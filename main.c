#define SUPRA_IMPLEMENTATION
#include "supra.h"

int main() {
  int quit = 0;

	if (!SDL_Init(SDL_INIT_VIDEO | SDL_INIT_GAMEPAD))
	{
		SDL_Log("Failed to initialize SDL: %s", SDL_GetError());
		return 1;
	}

	InitSupra("Hello!", 1000, 1000);
  
  while(!quit) {
    SDL_Event evt;
    while(SDL_PollEvent(&evt)) {
			if (evt.type == SDL_EVENT_QUIT)
			{
			  Quit();
			  quit = 1;
			}
    } 
    if(quit) break;
    Draw();
  }
  return 1;
}
