#define SUPRA_IMPLEMENTATION
#include "supra.h"

int main() {
  int quit = 0;

	if(InitSupra("Hello!", 1000, 1000) != 0) {
	  return 1;
	};
  
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
