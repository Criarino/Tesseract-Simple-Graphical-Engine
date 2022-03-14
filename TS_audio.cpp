#include "TSSGE.h"

TS_audio::~TS_audio()
{
	SDL_CloseAudioDevice(deviceID);
	SDL_FreeWAV(wavBuffer);
}

void TS_audio::Adicionar(string filename)
{
	SDL_LoadWAV(filename.c_str(), &wavSpec, &wavBuffer, &wavLength);//carrega um arquivo
	deviceID=SDL_OpenAudioDevice(NULL, 0, &wavSpec, NULL, 0);//abre o audio(default nesse caso)
}

void TS_audio::Play()
{
	SDL_QueueAudio(deviceID, wavBuffer, wavLength);//coloca na fila?
	SDL_PauseAudioDevice(deviceID, 0);
}
