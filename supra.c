#include "supra.h"

SupraContext* supra_gsc;

int InitSupra(char* windowName, int w, int h) {
	if (!SDL_Init(SDL_INIT_VIDEO | SDL_INIT_GAMEPAD))
	{
		SDL_Log("Failed to initialize SDL: %s", SDL_GetError());
		return -1;
	}
  supra_gsc = malloc(sizeof(SupraContext));
  supra_gsc->BasePath = SDL_GetBasePath();
	supra_gsc->Device = SDL_CreateGPUDevice(
		SDL_GPU_SHADERFORMAT_SPIRV | SDL_GPU_SHADERFORMAT_DXIL | SDL_GPU_SHADERFORMAT_MSL,
		false,
		NULL);

	if (supra_gsc->Device == NULL) {
		SDL_Log("GPUCreateDevice failed");
		return -1;
	}
	supra_gsc->Window = SDL_CreateWindow(windowName, w, h, 0);
	if (supra_gsc->Window == NULL) {
		SDL_Log("CreateWindow failed: %s", SDL_GetError());
		return -1;
	}
	if (!SDL_ClaimWindowForGPUDevice(supra_gsc->Device, supra_gsc->Window)) {
		SDL_Log("GPUClaimWindow failed");
		return -1;
	}
	SDL_GPUShader* vertexShader = LoadShader(supra_gsc->Device, "tri.vert", 0, 0, 0, 0);
	if (vertexShader == NULL) {
		SDL_Log("Failed to create vertex shader!");
		return -1;
	}
	SDL_GPUShader* fragmentShader = LoadShader(supra_gsc->Device, "solid.frag", 0, 0, 0, 0);
	if (fragmentShader == NULL) {
		SDL_Log("Failed to create fragment shader!");
		return -1;
	}

	SDL_GPUGraphicsPipelineCreateInfo pipelineCreateInfo = {
		.target_info = {
			.num_color_targets = 1,
			.color_target_descriptions = (SDL_GPUColorTargetDescription[]){{
				.format = SDL_GetGPUSwapchainTextureFormat(supra_gsc->Device, supra_gsc->Window)
			}},
		},
		.primitive_type = SDL_GPU_PRIMITIVETYPE_TRIANGLELIST,
		.vertex_shader = vertexShader,
		.fragment_shader = fragmentShader,
	};

	pipelineCreateInfo.rasterizer_state.fill_mode = SDL_GPU_FILLMODE_FILL;
	supra_gsc->Pipeline2D = SDL_CreateGPUGraphicsPipeline(supra_gsc->Device, &pipelineCreateInfo);
	if (supra_gsc->Pipeline2D == NULL) {
		SDL_Log("Failed to create 2D pipeline!");
		return -1;
	}

	SDL_ReleaseGPUShader(supra_gsc->Device, vertexShader);
	SDL_ReleaseGPUShader(supra_gsc->Device, fragmentShader);
	
  return 0;
}

int Draw()
{
    SDL_GPUCommandBuffer* cmdbuf = SDL_AcquireGPUCommandBuffer(supra_gsc->Device);
    if (cmdbuf == NULL) {
        SDL_Log("AcquireGPUCommandBuffer failed: %s", SDL_GetError());
        return -1;
    }

    SDL_GPUTexture* swapchainTexture;
    if (!SDL_WaitAndAcquireGPUSwapchainTexture(cmdbuf, supra_gsc->Window, &swapchainTexture, NULL, NULL)) {
        SDL_Log("WaitAndAcquireGPUSwapchainTexture failed: %s", SDL_GetError());
        return -1;
    }

	if (swapchainTexture != NULL) {
		SDL_GPUColorTargetInfo colorTargetInfo = { 0 };
		colorTargetInfo.texture = swapchainTexture;
		colorTargetInfo.clear_color = (SDL_FColor){ 0.0f, 0.0f, 0.0f, 1.0f };
		colorTargetInfo.load_op = SDL_GPU_LOADOP_CLEAR;
		colorTargetInfo.store_op = SDL_GPU_STOREOP_STORE;

		SDL_GPURenderPass* renderPass = SDL_BeginGPURenderPass(cmdbuf, &colorTargetInfo, 1, NULL);
		SDL_BindGPUGraphicsPipeline(renderPass, supra_gsc->Pipeline2D);
		SDL_DrawGPUPrimitives(renderPass, 3, 1, 0, 0);
		SDL_EndGPURenderPass(renderPass);
	}

	SDL_SubmitGPUCommandBuffer(cmdbuf);

	return 0;
}

SDL_GPUShader* LoadShader(
	SDL_GPUDevice* device,
	const char* shaderFilename,
	Uint32 samplerCount,
	Uint32 uniformBufferCount,
	Uint32 storageBufferCount,
	Uint32 storageTextureCount
) {
	// Auto-detect the shader stage from the file name for convenience
	SDL_GPUShaderStage stage;
	if (SDL_strstr(shaderFilename, ".vert"))
	{
		stage = SDL_GPU_SHADERSTAGE_VERTEX;
	}
	else if (SDL_strstr(shaderFilename, ".frag"))
	{
		stage = SDL_GPU_SHADERSTAGE_FRAGMENT;
	}
	else
	{
		SDL_Log("Invalid shader stage!");
		return NULL;
	}

	char fullPath[256];
	SDL_GPUShaderFormat backendFormats = SDL_GetGPUShaderFormats(device);
	SDL_GPUShaderFormat format = SDL_GPU_SHADERFORMAT_INVALID;
	const char *entrypoint;

	if (backendFormats & SDL_GPU_SHADERFORMAT_SPIRV) {
		SDL_snprintf(fullPath, sizeof(fullPath), "%sshaders/SPIRV/%s.spv", supra_gsc->BasePath, shaderFilename);
		format = SDL_GPU_SHADERFORMAT_SPIRV;
		entrypoint = "main";
	}

	size_t codeSize;
	void* code = SDL_LoadFile(fullPath, &codeSize);
	if (code == NULL)
	{
		SDL_Log("Failed to load shader from disk! %s", fullPath);
		return NULL;
	}

	SDL_GPUShaderCreateInfo shaderInfo = {
		.code = code,
		.code_size = codeSize,
		.entrypoint = entrypoint,
		.format = format,
		.stage = stage,
		.num_samplers = samplerCount,
		.num_uniform_buffers = uniformBufferCount,
		.num_storage_buffers = storageBufferCount,
		.num_storage_textures = storageTextureCount
	};
	SDL_GPUShader* shader = SDL_CreateGPUShader(device, &shaderInfo);
	if (shader == NULL)
	{
		SDL_Log("Failed to create shader!");
		SDL_free(code);
		return NULL;
	}

	SDL_free(code);
	return shader;
}

void Quit() {
	SDL_ReleaseGPUGraphicsPipeline(supra_gsc->Device, supra_gsc->Pipeline2D);
	SDL_ReleaseWindowFromGPUDevice(supra_gsc->Device, supra_gsc->Window);
	SDL_DestroyWindow(supra_gsc->Window);
	SDL_DestroyGPUDevice(supra_gsc->Device);
}
