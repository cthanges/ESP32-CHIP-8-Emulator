#include "Platform.hpp"
#include <SDL.h>
#include <cmath>

static const int AMPLITUDE = 28000;
static const int SAMPLE_RATE = 44100;
static const double TONE_FREQ = 440.0; // A4

// Audio callback used by SDL to fill the audio buffer
static void AudioCallback(void* user_data, Uint8* raw_buffer, int bytes)
{
	Sint16* buffer = (Sint16*)raw_buffer;
	int length = bytes / 2; // 2 bytes per sample for AUDIO_S16SYS
	int &sample_nr = *(int*)user_data;

	for (int i = 0; i < length; ++i, ++sample_nr)
	{
		double time = (double)sample_nr / (double)SAMPLE_RATE;
		const double PI = 3.14159265358979323846;
		buffer[i] = (Sint16)(AMPLITUDE * sin(2.0 * PI * TONE_FREQ * time));
	}
}

Platform::Platform(char const* title, int windowWidth, int windowHeight, int textureWidth, int textureHeight)
{
	SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO);

	window = SDL_CreateWindow(
		title,
		SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
		windowWidth, windowHeight,
		SDL_WINDOW_SHOWN);

	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

	texture = SDL_CreateTexture(
		renderer,
		SDL_PIXELFORMAT_RGBA8888,
		SDL_TEXTUREACCESS_STREAMING,
		textureWidth, textureHeight);

	// Setup audio
	SDL_AudioSpec want;
	SDL_zero(want);
	want.freq = SAMPLE_RATE;
	want.format = AUDIO_S16SYS;
	want.channels = 1;
	want.samples = 2048;
	want.callback = AudioCallback;
	want.userdata = &sampleNr;

	SDL_AudioSpec have;
	audioDevice = SDL_OpenAudioDevice(NULL, 0, &want, &have, 0);
	if (audioDevice == 0)
	{
		SDL_Log("Failed to open audio: %s", SDL_GetError());
	}
	else
	{
		SDL_PauseAudioDevice(audioDevice, 1); // Start paused; beep will unpause when needed
	}
}

Platform::~Platform()
{
	SDL_DestroyTexture(texture);
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	if (audioDevice != 0)
	{
		SDL_CloseAudioDevice(audioDevice);
	}
	SDL_Quit();
}

void Platform::Update(void const* buffer, int pitch)
{
	SDL_UpdateTexture(texture, nullptr, buffer, pitch);
	SDL_RenderClear(renderer);
	SDL_RenderCopy(renderer, texture, nullptr, nullptr);
	SDL_RenderPresent(renderer);
}

bool Platform::ProcessInput(uint8_t* keys)
{
	bool quit = false;

	SDL_Event event;

	while (SDL_PollEvent(&event))
	{
		switch (event.type)
		{
			case SDL_QUIT:
			{
				quit = true;
			} break;

			case SDL_KEYDOWN:
			{
				switch (event.key.keysym.sym)
				{
					case SDLK_ESCAPE:
					{
						quit = true;
					} break;

					case SDLK_x:
					{
						keys[0] = 1;
					} break;

					case SDLK_1:
					{
						keys[1] = 1;
					} break;

					case SDLK_2:
					{
						keys[2] = 1;
					} break;

					case SDLK_3:
					{
						keys[3] = 1;
					} break;

					case SDLK_q:
					{
						keys[4] = 1;
					} break;

					case SDLK_w:
					{
						keys[5] = 1;
					} break;

					case SDLK_e:
					{
						keys[6] = 1;
					} break;

					case SDLK_a:
					{
						keys[7] = 1;
					} break;

					case SDLK_s:
					{
						keys[8] = 1;
					} break;

					case SDLK_d:
					{
						keys[9] = 1;
					} break;

					case SDLK_z:
					{
						keys[0xA] = 1;
					} break;

					case SDLK_c:
					{
						keys[0xB] = 1;
					} break;

					case SDLK_4:
					{
						keys[0xC] = 1;
					} break;

					case SDLK_r:
					{
						keys[0xD] = 1;
					} break;

					case SDLK_f:
					{
						keys[0xE] = 1;
					} break;

					case SDLK_v:
					{
						keys[0xF] = 1;
					} break;
				}
			} break;

			case SDL_KEYUP:
			{
				switch (event.key.keysym.sym)
				{
					case SDLK_x:
					{
						keys[0] = 0;
					} break;

					case SDLK_1:
					{
						keys[1] = 0;
					} break;

					case SDLK_2:
					{
						keys[2] = 0;
					} break;

					case SDLK_3:
					{
						keys[3] = 0;
					} break;

					case SDLK_q:
					{
						keys[4] = 0;
					} break;

					case SDLK_w:
					{
						keys[5] = 0;
					} break;

					case SDLK_e:
					{
						keys[6] = 0;
					} break;

					case SDLK_a:
					{
						keys[7] = 0;
					} break;

					case SDLK_s:
					{
						keys[8] = 0;
					} break;

					case SDLK_d:
					{
						keys[9] = 0;
					} break;

					case SDLK_z:
					{
						keys[0xA] = 0;
					} break;

					case SDLK_c:
					{
						keys[0xB] = 0;
					} break;

					case SDLK_4:
					{
						keys[0xC] = 0;
					} break;

					case SDLK_r:
					{
						keys[0xD] = 0;
					} break;

					case SDLK_f:
					{
						keys[0xE] = 0;
					} break;

					case SDLK_v:
					{
						keys[0xF] = 0;
					} break;
				}
			} break;
		}
	}

	return quit;
}

void Platform::Beep(bool on)
{
	if (audioDevice == 0)
		return;
	if (on)
	{
		SDL_PauseAudioDevice(audioDevice, 0); // Unpause/start
	}
	else
	{
		SDL_PauseAudioDevice(audioDevice, 1); // Pause/stop
		sampleNr = 0; // Reset the sample number to start the tone from the beginning next time
	}
}