/*
  Supra!

  A simple abstraction on top of the SDL3 GPU API.

  Depends on:
  - SDL3

*/

#ifndef SUPRA
#define SUPRA

#include <SDL3/SDL.h>
#include <SDL3/SDL_gpu.h>

#include "stdlib.h"

typedef struct SupraContext
{
	const char* BasePath;
	SDL_Window* Window;
	SDL_GPUDevice* Device;
	SDL_GPUGraphicsPipeline* Pipeline2D;
	float DeltaTime;
} SupraContext;

int InitSupra(char* windowName, int w, int h);

int Draw();
void Quit();

SDL_GPUShader* LoadShader(
	SDL_GPUDevice* device,
	const char* shaderFilename,
	Uint32 samplerCount,
	Uint32 uniformBufferCount,
	Uint32 storageBufferCount,
	Uint32 storageTextureCount
);

extern SupraContext* supra_gsc;

#endif
