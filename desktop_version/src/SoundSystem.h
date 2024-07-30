#ifndef SOUNDSYSTEM_H
#define SOUNDSYSTEM_H

#ifndef __PS2__
#include <SDL_mixer.h>
#else
#include <SDL.h>
#endif

class MusicTrack
{
public:
	MusicTrack(const char* fileName);
	MusicTrack(SDL_RWops *rw);
#ifndef __PS2__
	Mix_Music *m_music;
#endif
	bool m_isValid;
};

class SoundTrack
{
public:
	SoundTrack(const char* fileName);
#ifndef __PS2__
	Mix_Chunk *sound;
#endif
};

class SoundSystem
{
public:
	SoundSystem();
};

#endif /* SOUNDSYSTEM_H */
